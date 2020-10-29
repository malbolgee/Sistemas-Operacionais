#!/bin/bash

# Pega o workload da linha de comando;
# Para cada workload, existem 4 gráficos;
for ((j = 0; j < 4; ++j));
    do
        python3 plota.py $j ./results/w$1bft ./results/w$1fst ./results/w$1nft
    done;