#!/bin/bash
# rm secuencial > /dev/null 2>/dev/null
# g++ secuencial.cpp -o secuencial
# time ./secuencial $1
# rm distribuida_paralela > /dev/null 2>/dev/null
# mpic++ distribuida_paralela.cpp -o distribuida_paralela
# time mpirun -np 4 distribuida_paralela $1
rm distribuida_paralela_mod > /dev/null 2>/dev/null
mpic++ distribuida_paralela_mod.cpp -o distribuida_paralela_mod
mpirun -np 4 distribuida_paralela_mod $1