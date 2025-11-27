#!/bin/bash

run=$1

if [ $run == "ser_exec" ]; then
    ./ser
elif [ $run == "cli_exec" ]; then
    ./cli
elif [ $run == "ser_comp" ]; then
    gcc -o ser p3-serUEB.c p3-aUEBs.* p3-tTCP.*
elif [ $run == "cli_comp" ]; then
    gcc -o cli p3-cliUEB.c p3-aUEBc.* p3-tTCP.* p3-aDNSc.*
elif [ $run == "clean" ]; then
    rm ser cli
fi
