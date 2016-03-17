#ifndef __platform_h_
#define __platform_h_

#include <GLES2/gl2.h>

#include <emscripten/emscripten.h>
#define trace(...) emscripten_log(EM_LOG_CONSOLE, __VA_ARGS__)

#endif

