#include "gnollhackdroid.h"
#include "hack.h"
#include "libproc.h"

extern int GnollHackMain(int argc, char** argv);

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "gnollhackdroid", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "gnollhackdroid", __VA_ARGS__))

/* This trivial function returns the platform ABI for which this dynamic native library is compiled.*/
const char *getPlatformABI()
{
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
	#define ABI "armeabi-v7a/NEON"
#else
	#define ABI "armeabi-v7a"
#endif
#else
	#define ABI "armeabi"
#endif
#elif defined(__i386__)
	#define ABI "x86"
#else
	#define ABI "unknown"
#endif
	LOGI("This dynamic shared library is compiled with ABI: %s", ABI);
	return "This native library is compiled with ABI: %s" ABI ".";
}

void gnollhackdroid()
{

}

int DoSomeCalcDroid()
{
	return 5;
}


void
libdef_raw_print(s)
const char* s;
{
	puts(s);
}

void
libdef_wait_synch(VOID_ARGS)
{
	/* Config file error handling routines
	 * call wait_sync() without checking to
	 * see if it actually has a value,
	 * leading to spectacular violations
	 * when you try to execute address zero.
	 * The existence of this allows early
	 * processing to have something to execute
	 * even though it essentially does nothing
	 */
	return;
}

int RunGnollHackTest(char* gnhdir)
{
	char* params[2] = { 0 };

	params[0] = "gnollhack";
	params[1] = 0;

	lib_procs.win_raw_print = libdef_raw_print;
	lib_procs.win_wait_synch = libdef_wait_synch;

#if 0
	{
		char cwbuf[BUFSIZ] = "";
		char* curwd = getcwd(cwbuf, sizeof(cwbuf));
		char* curwd2 = argv[2];
		int res = chdir(curwd2);
		int e = errno;

		char dirbuf[BUFSZ];
		Sprintf(dirbuf, "%s%s", curwd2, "");
		int cnt = 0;
		DIR* dir;
		struct dirent* ent;
		if ((dir = opendir(dirbuf)) != NULL) {
			/* print all the files and directories within directory */
			while ((ent = readdir(dir)) != NULL) {
				Sprintf(dirbuf, "%s\n", ent->d_name);
				cnt++;
			}
			closedir(dir);
		}
		else {
			/* could not open directory */
			e = errno;
			return EXIT_FAILURE;
		}
		char cwbuf[BUFSIZ] = "";
		char* curwd = getcwd(cwbuf, sizeof(cwbuf));
		char* curwd2 = argv[2];
		int res = chdir(curwd2);
		int e = errno;

		char dirbuf[BUFSZ];
		Sprintf(dirbuf, "%s%s", curwd2, "");
		int cnt = 0;
		DIR* dir;
		struct dirent* ent;
		if ((dir = opendir(dirbuf)) != NULL) {
			/* print all the files and directories within directory */
			while ((ent = readdir(dir)) != NULL) {
				Sprintf(dirbuf, "%s\n", ent->d_name);
				cnt++;
			}
			closedir(dir);
		}
		else {
			/* could not open directory */
			e = errno;
			return EXIT_FAILURE;
		}
	}
#endif


	int res = chdir(gnhdir);
	if (res != 0)
	{
		/* Failed to change to right directory */
		return 0;
	}

	return GnollHackMain(1, params);
}
