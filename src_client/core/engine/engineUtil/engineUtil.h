#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------
// 領域確保関数ラッパ メモリリーク調査のためにmalloc, calloc, freeの代わりにこれらを用いる

//#define FUHAHA_MEMORY_DEBUG

#ifdef FUHAHA_MEMORY_DEBUG
void* engineUtilMemoryMallocImplement(char *info, size_t size);
void* engineUtilMemoryCallocImplement(char *info, size_t n, size_t size);
char* engineUtilMemoryStrdupImplement(char *info, char *src);
void engineUtilMemoryFreeImplement(char *info, void *ptr);
void engineUtilMemoryTraceImplement(char *info);
#define ENGINEUTILMEMORY_STRINGIFY(n) #n
#define ENGINEUTILMEMORY_TOSTRING(n) ENGINEUTILMEMORY_STRINGIFY(n) 
#define ENGINEUTILMEMORY_LINEINFO __FILE__ ":" ENGINEUTILMEMORY_TOSTRING(__LINE__)
#define engineUtilMemoryMalloc(size) engineUtilMemoryMallocImplement(ENGINEUTILMEMORY_LINEINFO, size)
#define engineUtilMemoryCalloc(n, size) engineUtilMemoryCallocImplement(ENGINEUTILMEMORY_LINEINFO, n, size)
#define engineUtilMemoryStrdup(src) engineUtilMemoryStrdupImplement(ENGINEUTILMEMORY_LINEINFO, src)
#define engineUtilMemoryFree(ptr) engineUtilMemoryFreeImplement(ENGINEUTILMEMORY_LINEINFO, ptr)
#define engineUtilMemoryInfoMalloc(info, size) engineUtilMemoryMallocImplement(info, size)
#define engineUtilMemoryInfoCalloc(info, n, size) engineUtilMemoryCallocImplement(info, n, size)
#define engineUtilMemoryInfoStrdup(info, src) engineUtilMemoryStrdupImplement(info, src)
#define engineUtilMemoryInfoFree(info, ptr) engineUtilMemoryFreeImplement(info, ptr)
#define engineUtilMemoryTrace(info) engineUtilMemoryTraceImplement(info)
#else
#define engineUtilMemoryMalloc(size) malloc(size)
#define engineUtilMemoryCalloc(n, size) calloc(n, size)
#define engineUtilMemoryStrdup(src) strdup(src)
#define engineUtilMemoryFree(ptr) free(ptr)
#define engineUtilMemoryInfoMalloc(info, size) malloc(size)
#define engineUtilMemoryInfoCalloc(info, n, size) calloc(n, size)
#define engineUtilMemoryInfoStrdup(info, src) strdup(src)
#define engineUtilMemoryInfoFree(info, ptr) free(ptr)
#define engineUtilMemoryTrace(info)
#endif

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// ゲーム終了
void engineUtilSystemExit(void);

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

