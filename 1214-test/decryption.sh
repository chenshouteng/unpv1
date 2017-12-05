#!/bin/bash

function recursive_decryption (){
for file in `ls $1`
do
	if [ -d $1"/"$file ]
        then
                recursive_decryption $1"/"$file
        else
		if [ "${file##*.}" = "c" -o "${file##*.}" = "java" -o "${file##*.}" = "cpp" ]
		then
			cat $1"/"$file > $1"/"$file".txt"
			mv $1"/"$file".txt" $1"/"$file
		fi	
	fi
done
}

echo $PWD
recursive_decryption $PWD
