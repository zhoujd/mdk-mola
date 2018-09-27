#ifndef DEFS_H
#define DEFS_H 1

#include <stddef.h>
#include "defs.h"
#include "file_defs.h"
#include "string_defs.h"
#include "list.h"


#ifndef ANDROID
#define ZZPRINTF           printf
#define ZZDEBUG(...)
#else
#define ZZPRINTF(...)      __android_log_print(ANDROID_LOG_ERROR, "ZZMEDIAX", __VA_ARGS__)
#endif //ANDROID

//extern declare
extern void*      ZZAllocAndZeroMem(size_t size);
extern zzStatus   ZZInfoPrint(zz_char*  pStrInfoFile, zz_char* format, ...);

#define ZZINFO(...)        ZZInfoPrint("zz_info.txt", __VA_ARGS__)
#define AllocAndZeroMem    ZZAllocAndZeroMem

#define ZZ_ALIGN(x,a)        (((x)+((a)-1))&~((a)-1))
#define ZZ_ALIGN32(value)    (((value + 31) >> 5) << 5) // round up to a multiple of 32
#define ZZ_MAX( a, b )       (((a) > (b)) ? (a) : (b))
#define ZZ_MIN( a, b )       (((a) < (b)) ? (a) : (b))

#define CHECK_RESULT_SAFE(P, X, ERR, ADD)   {if ((X) != (P)) {ADD; return ERR;}}
#define CHECK_POINTER(P, ERR)               {if (!(P)) {return ERR;}}
#define CHECK_POINTER_NO_RET(P)             {if (!(P)) {return;}}
#define CHECK_POINTER_SAFE(P, ERR, ADD)     {if (!(P)) {ADD; return ERR;}}
#define FREEIF(P)                           {if (P) {free(P); P = 0;}}
#define CHECK_NOT_EQUAL(P, X, ERR)          {if ((X) != (P)) {return ERR;}}
#define ZERO_MEMORY(VAR)                    {memset(&VAR, 0, sizeof(VAR));}
#define ARRAY_NUM(arr)                      (sizeof(arr)/sizeof(arr[0]))
#define VAL_CHECK(val)                      {if (val) return ZZ_ERR_UNKNOWN;}

#define IOSTREAM_CHECK_NOT_EQUAL(P, X, ERR)          {if ((X) != (P)) {return ERR;}}

#ifndef ANDROID //not for Android
#define rdtscll(val) do { \
     unsigned int __a,__d; \
     asm volatile("rdtsc" : "=a" (__a), "=d" (__d)); \
     (val) = ((unsigned long)__a) | (((unsigned long)__d)<<32); \
} while(0)
#else
#define rdtscll
#endif //not for Android

#endif //DEFS_H
