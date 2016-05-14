
Environment
----------------

1. Microsoft Windows 10 (64bit)
2. Visual Studio Community 2015 Update 2
3. Most command run in "VS2015 x64 Native Tool Command Prompt" in ADMINISTRATOR mode
   (Do not use the OS default "Command Prompt")
4. Some command run in BASH shell on Cygwin

Choose any directory for working directory
```
(in VS2015 x64 Native Tool Command Prompt)
set WORKDIR=<Your working directory>

(in BASH shell)
export WORKDIR=<Your working directory>
```

Install Packages
----------------------
```
Install cmake-3.5.2
  1) Download from https://cmake.org/download/
  2) set PATH=%PATH%;C:\Program Files (x86)\CMake\bin

Install Python 2.7.11
  1) Download from https://www.python.org/downloads/
  2) set PATH=%PATH%;C:\Python27

Install libxml2-2.9.3
  1) Download
	http://xmlsoft.org/sources/win32/64bit/libxml2-2.9.3-win32-x86_64.7z
  2) Extract and check directory structure 
        %WORKDIR%\libxml2\include\libxml2\libxml
		%WORKDIR%\libxml2\lib

Install pkg-config_0.26-1
  1) Download 3 files
    http://ftp.gnome.org/pub/gnome/binaries/win32/dependencies/pkg-config_0.26-1_win32.zip
    http://ftp.gnome.org/pub/gnome/binaries/win32/dependencies/gettext-runtime_0.18.1.1-2_win32.zip
    http://ftp.gnome.org/pub/gnome/binaries/win32/glib/2.28/glib_2.28.8-1_win32.zip
  2) Extract and check directory structure 
		C:\pkg-config\bin\pkg-config.exe
		C:\pkg-config\bin\libglib-2.0-0.dll
		C:\pkg-config\bin\intl.dll
  (referenced: http://stackoverflow.com/questions/1710922/how-to-install-pkg-config-in-windows)

Install icu4c-56
  1) Download
    http://site.icu-project.org/download/56#TOC-ICU4C-Download
	choose icu4c-56_1-Win64-msvc10.zip
  2) Extract and check directory structure 
	  %WORKDIR%\icu\bin64
	  %WORKDIR%\icu\include
	  %WORKDIR%\icu\lib64
  
Install Ninja
  1) Download
    https://github.com/ninja-build/ninja/releases
	Choose ninja-win.zip
  2) Make sure your PATH include the installed directory
```

Download sources
----------------
```
  git clone https://github.com/tinysun212/swift-windows.git swift
  git clone https://github.com/tinysun212/swift-llvm-cygwin.git llvm
  git clone https://github.com/tinysun212/swift-clang-cygwin.git clang
  git clone https://github.com/apple/swift-cmark.git cmark

  cd swift; git checkout swift-msvc-20160515 ; cd ..
  cd llvm; git checkout swift-msvc-20160515 ; cd ..
  cd clang; git checkout swift-msvc-20160515 ; cd ..
  cd cmark; git checkout 6873b; cd ..
```

More Install
------------
These installations are for interim workarounds and will be removed in future.
```
Install utils.zip (Python invoker)
  1) Download
    https://github.com/tinysun212/swift-windows/releases/tag/swift-msvc-20160418
	Choose utils.zip
  2) Extract and check directory structure 
     %WORKDIR%\swift\utils\gyb.exe
     %WORKDIR%\swift\utils\line-directive.exe
     (source is included - %WORKDIR%/swift/misc/gyb.cpp)

Prepare Cygwin Tool and Swift Cygwin port.
  - You will need BASH shell to run sed.
  - You will need dlltool.exe in Cygwin.
  - You will need Cygwin version swiftc.exe.
  (*) swiftc.exe for MSVC will be built, but we should use Cygwin version to build the standard library.
	 
Prepare Favorate Hexa Editor.
  - HxD will be good if you don't have other. ( https://mh-nexus.de/en/hxd )
```

Patch cmake
-----------
Backup and overwrite ```Windows-Clang.cmake```
```
Copy %WORKDIR%/swift/misc/Windows-Clang.cmake in repository
  to C:\Program Files (x86)\CMake\share\cmake-3.5\Modules\Platform
```

Build cmark
-----------
```
mkdir %WORKDIR%\build\NinjaMSVC\cmark
cd %WORKDIR%\build\NinjaMSVC\cmark
cmake -G "Visual Studio 14 2015 Win64" -D CMAKE_BUILD_TYPE=RELEASE ..\..\..\cmark
  (You may ignore the following messages)
    -- Could NOT find PythonInterp: Found unsuitable version "2.7.11", but required is at least "3" (found C:/Python27/python.exe)
    *** A python 3 interpreter is required to run the spec tests.
    
"c:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" /p:Configuration=Release ALL_BUILD.vcxproj 
```

Build clang
-----------
```
// You should run "VS2015 x64 Native Tool Command Prompt" in ADMINISTRATOR mode
// to run mklink.exe.

cd %WORKDIR%\llvm\tools
mklink /d clang ..\..\clang

mkdir %WORKDIR%\build\NinjaMSVC\llvm
cd %WORKDIR%\build\NinjaMSVC\llvm
cmake -G "Visual Studio 14 2015 Win64" -D CMAKE_BUILD_TYPE=RELEASE ..\..\..\llvm

"c:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe" /p:Configuration=Release ALL_BUILD.vcxproj 
```

Build Swift
-----------
```
// You already set environment variables WORKDIR and PATH
// and you will use the commands - cmake, python, ninja, llvm tools here.

set PATH=%WORKDIR%\build\NinjaMSVC\llvm\release\bin;%PATH%

mkdir %WORKDIR%\build\NinjaMSVC\swift\bin
Following DLL's must be copied to %WORKDIR%/build/NinjaMSVC/swift/bin
  cmark.dll  (can be found at %WORKDIR%\build\NinjaMSVC\cmark\src\Release)
  icudt56.dll  (can be found at %WORKDIR%\icu\bin64)
  icuin56.dll
  icuuc56.dll

cd %WORKDIR%\build\NinjaMSVC\swift

cmake -G Ninja ..\..\..\swift -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang  -DCMAKE_CXX_COMPILER=clang -DLIBXML2_LIBRARIES=%WORKDIR%\libxml2\lib\libxml2.a  -DLIBXML2_INCLUDE_DIR=%WORKDIR%\libxml2\include\libxml2  -DPKG_CONFIG_EXECUTABLE=c:\pkg-config\bin\pkg-config.exe -DICU_UC_INCLUDE_DIR=%WORKDIR%\icu\include -DICU_UC_LIBRARY=%WORKDIR%\icu\lib64\icuuc.lib -DICU_I18N_INCLUDE_DIR=%WORKDIR%\icu\include -DICU_I18N_LIBRARY=%WORKDIR%\icu\lib64\icuin.lib -DSWIFT_INCLUDE_DOCS=FALSE -DSWIFT_PATH_TO_CMARK_BUILD=%WORKDIR%\build\NinjaMSVC\cmark -DSWIFT_PATH_TO_CMARK_SOURCE=%WORKDIR%\cmark  -DCMAKE_CXX_FLAGS="-fms-extensions -fms-compatibility-version=19 -frtti " ..\..\..\swift

(In Cygwin64 Terminal)
// change to the same directory
//   export WORKDIR=<Your working directory>
//   cd $WORKDIR/build/NinjaMSVC/swift
sed	-e 's;libclang\([^.]*\).a;clang\1.lib;g' \
	-e 's;swift\\libcmark.a;build\\NinjaMSVC\\cmark\\src\\Release\\cmark.lib;g' \
	-e 's;swift swiftc;swift.exe swiftc.exe;' \
	-e 's;swift swift-autolink-extract;swift.exe swift-autolink-extract.exe;' \
	-e 's;-fno-rtti ;;' \
	-e 's;DEFINES = -DGTEST_HAS_RTTI;DEFINES = -D_MT -D_DLL -DGTEST_HAS_RTTI;' \
	-e 's;-Wl,--allow-multiple-definition;-Wl,/FORCE:MULTIPLE;' \
    -e 's;LINK_FLAGS = -target;LINK_FLAGS = -Wl,msvcrt.lib -target;' \
	-e 's;LINK_PATH = -LC:;LINK_PATH = -Wl,/LIBPATH:C$:;' \
    -e 's;-ledit ;;g' \
	build.ninja > tt; mv tt build.ninja

Run
  cd %WORKDIR%\build\NinjaMSVC\swift
  // Building swift.exe, libswiftRuntime.a, libswiftStdlibStubs.a
  ninja bin\swift.exe lib\swift\windows\x86_64\libswiftRuntime.a lib\swift\windows\x86_64\libswiftStdlibStubs.a

  // Swift.obj - it will faied after generate %WORKDIR%/build/NinjaMSVC/swift/stdlib/public/core/8/*.swift
  // That's OK. We will make the Swift.obj in next section with these 8/*.swift.
  ninja stdlib\public\core\windows\x86_64\Swift.obj
```

Compile Swift.obj
-----------------
```
**********************************************************************************
*** CAUTION: Due to a bug, you should use the swiftc.exe recent built on Cygwin 
***          with working files in %WORKDIR%/swift.
**********************************************************************************

(In Cygwin64 Terminal)
export WORKDIR=<Your Windows (MSVC) build working directory>
export CYGWIN_SWIFTC_DIR=<Your Cygwin build working directory>/build/Ninja-ReleaseAssert/swift-cygwin-x86_64/bin
// Your Cygwin build working directory can be different from the WORKDIR used in Windows (MSVC) build.
// Check if $CYGWIN_SWIFTC_DIR directory has the Cygwin version of swiftc.exe.
export CORE8DIR=$WORKDIR/build/NinjaMSVC/swift/stdlib/public/core/8

cd $WORKDIR/swift/stdlib/public/core

(FORCE_DLLIMPORT=0; $CYGWIN_SWIFTC_DIR/swiftc -c -sdk / -target x86_64-pc-windows-msvc -O -I $WORKDIR/build/NinjaMSVC/swift/lib/swift/windows/x86_64 -module-cache-path $WORKDIR/build/NinjaMSVC/swift/module-cache -no-link-objc-runtime -Xfrontend -enable-reflection-metadata -nostdimport -parse-stdlib -module-name Swift -Xfrontend -enable-reflection-builtins -Xfrontend -group-info-path -Xfrontend GroupInfo.json -Xfrontend -sil-serialize-all -module-link-name swiftCore -force-single-frontend-invocation -parse-as-library -emit-module -emit-module-path $WORKDIR/build/NinjaMSVC/swift/lib/swift/windows/x86_64/Swift.swiftmodule -o $WORKDIR/build/NinjaMSVC/swift/stdlib/public/core/windows/x86_64/Swift.obj Algorithm.swift ArrayBody.swift ArrayBuffer.swift ArrayBufferProtocol.swift ArrayCast.swift $CORE8DIR/Arrays.swift ArrayType.swift Assert.swift AssertCommon.swift BidirectionalCollection.swift Bool.swift Boolean.swift BridgeObjectiveC.swift BridgeStorage.swift Builtin.swift $CORE8DIR/BuiltinMath.swift Character.swift CocoaArray.swift Collection.swift $CORE8DIR/CollectionAlgorithms.swift CompilerProtocols.swift ClosedRange.swift ContiguousArrayBuffer.swift CString.swift CTypes.swift EmptyCollection.swift ErrorType.swift Existential.swift $CORE8DIR/Filter.swift $CORE8DIR/FixedPoint.swift FlatMap.swift $CORE8DIR/Flatten.swift $CORE8DIR/FloatingPoint.swift $CORE8DIR/FloatingPointOperations.swift $CORE8DIR/FloatingPointParsing.swift $CORE8DIR/HashedCollections.swift Hashing.swift HeapBuffer.swift ImplicitlyUnwrappedOptional.swift Index.swift $CORE8DIR/Indices.swift InputStream.swift $CORE8DIR/IntegerArithmetic.swift $CORE8DIR/IntegerParsing.swift Join.swift $CORE8DIR/LazyCollection.swift LazySequence.swift LifetimeManager.swift ManagedBuffer.swift $CORE8DIR/Map.swift $CORE8DIR/Mirrors.swift Misc.swift MutableCollection.swift NewtypeWrapper.swift ObjCMirrors.swift Optional.swift OptionSet.swift OutputStream.swift Pointer.swift Policy.swift Print.swift RandomAccessCollection.swift $CORE8DIR/Range.swift $CORE8DIR/RangeReplaceableCollection.swift Reflection.swift Repeat.swift REPL.swift Reverse.swift $CORE8DIR/Runtime.swift Sequence.swift $CORE8DIR/SequenceAlgorithms.swift SequenceWrapper.swift SetAlgebra.swift ShadowProtocols.swift Shims.swift $CORE8DIR/Slice.swift $CORE8DIR/Sort.swift StaticString.swift $CORE8DIR/Stride.swift StringCharacterView.swift String.swift StringBridge.swift StringBuffer.swift StringCore.swift $CORE8DIR/StringInterpolation.swift StringLegacy.swift $CORE8DIR/StringRangeReplaceableCollection.swift StringIndexConversions.swift StringUnicodeScalarView.swift StringUTF16.swift StringUTF8.swift SwiftNativeNSArray.swift $CORE8DIR/UnavailableStringAPIs.swift Unicode.swift UnicodeScalar.swift $CORE8DIR/UnicodeTrie.swift Unmanaged.swift $CORE8DIR/UnsafeBufferPointer.swift $CORE8DIR/UnsafePointer.swift WriteBackMutableSlice.swift Availability.swift CollectionOfOne.swift $CORE8DIR/ExistentialCollection.swift Mirror.swift Process.swift SliceBuffer.swift $CORE8DIR/Tuple.swift VarArgs.swift Zip.swift )
```

Build swiftCore (shared)
------------------------
```
**********************************************************************************
*** CAUTION: To build Swift.obj, should use swiftc.exe built on Cygwin
**********************************************************************************

cd %WORKDIR%/build/NinjaMSVC/swift/stdlib/public/core

link /ERRORREPORT:PROMPT /OUT:"%WORKDIR%\build\NinjaMSVC\swift\lib\swift\windows\libswiftCore.dll" /INCREMENTAL:NO /NOLOGO /LIBPATH:%WORKDIR%/build/NinjaMSVC/llvm/Release/lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib ..\..\..\lib\swift\windows\x86_64\libswiftRuntime.a ..\..\..\lib\swift\windows\x86_64\libswiftStdlibStubs.a %WORKDIR%\icu\lib64\icuuc.lib %WORKDIR%\icu\lib64\icuin.lib /IMPLIB:"%WORKDIR%\build\NinjaMSVC\swift\lib\swift\windows\libswiftCore.lib" /DLL %WORKDIR%\build\NinjaMSVC\swift\stdlib\public\core\windows\x86_64\Swift.obj /DEF:allexp.def /MERGE:.rdata=.rodata /IGNORE:4102,4197 msvcrt.lib
```

Build swiftSwiftOnoneSupport (shared)
-------------------------------------
```
**********************************************************************************
*** CAUTION: To build SwiftOnoneSupport.obj, should use swiftc.exe built on MSVC
***          To use with static link, rename .pdata to .qdata with hexa editor
**********************************************************************************

cd %WORKDIR%/build/NinjaMSVC/swift/bin

swiftc -emit-ir -sdk / -target x86_64-pc-windows-msvc -O -I %WORKDIR%/build/NinjaMSVC/swift/lib/swift/windows/x86_64 -module-cache-path %WORKDIR%/build/NinjaMSVC/swift/clang-module-cache -no-link-objc-runtime -Xfrontend -sil-serialize-all -parse-stdlib -module-link-name swiftSwiftOnoneSupport -force-single-frontend-invocation -parse-as-library -emit-module -emit-module-path %WORKDIR%/build/NinjaMSVC/swift/lib/swift/windows/x86_64/SwiftOnoneSupport.swiftmodule -o SwiftOnoneSupport.ll %WORKDIR%/swift/stdlib/public/SwiftOnoneSupport/SwiftOnoneSupport.swift

python %WORKDIR%/swift/misc/inject_dllimport.py SwiftOnoneSupport.ll SwiftOnoneSupport.new.ll

clang -c SwiftOnoneSupport.new.ll -o %WORKDIR%\build\NinjaMSVC\swift\stdlib\public\SwiftOnoneSupport\windows\x86_64\SwiftOnoneSupport.obj -mcmodel=large -target x86_64-pc-windows-msvc19.0.0

cd %WORKDIR%/build/NinjaMSVC/swift/stdlib/public/SwiftOnoneSupport

link /ERRORREPORT:PROMPT /OUT:"%WORKDIR%\build\NinjaMSVC\swift\lib\swift\windows\libswiftSwiftOnoneSupport.dll" /INCREMENTAL:NO /NOLOGO /LIBPATH:%WORKDIR%/build/NinjaMSVC/llvm/Release/lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib ..\..\..\lib\swift\windows\libswiftCore.lib /MANIFEST /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /manifest:embed /PDB:"%WORKDIR%/build/NinjaMSVC/swift/bin/libswiftSwiftOnoneSupport.pdb" /SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT /IMPLIB:"%WORKDIR%/build/NinjaMSVC/swift/lib/swift/windows/libswiftSwiftOnoneSupport.lib" /MACHINE:X64 /DLL %WORKDIR%\build\NinjaMSVC\swift\stdlib\public\SwiftOnoneSupport\windows\x86_64\SwiftOnoneSupport.obj    /DEF:allexp.def msvcrt.lib /MERGE:.rdata=.rodata /IGNORE:4197
```

Run with Interpreter
--------------------
```
cd %WORKDIR%/build/NinjaMSVC/swift/bin

Create a sample source
  echo print("Hello") > Hello.swift

Run
  swift Hello.swift
```

Compile & Run with DLL
----------------------
```
cd %WORKDIR%/build/NinjaMSVC/swift/bin

Compile
  swiftc Hello.swift

Run
  set PATH=%PATH%;%WORKDIR%/build/NinjaMSVC/swift/lib/swift/windows
  Hello.exe
```

Build Static libraries
----------------------
```
cd %WORKDIR%/build/NinjaMSVC/swift
mkdir lib\swift_static\windows

lib /out:lib\swift_static\windows\libswiftCore.lib  lib\swift\windows\x86_64\libswiftRuntime.a  lib\swift\windows\x86_64\libswiftStdlibStubs.a stdlib/public/core/windows/x86_64/Swift.obj %WORKDIR%\icu\lib64\icuuc.lib %WORKDIR%\icu\lib64\icuin.lib /IGNORE:4006,4221

lib /out:lib\swift_static\windows\libswiftSwiftOnoneSupport.lib stdlib\public\SwiftOnoneSupport\windows\x86_64\SwiftOnoneSupport.obj

Edit libswiftCore.lib with hexa editor
rename all ".pdata" to ".qdata" (multiple occurrence)
```

Compile & Run with Static library
---------------------------------
```
cd %WORKDIR%/build/NinjaMSVC/swift/bin

Compile
  swiftc -static-stdlib Hello.swift

Run
  Hello.exe
```
