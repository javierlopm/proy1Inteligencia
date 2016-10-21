#!/bin/bash

NAME=$1
ARG=$2
# 16 15 14 13 12 11 10 9 8 7 6 5 4 1 0 17 18 19 20 21 22 23 24 25 3 2
# 6 5 4 0 7 8 9 10 11 12 21 20 19 18 17 16 15 14 13 1 2 3 22 23
echo $1
for x in $(seq 3);
#for x in $(echo 3);
do x=$(($x-1));
echo -e "\n RUNNING CASE "$x"\n";
echo -e "\nCompiling "$NAME".dfs_count...\n";
make HIST=$x ./$NAME".dfs_count";
echo -e "\nGenerating datafile for "$NAME".dfs_count...\n";
rm file;
echo "7" > file"-"$x;
echo $2  >> file"-"$x;
echo -e "\nExecuting case "$NAME".dfs_count...\n";
(./$NAME".dfs_count" < file-$x) > $1"-"$x"."salida;
done;
cat $1-*.salida > $1.salida;
