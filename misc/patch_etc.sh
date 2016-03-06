#!/bin/sh

for SWIFT_PROJ in \
  tools/swift-llvm-opt/swift-llvm-opt.vcxproj \
  tools/sil-opt/sil-opt.vcxproj \
  tools/sil-extract/sil-extract.vcxproj \
  tools/lldb-moduleimport-test/lldb-moduleimport-test.vcxproj
do
  echo "Patching $SWIFT_PROJ"
  sed \
    -e 's;libclang\([^.]*\).a;clang\1.lib;g' \
	-e 's;swift\\libcmark.a;build\\firsttime\\cmark\\src\\Release\\cmark.lib;g' \
	$SWIFT_PROJ > tmp.out

  mv tmp.out $SWIFT_PROJ
done
