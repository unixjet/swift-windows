#!/bin/sh

export SWIFT_PROJ=tools/driver/swift.vcxproj

sed \
	-e 's;libclang\([^.]*\).a;clang\1.lib;g' \
	-e 's;swift\\libcmark.a;build\\firsttime\\cmark\\src\\Release\\cmark.lib;g' \
	-e 's;\;edit.lib;;g' \
  -e 's;swift swiftc;swift.exe swiftc.exe;' \
  -e 's;swift swift-autolink-extract;swift.exe swift-autolink-extract.exe;' \
	$SWIFT_PROJ > tmp.out

mv tmp.out $SWIFT_PROJ
