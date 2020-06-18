#include "lista.h"
#include "eclnr.h"
#include "process.h"
#include <errno.h>
#include <err.h>
#include <unistd.h>
#include <error.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* Inicializa um escalonador do tipo FIFO. Preenchendo a lista de processos
com PROCESSES. */
void FIFO_init(FIFO_t *eclnr, lista_t *processes)
{

    eclnr->prontos = processes;
}

/* Adiciona um processo pronto à lista de processos do escalonador FIFO. */
void FIFO_pronto(FIFO_t *eclnr, const process_t process)
{

    lista_push(eclnr->prontos, process);
}

/* Retorna o próximo processo que será executado. */
process_t FIFO_proximo(FIFO_t *eclnr)
{

    process_t p = lista_pop_front(eclnr->prontos);
    return p;
}

/* Inicializa um escalonador do tipo SJF. Preenchendo a lista de processos
com PROCESSES. */
void SJF_init(SJF_t *eclnr, lista_t *processes)
{

    eclnr->prontos = processes;
}

/* Adiciona um processo pronto à lista de processos do escalonador SJF. */
void SJF_pronto(SJF_t *eclnr, const process_t process)
{

    lista_push(eclnr->prontos, process);
}

/* Retorna o próximo processo que será executado. */
process_t SJF_proximo(SJF_t *eclnr)
{

    const unsigned minIndex = lista_min(eclnr->prontos);
    process_t p = lista_pop(eclnr->prontos, minIndex);
    return p;
}

/* Inicializa um escalonador do tipo Round Robin. Preenchendo a lista de processos
com PROCESSES. */
void RoundRobin_init(RoundRobin_t *eclnr, lista_t *processes)
{

    eclnr->prontos = processes;
}

/* Adiciona um processo pronto à lista de processos do escalonador Round Robin. */
void RoundRobin_pronto(RoundRobin_t *eclnr, const process_t process)
{

    lista_push(eclnr->prontos, process);
}

/* Retorna o próximo processo que será executado. */
process_t RoundRobin_proximo(RoundRobin_t *eclnr)
{

    process_t p = lista_pop_front(eclnr->prontos);
    return p;
}
