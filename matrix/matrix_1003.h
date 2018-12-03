/* matrix_1003.h ---
 */

#ifndef INCLUDED_ZZ_MATRIX_1003_H
#define INCLUDED_ZZ_MATRIX_1003_H 1

#include "cmn.h"
#include "matrix_defs.h"
#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>


#include "lib/frame_reader.h"
#include "lib/va_surf_mng.h"

#define MATRIX1003_ID    (1003)

typedef struct _zzMatrix1003ST zzMatrix1003ST;
typedef struct _zzMatrix1003InputParamsST zzMatrix1003InputParamsST;

struct _zzMatrix1003ST
{
    zzMatrixBaseST    base;

    zzSurfaceST       dst_surf;
    zzFrameReaderST   *pFrameReader;
    zzU16             frame_idx;
    zzU16             frame_num;

    zzBOOL            demo_flag;
};


#define GET_MATRIX1003(p)  container_of(p, zzMatrix1003ST, base)

zzStatus ZZMatrix1003_Create(zzMatrix1003ST **ppRet);
zzStatus ZZMatrix1003_Release(zzMatrix1003ST *pSelf);

zzStatus ZZMatrix1003_Init(zzMatrix1003ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl,
                           zzSurfaceST    *pSurf, zz_char *pFilename);

zzStatus ZZMatrix1003_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix1003_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_1003_H */
