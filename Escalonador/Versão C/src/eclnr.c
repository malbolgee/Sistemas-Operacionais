#include <stdlib.h>
#include <stdbool.h>
#include "../include/lista.h"
#include "../include/eclnr.h"
#include "../include/process.h"

void eclnrInit(FIFO_t *eclnr, lista_t *processes)
{

    eclnr->prontos = processes;
}

void eclnrPronto(FIFO_t *eclnr, process_t *process)
{

    lista_push(eclnr->prontos, process);
}

process_t *eclnrProximo(FIFO_t *eclnr)
{

    return lista_pop_front(eclnr->prontos);
}