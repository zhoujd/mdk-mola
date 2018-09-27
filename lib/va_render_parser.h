/* va_render_parser.h ---
 */

#ifndef INCLUDED_ZZ_RENDER_PARSER_H
#define INCLUDED_ZZ_RENDER_PARSER_H 1


#include "va_render_utils.h"

zzStatus render_parser_deinterlace(zzDeinterlaceParamST *pDif, zz_char *szParams);
zzStatus render_parser_sharpness(zzSharpnessParamST *pSharp, zz_char *pszParams);
zzStatus render_parser_amp(zzAmpParamST *pAmp, zz_char *pszParams);
zzStatus render_parser_denoise(zzDenoiseParamST  *pDN, zz_char *pszParams);
zzStatus render_parser_comp(zzCompParamST  *pComp, zz_char *pszParams);


#endif /* INCLUDED_ZZ_RENDER_PARSER_H */
