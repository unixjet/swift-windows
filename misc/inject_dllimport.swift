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

import SwiftShims
import CCompilerUtil

// Replace rules for llvm assmebler source
// Format is 'from string' : 'to string'
var replace_tbl: [String: String] = [
  "external global": "external dllimport global",
  "external constant": "external dllimport constant",
  "@_TWVBo = external dllimport": "@_TWVBo = external",
  "@_TWVBo": "@__imp__TWVBo",
  ]

///////////////////////////////////////////////////////
//
// Add replace entry
//
///////////////////////////////////////////////////////
computil_cstr_replace_init(); 
for (exp, newstr) in replace_tbl {
  computil_cstr_replace_add(exp, newstr)
}

/////////////////////////////////////////////////////
//
// Read all data from file arguments[1] to 'buffer'
//
/////////////////////////////////////////////////////
computil_set_fmode(O_BINARY)
var rfd = computil_open(Process.arguments[1], O_RDONLY)
if rfd == -1 {
  print("Error in opening read file")
  computil_exit(1) 
}
var len = computil_filelength(rfd)
var buffer = UnsafeMutablePointer<CChar>(allocatingCapacity:len)
var ret = _swift_stdlib_read(rfd, buffer, len)
_swift_stdlib_close(rfd)


/////////////////////////////////////////////////////
//
// Replace string
//
/////////////////////////////////////////////////////
var out_len : Int = 0
var changed_data = computil_cstr_replace(buffer, len, &out_len)


/////////////////////////////////////////////////////
//
// Write 'changed_data' to file arguments[2]
//
/////////////////////////////////////////////////////
var wfd = computil_open(Process.arguments[2], O_WRONLY | O_CREAT | O_TRUNC)
if wfd == -1 {
  print("Error in opening write file")
  computil_exit(1) 
}
ret = _swift_stdlib_write(wfd, changed_data, out_len)
_swift_stdlib_close(wfd)

// Free the allocated memory by computil_cstr_replace()
_swift_stdlib_free(changed_data)
