#!/bin/bash

#Gera todas as saídas 
for f in $(ls -v ./workloads/); 
    do
        ../src/main < ./workloads/$f > ./results/$f;
    done;