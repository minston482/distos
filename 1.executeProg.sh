#! /bin/bash
echo "" > $1.result
for (( i=1; i<=10; i++))
do
	echo "size of thread : $i start"
	echo "size of thread : $i" >> $1.result
	./$1 $i >> $1.result
	echo "size of thread : $i end"
done
