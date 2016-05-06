@echo off

set LIBDIR=%~dp0/../lib/swift/windows

echo Compiling %1

swiftc -emit-ir %1 -o %~n1.orig.ll -O -target x86_64-pc-windows-msvc19.0.0

swift "%~dp0/inject_dllimport.swift" %~n1.orig.ll %~n1.ll

clang -o %~n1.exe %~n1.ll -llibswiftCore -llibswiftSwiftOnoneSupport -Wl,/LIBPATH:"%LIBDIR%",/MERGE:.rdata=.rodata,/FORCE:MULTIPLE,/NODEFAULTLIB:libcmt,msvcrt.lib -Xlinker /IGNORE:4006,4049,4217 -mcmodel=large -target x86_64-pc-windows-msvc19.0.0

del %~n1.ll %~n1.orig.ll %~n1.obj
