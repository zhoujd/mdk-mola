/* Last modified Time-stamp: <Jiandong zhou 2007-12-30 16:29:58>
 * @(#)va_ctx_mng.c
 */

#include "va_ctx_mng.h"

#define GetVaDisplay(p)    (p->va_dpy)
#define GetDisplay(p)      (p->display)


static zzStatus ZZVAContext_CreateVppCtx(VADisplay va_dpy, zzU16 width, zzU16 height, zzU32 *ctx_id);

zzStatus ZZVAContext_CreateVppCtx(VADisplay va_dpy, zzU16 width, zzU16 height, zzU32 *ctx_id)
{
    VAStatus   va_res  = VA_STATUS_SUCCESS;
    zzStatus   sts     = ZZ_ERR_NONE;

    int          entrynum   = 0;
    int          entrycount = 0;
    int          idx        = 0;
    int          support    = 0;

    VAEntrypoint         *entrypoints = NULL;
    VAConfigAttrib        attributes;
    VAConfigID            config_id;

    //check entrypoint
    entrycount = vaMaxNumEntrypoints(va_dpy);
    entrypoints = calloc(entrycount, sizeof(*entrypoints));
    if (!entrypoints)
    {
        ZZPRINTF("no VPP entry points allocated.\n");
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    va_res = vaQueryConfigEntrypoints(va_dpy,
                                      VAProfileNone,
                                      entrypoints,
                                      &entrynum);
    sts = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaQueryConfigEntrypoints error\n");
        goto END;
    }

    for (idx=0; idx < entrynum; idx++)
    {
        if (VAEntrypointVideoProc == entrypoints[idx])
        {
            support = 1;
            break;
        }
    }

    if (entrypoints)
    {
        free(entrypoints);
        entrypoints = NULL;
    }

    if (0 == support)
    {
        ZZPRINTF("config_vp: no VPP entry points supported.\n");
        sts = ZZ_ERR_UNSUPPORTED;
        goto END;
    }

    //configuration
    va_res = vaGetConfigAttributes(va_dpy,
                                   VAProfileNone,
                                   VAEntrypointVideoProc,
                                   &attributes,
                                   1);

    sts = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaGetConfigAttributes error\n");
        goto END;
    }

    va_res = vaCreateConfig(va_dpy,
                            VAProfileNone,
                            VAEntrypointVideoProc,
                            &attributes,
                            1,
                            &config_id);

    sts = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateConfig error\n");
        goto END;
    }

    //context
    va_res = vaCreateContext(va_dpy,
                             config_id,
                             width,
                             height,
                             VA_PROGRESSIVE,
                             0, 0,
                             ctx_id);

    sts = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateContext error\n");
        goto END;
    }

END:
    return sts;
}


zzStatus ZZVAContext_Create(zzVAContextST *pSelf)
{
    zzStatus sts     = ZZ_ERR_NONE;


    //init vaapi
    sts = ZZVA_InitVA(&pSelf->display, &pSelf->va_dpy);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("InitVA error\n");
        goto END;
    }

    //create vpp context
    pSelf->width  = ZZ_CTX_DEF_WIDTH;    //screen height
    pSelf->height = ZZ_CTX_DEF_HEIGHT;   //screen width

    sts = ZZVA_GetScreenInfo(&pSelf->width, &pSelf->height);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZVA_GetScreenInfo error\n");
        goto END;
    }

    sts = ZZVAContext_CreateVppCtx(pSelf->va_dpy, pSelf->width, pSelf->height, &pSelf->id);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("VAContext_Create error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZVAContext_Release(zzVAContextST *pSelf)
{
    VAStatus   va_res  = VA_STATUS_SUCCESS;
    zzStatus   sts     = ZZ_ERR_NONE;

    va_res = vaDestroyContext(pSelf->va_dpy, pSelf->id);
    sts = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaDestroyContext error\n");
        goto END;
    }

    pSelf->id = VA_INVALID_ID;

END:
    return sts;
}
