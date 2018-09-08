#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 領域確保関数ラッパ メモリリーク調査のためにmalloc, calloc, freeの代わりにこれらを用いる

//#define FUHAHA_MEMORY_DEBUG

#ifdef FUHAHA_MEMORY_DEBUG
char *engineUtilMemoryInfoImplement(char *info, char *filePath, int line);
void *engineUtilMemoryMallocImplement(char *info, size_t size);
void *engineUtilMemoryCallocImplement(char *info, size_t n, size_t size);
char *engineUtilMemoryStrdupImplement(char *info, char *src);
void engineUtilMemoryFreeImplement(char *info, void *ptr);
void engineUtilMemoryTraceImplement(char *info);
#define engineUtilMemoryInfo(info, filePath, line) engineUtilMemoryInfoImplement(info, filePath, line)
#define engineUtilMemoryInfoMalloc(info, size) engineUtilMemoryMallocImplement(engineUtilMemoryInfo(info, __FILE__, __LINE__), size)
#define engineUtilMemoryInfoCalloc(info, n, size) engineUtilMemoryCallocImplement(engineUtilMemoryInfo(info, __FILE__, __LINE__), n, size)
#define engineUtilMemoryInfoStrdup(info, src) engineUtilMemoryStrdupImplement(engineUtilMemoryInfo(info, __FILE__, __LINE__), src)
#define engineUtilMemoryInfoFree(info, ptr) engineUtilMemoryFreeImplement(engineUtilMemoryInfo(info, __FILE__, __LINE__), ptr)
#define engineUtilMemoryMalloc(size) engineUtilMemoryMallocImplement(engineUtilMemoryInfo(NULL, __FILE__, __LINE__), size)
#define engineUtilMemoryCalloc(n, size) engineUtilMemoryCallocImplement(engineUtilMemoryInfo(NULL, __FILE__, __LINE__), n, size)
#define engineUtilMemoryStrdup(src) engineUtilMemoryStrdupImplement(engineUtilMemoryInfo(NULL, __FILE__, __LINE__), src)
#define engineUtilMemoryFree(ptr) engineUtilMemoryFreeImplement(engineUtilMemoryInfo(NULL, __FILE__, __LINE__), ptr)
#define engineUtilMemoryTrace(info) engineUtilMemoryTraceImplement(engineUtilMemoryInfo(info, __FILE__, __LINE__))
#else
#define engineUtilMemoryInfo(info, filePath, line) ""
#define engineUtilMemoryInfoMalloc(info, size) malloc(size)
#define engineUtilMemoryInfoCalloc(info, n, size) calloc(n, size)
#define engineUtilMemoryInfoStrdup(info, src) strdup(src)
#define engineUtilMemoryInfoFree(info, ptr) free(ptr)
#define engineUtilMemoryMalloc(size) malloc(size)
#define engineUtilMemoryCalloc(n, size) calloc(n, size)
#define engineUtilMemoryStrdup(src) strdup(src)
#define engineUtilMemoryFree(ptr) free(ptr)
#define engineUtilMemoryTrace(info)
#endif

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

