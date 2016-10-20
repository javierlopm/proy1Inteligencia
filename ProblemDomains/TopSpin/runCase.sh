#!/bin/bash

NAME=$1
ARG=$2
#1 4 2 3 5 8 6 10 7 9 B 11
echo $1
for x in $(seq 3);
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