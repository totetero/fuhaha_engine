#ifndef __platform_h_
#define __platform_h_

#include <GLES2/gl2.h>

#include <android/log.h>
#define trace(...) ((void)__android_log_print(ANDROID_LOG_INFO, "myon", __VA_ARGS__))

#endif

