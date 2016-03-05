#!/bin/sh

for f in $(find . -name "*.vcxproj")
do 
  grep 'w14062 /Zc:inline' $f > /dev/null
  if [ $? -eq 0 ]
  then
    echo "Exist in $f"
    sed -e 's; -w14062 /Zc:inline /EHs-c- ;;' -e 's; -target x86_64-pc-windows-msvc -isysroot -L; -L;' $f  > tmp.out
    mv tmp.out $f
    echo $f "changed"
  fi
  grep '_HAS_EXCEPTIONS=0' $f > /dev/null
  if [ $? -eq 0 ]
  then
    sed -e 's/_HAS_EXCEPTIONS=0;//g' $f  > tmp.out
    mv tmp.out $f
  fi
  grep 'WIN32;' $f > /dev/null
  if [ $? -eq 0 ]
  then
    sed -e 's/WIN32;/__STDC__=0;WIN32;/' $f  > tmp.out
    mv tmp.out $f
  fi
  grep 'target x86_64' $f > /dev/null
  if [ $? -eq 0 ]
  then
    sed -e 's/>-target/>-Wno-macro-redefined -target/' $f  > tmp.out
    mv tmp.out $f
  fi
done
