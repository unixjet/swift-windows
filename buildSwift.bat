mkdir ..\buildswift
cd ..\buildswift
cmake ..\swift -DCMAKE_BUILD_TYPE=Debug -DLLVM_CONFIG=..\buildllvm\Debug\bin\llvm-config -DSWIFT_INCLUDE_TESTS=FALSE -DSWIFT_INCLUDE_DOCS=FALSE
"C:\Program Files (x86)\MSBuild\14.0\Bin\Msbuild" tools\driver\swift.vcxproj