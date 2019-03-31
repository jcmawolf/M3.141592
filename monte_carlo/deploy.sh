#!/bin/bash
rm secuencial > /dev/null 2>/dev/null
g++ secuencial.cpp -o secuencial
./secuencial
rm distribuida_paralela > /dev/null 2>/dev/null
mpic++ distribuida_paralela.cpp -o distribuida_paralela
./distribuida_paralela