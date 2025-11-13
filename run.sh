#!/bin/bash

run=$1

if [ $run == "s" ]; then
    ./ser
elif [ $run == "c" ]; then
    ./cli
elif [ $run == "compile" ]; then
    gcc -o ser p2-serUEB.c p2-aUEBs.* p2-tTCP.*
    gcc -o cli p2-cliUEB.c p2-aUEBc.* p2-tTCP.*
elif [ $run == "clean" ]; then
    rm ser cli
fi
