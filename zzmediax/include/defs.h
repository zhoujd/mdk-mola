#ifndef COMMON_H
#define COMMON_H 1

#include <stdio.h>


#if defined( _WIN32 ) || defined ( _WIN64 )
  #include <tchar.h>
  #define __INT64   __int64
  #define __UINT64  unsigned __int64
#else
  #define __INT64   long long
  #define __UINT64  unsigned long long
#endif

typedef unsigned char       zzU8;
typedef char                zzI8;
typedef short               zzI16;
typedef unsigned short      zzU16;
typedef unsigned int        zzU32;
typedef int                 zzI32;
typedef unsigned long       zzUL32;
typedef long                zzL32;
typedef float               zzF32;
typedef double              zzF64;
typedef __UINT64            zzU64;
typedef __INT64             zzI64;
typedef void*               zzHDL;
typedef zzHDL               zzMemId;
typedef void*               zzThreadTask;

typedef struct zzI16PairST
{
    zzI16  x;
    zzI16  y;
} zzI16PairST;

typedef int zzBOOL;

#define TRUE  1
#define FALSE 0



/*********************************************************************************\
Error message
\*********************************************************************************/
typedef enum
{
    /* no error */
    ZZ_ERR_NONE                        = 0,    /* no error */

    /* reserved for unexpected errors */
    ZZ_ERR_UNKNOWN                     = -1,   /* unknown error. */

    /* error codes <0 */
    ZZ_ERR_NULL_PTR                    = -2,   /* null pointer */
    ZZ_ERR_UNSUPPORTED                 = -3,   /* undeveloped feature */
    ZZ_ERR_MEMORY_ALLOC                = -4,   /* failed to allocate memory */
    ZZ_ERR_NOT_ENOUGH_BUFFER           = -5,   /* insufficient buffer at input/output */
    ZZ_ERR_INVALID_HANDLE              = -6,   /* invalid handle */
    ZZ_ERR_LOCK_MEMORY                 = -7,   /* failed to lock the memory block */
    ZZ_ERR_NOT_INITIALIZED             = -8,   /* member function called before initialization */
    ZZ_ERR_NOT_FOUND                   = -9,   /* the specified object is not found */
    ZZ_ERR_MORE_DATA                   = -10,  /* expect more data at input */
    ZZ_ERR_MORE_SURFACE                = -11,  /* expect more surface at output */
    ZZ_ERR_ABORTED                     = -12,  /* operation aborted */
    ZZ_ERR_DEVICE_LOST                 = -13,  /* lose the HW acceleration device */
    ZZ_ERR_INCOMPATIBLE_VIDEO_PARAM    = -14,  /* incompatible video parameters */
    ZZ_ERR_INVALID_VIDEO_PARAM         = -15,  /* invalid video parameters */
    ZZ_ERR_UNDEFINED_BEHAVIOR          = -16,  /* undefined behavior */
    ZZ_ERR_DEVICE_FAILED               = -17,  /* device operation failure */
    ZZ_ERR_MORE_BITSTREAM              = -18,  /* expect more bitstream buffers at output */

    ZZ_ERR_EOF_STREAM                  = -19,

    /* warnings >0 */
    ZZ_WRN_IN_EXECUTION                = 1,    /* the previous asynchrous operation is in execution */
    ZZ_WRN_DEVICE_BUSY                 = 2,    /* the HW acceleration device is busy */
    ZZ_WRN_VIDEO_PARAM_CHANGED         = 3,    /* the video parameters are changed during decoding */
    ZZ_WRN_PARTIAL_ACCELERATION        = 4,    /* SW is used */
    ZZ_WRN_INCOMPATIBLE_VIDEO_PARAM    = 5,    /* incompatible video parameters */
    ZZ_WRN_VALUE_NOT_CHANGED           = 6,    /* the value is saturated based on its valid range */
    ZZ_WRN_OUT_OF_RANGE                = 7,    /* the value is out of valid range */

} zzStatus;

#define ZZ_MAKEFOURCC(A,B,C,D)    ((((int)A))+(((int)B)<<8)+(((int)C)<<16)+(((int)D)<<24))

/* FourCC */
enum
{
    ZZ_FOURCC_NV12         = ZZ_MAKEFOURCC('N','V','1','2'),   /* Native Format */
    ZZ_FOURCC_NV12_LINEAR  = ZZ_MAKEFOURCC('N','V','1','3'),
    ZZ_FOURCC_YV12         = ZZ_MAKEFOURCC('Y','V','1','2'),
    ZZ_FOURCC_IYUV         = ZZ_MAKEFOURCC('I','Y','U','V'),
    ZZ_FOURCC_IMC3         = ZZ_MAKEFOURCC('I','M','C','3'),
    ZZ_FOURCC_411P         = ZZ_MAKEFOURCC('4','1','1','P'),
    ZZ_FOURCC_400P         = ZZ_MAKEFOURCC('4','0','0','P'),
    ZZ_FOURCC_444P         = ZZ_MAKEFOURCC('4','4','4','P'),
    ZZ_FOURCC_422H         = ZZ_MAKEFOURCC('4','2','2','H'),
    ZZ_FOURCC_422V         = ZZ_MAKEFOURCC('4','2','2','V'),
    ZZ_FOURCC_YUY2         = ZZ_MAKEFOURCC('Y','U','Y','2'),
    ZZ_FOURCC_ABGR         = ZZ_MAKEFOURCC('A','B','G','R'),    /* ABGR */
    ZZ_FOURCC_ARGB         = ZZ_MAKEFOURCC('A','R','G','B'),    /* ARGB */
    ZZ_FOURCC_R5G6B5       = ZZ_MAKEFOURCC('R','G','1','6'),    /* R5G6B5 */
    ZZ_FOURCC_XRGB         = ZZ_MAKEFOURCC('X','R','G','B'),    /* XRGB */
};

/* PicStruct */
enum
{
    ZZ_PICSTRUCT_UNKNOWN       =0x00,
    ZZ_PICSTRUCT_PROGRESSIVE   =0x01,
    ZZ_PICSTRUCT_FIELD_TFF     =0x02,
    ZZ_PICSTRUCT_FIELD_BFF     =0x04,

    ZZ_PICSTRUCT_FIELD_REPEATED=0x10,  /* first field repeated, pic_struct=5 or 6 in H.264 */
    ZZ_PICSTRUCT_FRAME_DOUBLING=0x20,  /* pic_struct=7 in H.264 */
    ZZ_PICSTRUCT_FRAME_TRIPLING=0x40   /* pic_struct=8 in H.264 */
};



#endif //COMMON_H
