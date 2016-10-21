#!/bin/bash

NAME=$1
ARG=$2
#0 1 0 0 1 0 0 0 0 0 0 1 0 0 1 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0 1 0 0 0
echo $1
for x in $(seq 2);
do x=$(($x-1));
echo -e "\n RUNNING CASE "$x"\n";
echo -e "\nCompiling "$NAME".dfs_count...\n";
make HIST=$x ./$NAME".dfs_count";
echo -e "\nGenerating datafile for "$NAME".dfs_count...\n";
rm file;
echo "12" > file"-"$x;
echo $2  >> file"-"$x;
echo -e "\nExecuting case "$NAME".dfs_count...\n";
(./$NAME".dfs_count" < file-$x) > $1"-"$x"."salida;
done;
cat $1-*.salida > $1.salida;