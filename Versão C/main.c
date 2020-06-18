#include "lista.h"
#include "eclnr.h"
#include "process.h"
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

size_t sum(size_t *array, size_t size)
{

    size_t soma = 0;
    for (size_t i = 0; i < size; ++i)
        soma += array[i];

    return soma;

}

int main(int argc, char **argv)
{

    SJF_t fifo;
    lista_t procs;
    char *nomes[] = {"A", "B", "C"};
    unsigned chance_io[] = {0, 0, 0};
    size_t tamanho[] = {100, 10, 10};
    unsigned nprocs = 3;

    unsigned total = sum(tamanho, sizeof(tamanho) / sizeof(tamanho[0]));
    lista_init(&procs);

    for (size_t i = 0; i < nprocs; ++i)
        lista_push(&procs, createProcess(nomes[i], chance_io[i], tamanho[i], 0, 0));

    SJF_init(&fifo, &procs);

    int tempo = 0;
    int t_wait = 0;
    int t_exec = 0;
    int t_exec_m = 0;
    while (total > 0)
    {

        process_t p = SJF_proximo(&fifo);

        printf("Valor de total: %d\n", total);
        printf("Valor de p.tam: %ld\n", p.tam);

        sleep(1);

        if (p.tam != 0)
        {

            unsigned rodou = run(&p, 0);

            if (p.tam > 0)
                SJF_pronto(&fifo, p);
            
            t_wait += tempo + p.chegada; 

            total -= rodou;
            tempo += rodou;

            t_exec += (rodou - p.chegada);
            t_exec_m += t_exec;

        }
        else
            tempo += 1;

    }

    printf("Tempo de Execução: %d, Tempo de Resposta: %d\n", t_exec_m / nprocs, t_wait / nprocs);

    return 0;
}