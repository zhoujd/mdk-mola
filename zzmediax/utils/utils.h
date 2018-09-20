/* utils.h --- 
 */

#ifndef ZZUTILS_H
#define ZZUTILS_H

#include "cmn.h"
#include <stddef.h>
#include <stdarg.h>

void*      ZZAllocAndZeroMem(size_t size);
zzStatus   ZZInfoPrint(zz_char*  pStrInfoFile, zz_char* format, ...);

#endif //ZZUTILS_H


	
