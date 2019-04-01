#!/bin/bash
clear
rm build/secuencial > /dev/null 2>/dev/null
g++ secuencial.cpp -o build/secuencial
time build/secuencial $1 $2 $3
rm build/distribuida_paralela > /dev/null 2>/dev/null
mpic++ distribuida_paralela.cpp -o build/distribuida_paralela
time mpirun -np 4 build/distribuida_paralela $1 $2 $3
rm build/distribuida_paralela_mod > /dev/null 2>/dev/null
mpic++ distribuida_paralela_mod.cpp -o build/distribuida_paralela_mod
time mpirun -np 4 build/distribuida_paralela_mod $1 $2 $3