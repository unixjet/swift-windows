#!/bin/sh

echo  "sed  \\" > tmp.sh
sed -e 's;\(.*\);-e "s/@\1/@__imp_\1/g" \\;' $1 >> tmp.sh
echo  " $2 " >> tmp.sh

sh tmp.sh > $3
