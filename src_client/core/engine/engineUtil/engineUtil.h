#pragma once

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

//#define FUHAHA_DEBUG

#ifdef FUHAHA_DEBUG
void* engineUtilMemoryMallocImplement(char *info, size_t size);
void* engineUtilMemoryCallocImplement(char *info, size_t n, size_t size);
void engineUtilMemoryFreeImplement(void *ptr);
void engineUtilMemoryTraceImplement();
#define ENGINEUTILMEMORY_STRINGIFY(n) #n
#define ENGINEUTILMEMORY_TOSTRING(n) ENGINEUTILMEMORY_STRINGIFY(n) 
#define ENGINEUTILMEMORY_LINEINFO __FILE__ ":" ENGINEUTILMEMORY_TOSTRING(__LINE__)
#define engineUtilMemoryMalloc(size) engineUtilMemoryMallocImplement(ENGINEUTILMEMORY_LINEINFO, size)
#define engineUtilMemoryCalloc(n, size) engineUtilMemoryCallocImplement(ENGINEUTILMEMORY_LINEINFO, n, size)
#define engineUtilMemoryFree(ptr) engineUtilMemoryFreeImplement(ptr)
#define engineUtilMemoryTrace() engineUtilMemoryTraceImplement()
#else
#define engineUtilMemoryMalloc(size) malloc(size)
#define engineUtilMemoryCalloc(n, size) calloc(n, size)
#define engineUtilMemoryFree(ptr) free(ptr)
#define engineUtilMemoryTrace()
#endif

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

