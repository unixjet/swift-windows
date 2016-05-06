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
#define WILL_INCLUDE_FCNTL
#include "StringUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <vector>

static std::vector<std::string> exp_list;
static std::vector<std::string> newstr_list;

extern "C"
void computil_cstr_replace_init()
{
	exp_list.clear();
	newstr_list.clear();
}

extern "C"
int computil_cstr_replace_add(const char *exp, const char *newstr)
{
	exp_list.push_back(exp);
	newstr_list.push_back(newstr);
	return 0;
}

extern "C"
char *computil_cstr_replace(const char *in_str, long long in_len, long long *out_len)
{
	void *replaced_data;
	long long replaced_len;
	const void *data = in_str;
	long long data_len = in_len;
	for (int i = 0; i < exp_list.size(); i++) {
		mem_replace(data, data_len, exp_list[i].c_str(), exp_list[i].length(),
					newstr_list[i].c_str(), newstr_list[i].length(), &replaced_data, &replaced_len);
		if (data != in_str)
			free((void *)data);
		data = (char *)replaced_data;
		data_len = replaced_len;
	}
	*out_len = data_len;
	return (char *)data;
}

extern "C"
int computil_open(const char *filename, int oflag)
{
	_O_RDONLY;
	return _open(filename, oflag, _S_IREAD | _S_IWRITE);
}

extern "C"
long long computil_filelength(int fd)
{
	return _filelengthi64(fd);
}

extern "C"
int computil_set_fmode(int mode)
{
	return _set_fmode(mode);
}

extern "C"
void computil_exit(int status)
{
	return exit(status);
}
