cd ..
move clang llvm/tools/
cd llvm
git apply ..\swift\llvm.patch
mkdir buildllvm
cd buildllvm
cmake ..\llvm
cd ..
mkdir buildcmark
cd buildcmark
cmake ..\cmark
"C:\Program Files (x86)\MSBuild\14.0\Bin\Msbuild" .\cmark.sln
copy ..\cmark\src\cmark.h ..\swift\include
copy ..\buildcmark\src\cmark_export.h ..\swift\include
copy ..\buildcmark\src\cmark_version.h ..\swift\include
copy ..\buildcmark\src\Debug\cmark.lib ..\swift\
copy /Y ..\buildcmark\src\Debug\cmark.dll c:\windows\ 
"C:\Program Files (x86)\MSBuild\14.0\Bin\Msbuild" ..\buildllvm\llvm.sln