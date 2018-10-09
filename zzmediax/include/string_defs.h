/* string_defs.h
 *
 */

#ifndef STRING_DEFS_H
#define STRING_DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#ifdef __cplusplus
#include <string>
#endif

#if defined(_WIN32) || defined(_WIN64)

#include <tchar.h>

#define ZZ_STRING(x) _T(x)
#define ZZ_CHAR(x) _T(x)

#ifdef __cplusplus
typedef std::basic_string<TCHAR> zz_tstring;
#endif
typedef TCHAR zz_char;

#define zz_printf   _tprintf
#define zz_fprintf  _ftprintf
#define zz_sprintf  _stprintf_s // to be removed
#define zz_vprintf  _vtprintf
#define zz_strlen   _tcslen
#define zz_strcmp   _tcscmp
#define zz_strncmp  _tcsnicmp
#define zz_strstr   _tcsstr
#define zz_atoi     _ttoi
#define zz_strtol   _tcstol
#define zz_strtod   _tcstod
#define zz_strchr   _tcschr
#define zz_itoa_decimal(value, str)   _itow_s(value, str, 4, 10)

#define zz_strcat    _tcscat_s
#define zz_strncat   _tcsncat_s

// zz_strcopy is intended to be used with 2 parmeters, i.e. zz_strcopy(dst, src)
// for _tcscpy_s that's possible if DST is declared as: TCHAR DST[n];
#define zz_strcopy    _tcscpy_s
#define zz_strncopy   _tcsncpy_s

#define ZZ_MEMCPY_BITSTREAM(bitstream, offset, src, count) memcpy_s((bitstream).Data + (offset), (bitstream).MaxLength - (offset), (src), (count))
#define ZZ_MEMCPY_BUF(bufptr, offset, maxsize, src, count) memcpy_s((bufptr)+ (offset), (maxsize) - (offset), (src), (count))
#define ZZ_MEMCPY_VAR(dstVarName, src, count) memcpy_s(&(dstVarName), sizeof(dstVarName), (src), (count))
#define ZZ_MEMCPY(dst, src, count) memcpy_s(dst, (count), (src), (count))


#else // #if defined(_WIN32) || defined(_WIN64)

#define ZZ_STRING(x)   x
#define ZZ_CHAR(x)     x

#ifdef __cplusplus
typedef std::string zz_tstring;
#endif
typedef char zz_char;

#define zz_printf   printf
#define zz_sprintf  sprintf
#define zz_vprintf  vprintf
#define zz_fprintf  fprintf
#define zz_strlen   strlen
#define zz_strcmp   strcmp
#define zz_strncmp  strncmp
#define zz_strstr   strstr
#define zz_atoi     atoi
#define zz_atoll    atoll
#define zz_strtol   strtol
#define zz_strtod   strtod
#define zz_itoa_decimal(value, str) snprintf(str, sizeof(str)/sizeof(str[0])-1, "%d", value)

#define zz_strcat(dest, len, str)               strcat(dest, str)

#define zz_strcpy       strcpy
#define zz_strncpy      strncpy
#define zz_sscanf       sscanf

#define zz_strcopy(dest, len, str)              strcpy(dest, str)
#define zz_strncopy(dst, num_dst, src, count)   strncpy(dst, src, count)

#define ZZ_MEMCPY_BITSTREAM(bitstream, offset, src, count) memcpy((bitstream).Data + (offset), (src), (count))
#define ZZ_MEMCPY_BUF(bufptr, offset, maxsize, src, count) memcpy((bufptr)+ (offset), (src), (count))
#define ZZ_MEMCPY_VAR(dstVarName, src, count) memcpy(&(dstVarName), (src), (count))
#define ZZ_MEMCPY(dst, src, count) memcpy(dst, (src), (count))

#endif // #if defined(_WIN32) || defined(_WIN64)

#endif //STRING_DEFS_H
