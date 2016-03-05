#!/bin/sh

for f in $(find . -name "*.vcxproj")
do 
  grep 'w14062 /Zc:inline' $f > /dev/null
  if [ $? -eq 0 ]
  then
    echo "Exist in $f"
    sed -e 's; -w14062 /Zc:inline /EHs-c- ;;' -e 's; -target x86_64-pc-windows-msvc -isysroot -L; -L;' $f > tmp.out
    mv tmp.out $f
    echo $f "changed"
  fi
done
