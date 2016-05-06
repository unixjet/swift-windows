/*-
* Copyright (c) 2016 Han Sangjin <tinysun.net@gmail.com>
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. The name of the author may not be used to endorse or promote
*    products derived from this software without specific prior written
*    permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

#if defined(_MSC_VER)

#ifndef WILL_INCLUDE_FCNTL

#define O_RDONLY     0x0000
#define O_WRONLY     0x0001
#define O_RDWR       0x0002
#define O_APPEND     0x0008
#define O_CREAT      0x0100
#define O_TRUNC      0x0200
#define O_EXCL       0x0400
#define O_TEXT       0x4000
#define O_BINARY     0x8000
#define O_RAW        0x8000
#define O_TEMPORARY  0x0040
#define O_NOINHERIT  0x0080
#define O_SEQUENTIAL 0x0020
#define O_RANDOM     0x0010

#endif

#if _DLL
#define DLLEXP __declspec(dllexport)
#else
#define DLLEXP
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLLEXP
void computil_cstr_replace_init();

DLLEXP
int computil_cstr_replace_add(const char *exp, const char *newstr);

DLLEXP
char *computil_cstr_replace(const char *in_str, long long in_len, long long *out_len);

DLLEXP
int computil_open(const char *filename, int oflag);

DLLEXP
long long computil_filelength(int fd);

DLLEXP
int computil_set_fmode(int mode);

DLLEXP
void computil_exit(int status);

DLLEXP
int mem_replace(const void *data, long long data_len, const void *from, size_t from_len,
				const void *to, size_t to_len, void **replaced_data, long long *replaced_len);

#ifdef __cplusplus
}
#endif

#endif

