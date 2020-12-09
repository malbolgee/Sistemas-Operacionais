#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/utils.h"
#include "../include/lista.h"
#include "../include/eclnr.h"
#include "../include/process.h"

process_t *createProcess(const char *pnome, const unsigned pio, const size_t ptam,
                         const unsigned pprio, const unsigned ptempo)
{

    process_t *process = (process_t *)calloc(1, sizeof(process_t));
    alloc_check(process, "Falha ao alocar processo");

    strcpy(process->nome, pnome);
    process->io = pio;
    process->tam = ptam;
    process->prio = pprio;
    process->chegada = ptempo;

    return process;
}

unsigned run(process_t *process, unsigned quantum)
{

    if ((unsigned)((rand() % 100) + 1) < process->io)
    {
        process->tam -= 1;
        process->done_io = true;
        printf("%s fez e/s, falta %ld\n", process->nome, process->tam);
        return 1;
    }

    if (quantum == 0 || process->tam < quantum)
        quantum = process->tam;

    process->done_io = false;
    process->tam -= quantum;
    printf("rodou por: %d timeslice, faltam: %ld\n", quantum, process->tam);
    return quantum;
}