# GnollHack engine library for an Apple platform: libgnollhackios.a (iOS) or libgnollhackmac.dylib (macOS).
# Run on the Mac by Build-AppleEngine.ps1, from the root of the unpacked source tree:
#   make -f engine.mk PLATFORM=ios|macos CONFIG=Debug|Release MIN_OS=<version> OBJDIR=<dir> OUTDIR=<dir>
# gnh-sources.mk, written next to this file by the script, sets SOURCES to the repository-relative C sources.
# Uses only GNU make 3.81 features, the version macOS ships.

include gnh-sources.mk

OBJDIR ?= obj
OUTDIR ?= out

DEFINES := -DGNH_MOBILE -DUSE_TILES -DDLB -DLIB_GRAPHICS -DSAFEPROCS -DNOTTYGRAPHICS \
	-DGNOLLHACK_MAIN_PROGRAM -DEXIT_THREAD_ON_EXIT -DDUMPLOG -DDUMPHTML

# Frame pointers follow the compiler's default for the target
ifeq ($(CONFIG),Debug)
CONFIG_FLAGS := -O0 -D_DEBUG -DDEBUG
else ifeq ($(CONFIG),Release)
CONFIG_FLAGS := -O3 -DNDEBUG
else
$(error CONFIG must be Debug or Release)
endif

ifeq ($(MIN_OS),)
$(error MIN_OS is required)
endif

ifeq ($(PLATFORM),ios)
SDK := iphoneos
TARGET_FLAGS := -target arm64-apple-ios$(MIN_OS)
PLATFORM_DEFINES := -DGNH_IOS
PLATFORM_DIR := win/win32/xpl/gnollhackios
TARGET := $(OUTDIR)/libgnollhackios.a
TARGET_DSYM :=
else ifeq ($(PLATFORM),macos)
SDK := macosx
TARGET_FLAGS := -arch arm64 -arch x86_64 -mmacosx-version-min=$(MIN_OS)
PLATFORM_DEFINES := -DGNH_MAC
PLATFORM_DIR := win/win32/xpl/gnollhackmac
TARGET := $(OUTDIR)/libgnollhackmac.dylib
TARGET_DSYM := $(TARGET).dSYM
else
$(error PLATFORM must be ios or macos)
endif

CC := xcrun --sdk $(SDK) clang
LIBTOOL := xcrun --sdk $(SDK) libtool

CFLAGS := $(TARGET_FLAGS) -std=gnu99 -g -fno-strict-aliasing $(CONFIG_FLAGS) $(DEFINES) $(PLATFORM_DEFINES) \
	-I$(PLATFORM_DIR) -Iinclude -Iwin/win32/xpl/libshare \
	-Wall -Wno-enum-enum-conversion -Wno-deprecated-declarations -Wno-pointer-sign -Wno-format

# Objects depend on this file, rewritten only when the compiler, its flags or the SDK change
SDK_PATH := $(shell xcrun --sdk $(SDK) --show-sdk-path)
FLAGS_STAMP := $(OBJDIR)/flags.txt
FLAGS_TEXT := $(CC) $(CFLAGS) $(SDK_PATH)
$(shell mkdir -p $(OBJDIR) && printf '%s\n' '$(FLAGS_TEXT)' | cmp -s - $(FLAGS_STAMP) || printf '%s\n' '$(FLAGS_TEXT)' > $(FLAGS_STAMP))
$(info Compiler: $(CC) $(CFLAGS))

OBJS := $(SOURCES:%.c=$(OBJDIR)/%.o)

# Every object depends on every header: compiler dependency files cannot be written for two -arch at once
HEADERS := $(wildcard include/*.h win/win32/xpl/libshare/*.h $(PLATFORM_DIR)/*.h)

.PHONY: all
all: $(TARGET) $(TARGET_DSYM)

# WORKAROUND(WA-BUILD-0004): Xcode's GNU make 3.81 has no --output-sync
# Each compile's diagnostics go to a file and are printed in one piece under a lock, so parallel jobs do not
# interleave them
$(OBJDIR)/%.o: %.c $(HEADERS) engine.mk $(FLAGS_STAMP)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@ 2> $@.log; status=$$?; lockf -k $(OBJDIR)/output.lock cat $@.log >&2; rm -f $@.log; exit $$status

$(OUTDIR)/libgnollhackios.a: $(OBJS) gnh-sources.mk
	@mkdir -p $(OUTDIR)
	rm -f $@
	$(LIBTOOL) -static -o $@ $(OBJS)

$(OUTDIR)/libgnollhackmac.dylib: $(OBJS) gnh-sources.mk
	@mkdir -p $(OUTDIR)
	$(CC) $(TARGET_FLAGS) -dynamiclib -install_name @rpath/libgnollhackmac.dylib -o $@ $(OBJS) -lm -lpthread

# The engine's DWARF stays in the objects; dsymutil collects it while they exist
$(OUTDIR)/libgnollhackmac.dylib.dSYM: $(OUTDIR)/libgnollhackmac.dylib
	rm -rf $@
	xcrun dsymutil $< -o $@
