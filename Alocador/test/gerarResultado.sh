#!/bin/bash

# Gera todas as saídas
for f in $(ls -v ./workloads/); 
    do
        echo Gerando saída para o workload $f;
        ../src/main < ./workloads/$f > ./results/$f;
    done;