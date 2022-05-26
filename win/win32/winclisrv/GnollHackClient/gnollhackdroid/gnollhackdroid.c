/* GnollHack 4.1.0 gnollhackdroid.c */
/* Copyright (c) Janne Gustafsson, 2022. */
/* GnollHack may be freely redistributed.  See license for details. */

#include "gnollhackdroid.h"

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

void gnollhackdroid(void)
{

}
