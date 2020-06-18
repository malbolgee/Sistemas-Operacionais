#include "lista.h"
#include "eclnr.h"
#include "process.h"
#include <errno.h>
#include <err.h>
#include <unistd.h>
#include <error.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

process_t createProcess(const char *pnome, const unsigned pio, const size_t ptam, const unsigned pprio, const unsigned ptempo)
{

    process_t process = {0};
    strcpy(process.nome, pnome);
    process.io = pio;
    process.tam = ptam;
    process.prio = pprio;
    process.chegada = ptempo;
    return process;
}

unsigned run(process_t *process, unsigned quantum)
{

    srand(time(NULL));
    if (((rand() % 100) + 1) < process->io)
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