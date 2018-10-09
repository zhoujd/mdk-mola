/* file_defs.h
 *
 */

#ifndef FILE_DEFS_H
#define FILE_DEFS_H

#include <stdio.h>

#  include <stdio.h>
#  include <stdarg.h>
#  include <sys/types.h>
#  include <sys/stat.h>


# define MAX_PATH          260
# define MAX_FILENAME_LEN  256

typedef FILE zz_file;


#if defined(_WIN32) || defined(_WIN64)

#define zz_file_fopen(file, name, mode) _tfopen_s(&file, name, mode)
#define zz_file_fclose      fclose
#define zz_file_fgets       _fgetts
#define zz_file_vfprintf    _vftprintf

#else // #if defined(_WIN32) || defined(_WIN64)

#include <unistd.h>

#define zz_stderr     stderr
#define zz_stdout     stdout
#define zz_stdin      stdin
/*
 * file access functions
 */
#define zz_file_fopen(file, name, mode) file = fopen(name, mode)
#define zz_file_fclose     fclose
#define zz_file_feof       feof
#define zz_file_remove     remove

/*
 * binary file IO */
#define zz_file_fread      fread
#define zz_file_fwrite     fwrite

/*
 * character (string) file IO */
#define zz_file_fgets      fgets
#define zz_file_fputs      fputs
#define zz_file_fscanf     fscanf
#define zz_file_fprintf    fprintf
#define zz_file_vfprintf   vfprintf

/* temporary file support */
#define zz_file_tmpfile      tmpfile
#define zz_file_tmpnam       tmpnam
#define zz_file_tmpnam_r     tmpnam_r
#define zz_file_tempnam      tempnam

#endif // #if defined(_WIN32) || defined(_WIN64)

#endif // #ifndef FILE_DEFS_H
