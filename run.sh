#!/bin/bash

run=$1

if [ $run == "s" ]; then
    ./ser
elif [ $run == "c" ]; then
    ./cli
elif [ $run == "compile" ]; then
    gcc -o ser p3-serUEB.c p3-aUEBs.* p3-tTCP.*
    gcc -o cli p3-cliUEB.c p3-aUEBc.* p3-tTCP.* p3-aDNSc.*
elif [ $run == "clean" ]; then
    rm ser cli
fi
