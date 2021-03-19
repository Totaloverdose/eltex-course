#!/bin/bash

# Usage: .../test-starter.sh program1 program2 ... programN
# This program prints executon time of the programs with names from the
# arguments

i=1

printf "Time with the default optimization\n\n" > log.txt

while [ $i -le $# ]
do
    gcc ${!i}.c -o ${!i}
        
    printf "Time for the program ${!i}:\n" >> log.txt
    /usr/bin/time -pao log.txt ./${!i}
    printf "\n\n" >> log.txt
    i=`expr $i + 1`
done

printf "\n\n\n" >> log.txt

i=1

printf "Time without any optimization\n\n" >> log.txt

while [ $i -le $# ]
do
    gcc ${!i}.c -O0 -o ${!i}

    printf "Time for the program ${!i}:\n" >> log.txt
    /usr/bin/time -pao log.txt ./${!i}
    printf "\n\n" >> log.txt
    i=`expr $i + 1`
done

