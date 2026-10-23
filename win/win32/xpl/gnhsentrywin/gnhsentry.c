/* GnollHack 4.3.0 gnhsentry.c */
/* Copyright (c) Janne Gustafsson, 2026. */
/* GnollHack may be freely redistributed.  See license for details. */

/*
 * Native crash capture for the Windows build. gnhsentry.dll links sentry-native's inproc backend statically and
 * adds two pieces in front of it:
 *
 * - A vectored exception handler that hands hardware faults raised inside an allowlisted native module to
 *   sentry_handle_exception. Under managed frames such a fault ends in CoreCLR's fail-fast without reaching the
 *   unhandled-exception filter, so the vectored handler is the only place that sees it.
 * - An unhandled-exception filter in front of sentry-native's own filter, which never calls the filter it
 *   displaced. Hardware faults go to sentry-native and then to the filter that was current before sentry_init;
 *   every other exception goes to that filter directly.
 *
 * Call order: gnh_crash_prepare, sentry_init, gnh_crash_install; at shutdown gnh_crash_uninstall, then sentry_close.
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* Declarations as in include/sentry.h of sentry-native at tag 0.17.2 (_WIN32, static build) */
typedef struct sentry_ucontext_s
{
    EXCEPTION_POINTERS exception_ptrs;
} sentry_ucontext_t;

void sentry_handle_exception(const sentry_ucontext_t *uctx);

/* Modules whose faults are captured, by the name in their export directory */
static const char *const listed_modules[] =
{
    "gnollhackwin.dll",
    "libSkiaSharp.dll",
    "fmod.dll",
    "fmodstudio.dll",
    "libEGL.dll",
    "libGLESv2.dll",
    "SkiaSharp.Views.WinUI.Native.dll"
};

/* The unhandled-exception filter that was current before sentry_init */
static LPTOP_LEVEL_EXCEPTION_FILTER g_runtime_uef = NULL;

/* sentry-native's unhandled-exception filter, displaced by gnh_unhandled_filter */
static LPTOP_LEVEL_EXCEPTION_FILTER g_sentry_uef = NULL;

static int g_prepared = 0;
static int g_filter_installed = 0;
static PVOID g_vectored_handle = NULL;

/*
 * Access violations, illegal and privileged instructions, and in-page errors. Integer division by zero becomes a
 * catchable managed exception in CoreCLR, a stack overflow leaves no stack to capture on, and breakpoints are not
 * crashes.
 */
static int is_crash_code(DWORD code)
{
    switch (code)
    {
    case EXCEPTION_ACCESS_VIOLATION:
    case EXCEPTION_ILLEGAL_INSTRUCTION:
    case EXCEPTION_PRIV_INSTRUCTION:
    case EXCEPTION_IN_PAGE_ERROR:
        return 1;
    default:
        return 0;
    }
}

static char ascii_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return (char)(c - 'A' + 'a');
    }
    return c;
}

/* Case-insensitive ASCII comparison of a name that ends within max_length bytes with a NUL-terminated string */
static int names_equal(const char *name, DWORD max_length, const char *listed)
{
    DWORD i;

    for (i = 0; i < max_length; i++)
    {
        if (ascii_lower(name[i]) != ascii_lower(listed[i]))
        {
            return 0;
        }
        if (listed[i] == '\0')
        {
            return 1;
        }
    }
    return 0;
}

/*
 * Reads the export directory name of the 64-bit image mapped at base and looks it up in listed_modules. It runs
 * inside exception dispatch, so it reads the mapped headers directly and makes no loader call: taking the loader
 * lock there can deadlock.
 */
static int is_listed_image(PVOID base)
{
    const BYTE *image = (const BYTE *)base;
    const IMAGE_DOS_HEADER *dos_header;
    const IMAGE_NT_HEADERS64 *nt_headers;
    const IMAGE_DATA_DIRECTORY *export_entry;
    const IMAGE_EXPORT_DIRECTORY *export_directory;
    DWORD size_of_image;
    DWORD name_rva;
    size_t i;

    if (image == NULL)
    {
        return 0;
    }

    dos_header = (const IMAGE_DOS_HEADER *)image;
    if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
    {
        return 0;
    }
    /* The NT headers must lie in the first page, which is mapped for every loaded image */
    if (dos_header->e_lfanew < (LONG)sizeof(IMAGE_DOS_HEADER)
        || (DWORD)dos_header->e_lfanew > 0x1000 - sizeof(IMAGE_NT_HEADERS64))
    {
        return 0;
    }

    nt_headers = (const IMAGE_NT_HEADERS64 *)(image + dos_header->e_lfanew);
    if (nt_headers->Signature != IMAGE_NT_SIGNATURE
        || nt_headers->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC
        || nt_headers->OptionalHeader.NumberOfRvaAndSizes <= IMAGE_DIRECTORY_ENTRY_EXPORT)
    {
        return 0;
    }
    size_of_image = nt_headers->OptionalHeader.SizeOfImage;

    export_entry = &nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (export_entry->VirtualAddress == 0 || export_entry->Size == 0)
    {
        return 0;
    }
    if (export_entry->VirtualAddress >= size_of_image
        || size_of_image - export_entry->VirtualAddress < sizeof(IMAGE_EXPORT_DIRECTORY))
    {
        return 0;
    }

    export_directory = (const IMAGE_EXPORT_DIRECTORY *)(image + export_entry->VirtualAddress);
    name_rva = export_directory->Name;
    if (name_rva == 0 || name_rva >= size_of_image)
    {
        return 0;
    }

    for (i = 0; i < sizeof(listed_modules) / sizeof(listed_modules[0]); i++)
    {
        if (names_equal((const char *)(image + name_rva), size_of_image - name_rva, listed_modules[i]))
        {
            return 1;
        }
    }
    return 0;
}

/*
 * Captures a crash fault raised inside a listed module. The exception always continues unchanged, so CoreCLR's
 * fail-fast and WER still run. There is no one-shot guard here: sentry-native captures at most once per process and
 * makes a second crashing thread wait until the first capture is written, which an early return here would bypass.
 */
static LONG CALLBACK gnh_vectored_handler(PEXCEPTION_POINTERS info)
{
    PVOID base = NULL;
    sentry_ucontext_t uctx;

    if (info == NULL || info->ExceptionRecord == NULL || info->ContextRecord == NULL)
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    if (!is_crash_code(info->ExceptionRecord->ExceptionCode))
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    /* NULL for addresses outside any loaded image, such as JIT-compiled code */
    if (RtlPcToFileHeader(info->ExceptionRecord->ExceptionAddress, &base) == NULL)
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }
    if (!is_listed_image(base))
    {
        return EXCEPTION_CONTINUE_SEARCH;
    }

    uctx.exception_ptrs = *info;
    sentry_handle_exception(&uctx);
    return EXCEPTION_CONTINUE_SEARCH;
}

/*
 * Hardware faults go to sentry-native's filter first, which captures at most once per process. Every exception then
 * goes to the filter that was current before sentry_init: a CLR exception (0xE0434352) reaching this point, from the
 * main thread or from a managed callback on a native thread, must still reach CoreCLR's filter, which raises
 * AppDomain.UnhandledException and writes the "Unhandled exception." message and the ".NET Runtime" event-log entry.
 */
static LONG WINAPI gnh_unhandled_filter(EXCEPTION_POINTERS *info)
{
    if (info != NULL && info->ExceptionRecord != NULL && is_crash_code(info->ExceptionRecord->ExceptionCode)
        && g_sentry_uef != NULL)
    {
        (void)g_sentry_uef(info);
    }
    if (g_runtime_uef != NULL)
    {
        return g_runtime_uef(info);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

/*
 * Called before sentry_init. Records the current unhandled-exception filter; only the first call records, since a
 * later one would record sentry-native's filter.
 */
__declspec(dllexport) int gnh_crash_prepare(void)
{
    if (!g_prepared)
    {
        g_runtime_uef = SetUnhandledExceptionFilter(NULL);
        SetUnhandledExceptionFilter(g_runtime_uef);
        g_prepared = 1;
    }
    return 1;
}

/*
 * Called after sentry_init. Puts gnh_unhandled_filter in front of sentry-native's filter and adds the vectored
 * handler. Returns 0 without installing anything when gnh_crash_prepare was not called, or when the current filter
 * is not sentry-native's: wrapping the recorded filter itself would call it twice for a hardware fault. Idempotent;
 * returns 1 when both pieces are installed.
 */
__declspec(dllexport) int gnh_crash_install(void)
{
    LPTOP_LEVEL_EXCEPTION_FILTER previous;

    if (!g_prepared)
    {
        return 0;
    }

    if (!g_filter_installed)
    {
        previous = SetUnhandledExceptionFilter(gnh_unhandled_filter);
        if (previous == NULL || previous == g_runtime_uef)
        {
            SetUnhandledExceptionFilter(previous);
            return 0;
        }
        g_sentry_uef = previous;
        g_filter_installed = 1;
    }

    if (g_vectored_handle == NULL)
    {
        g_vectored_handle = AddVectoredExceptionHandler(1, gnh_vectored_handler);
    }
    return g_vectored_handle != NULL ? 1 : 0;
}

/*
 * Removes the vectored handler and gives the filter slot back to sentry-native's filter, keeping a filter that
 * someone else installed after gnh_unhandled_filter. Must run before sentry_close, which restores the filter it
 * displaced only while its own filter is the current one.
 */
__declspec(dllexport) void gnh_crash_uninstall(void)
{
    LPTOP_LEVEL_EXCEPTION_FILTER current;

    if (g_vectored_handle != NULL)
    {
        RemoveVectoredExceptionHandler(g_vectored_handle);
        g_vectored_handle = NULL;
    }

    if (g_filter_installed)
    {
        current = SetUnhandledExceptionFilter(g_sentry_uef);
        if (current != gnh_unhandled_filter)
        {
            SetUnhandledExceptionFilter(current);
        }
        g_sentry_uef = NULL;
        g_filter_installed = 0;
    }
}
