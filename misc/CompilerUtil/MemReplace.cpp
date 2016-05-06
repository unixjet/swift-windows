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

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

extern "C"
void *
memmem(const void *l, size_t l_len, const void *s, size_t s_len);

extern "C"
int mem_replace(const void *data, long long data_len, const void *from, size_t from_len,
				const void *to, size_t to_len, void **replaced_data, long long *replaced_len) {
	char *search_from = (char *)data;
	long long length_remained;
	char *new_data = (char *)malloc(data_len);
	long long new_len = data_len;
	size_t copy_to = 0;
	long long last_allocated_size = data_len;
	while (1) {
		length_remained = data_len - (int)(search_from - (char *)data);
		char *found = (char *)memmem(search_from, length_remained, from, from_len);
		if (!found)
			break;
//printf("Found at search_from=%p, length_remained=%lld, from=%p, from_len=%lld, found=%p\n",
//	   search_from, length_remained, from, from_len, found);
		new_len += to_len - from_len;
		if (last_allocated_size < new_len) {

			new_data = (char *)realloc(new_data, new_len);
			last_allocated_size = new_len;
		}
		memcpy(new_data + copy_to, search_from, found - search_from);
		copy_to += (char *)found - (char *)search_from;
		memcpy(new_data + copy_to, to, to_len);
		copy_to += to_len;
		search_from = found + from_len;
	}
	memcpy(new_data + copy_to, search_from, length_remained);
	*replaced_data = new_data;
	*replaced_len = new_len;
	return 0;
}
