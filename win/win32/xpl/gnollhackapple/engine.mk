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
else ifeq ($(PLATFORM),macos)
SDK := macosx
TARGET_FLAGS := -arch arm64 -arch x86_64 -mmacosx-version-min=$(MIN_OS)
PLATFORM_DEFINES := -DGNH_MAC
PLATFORM_DIR := win/win32/xpl/gnollhackmac
TARGET := $(OUTDIR)/libgnollhackmac.dylib
else
$(error PLATFORM must be ios or macos)
endif

CC := xcrun --sdk $(SDK) clang
LIBTOOL := xcrun --sdk $(SDK) libtool

CFLAGS := $(TARGET_FLAGS) -std=gnu99 -g -fno-strict-aliasing $(CONFIG_FLAGS) $(DEFINES) $(PLATFORM_DEFINES) \
	-I$(PLATFORM_DIR) -Iinclude -Iwin/win32/xpl/libshare \
	-Wall -Wno-enum-enum-conversion -Wno-deprecated-declarations -Wno-pointer-sign -Wno-format

OBJS := $(SOURCES:%.c=$(OBJDIR)/%.o)

# Every object depends on every header: compiler dependency files cannot be written for two -arch at once
HEADERS := $(wildcard include/*.h win/win32/xpl/libshare/*.h $(PLATFORM_DIR)/*.h)

.PHONY: all
all: $(TARGET)

$(OBJDIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/libgnollhackios.a: $(OBJS)
	@mkdir -p $(OUTDIR)
	rm -f $@
	$(LIBTOOL) -static -o $@ $(OBJS)

$(OUTDIR)/libgnollhackmac.dylib: $(OBJS)
	@mkdir -p $(OUTDIR)
	$(CC) $(TARGET_FLAGS) -dynamiclib -install_name @rpath/libgnollhackmac.dylib -o $@ $(OBJS) -lm -lpthread
