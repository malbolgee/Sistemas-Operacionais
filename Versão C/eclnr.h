#pragma once
#include "process.h"
#include "lista.h"

/* Estrutura que representa um escalonador do tipo FIFO. */
typedef struct __sclnrFIFO
{

    lista_t *prontos;

} FIFO_t;

/* Estrutura que representa um escalonador do tipo SJF. */
typedef struct __sclnrSJF
{

    lista_t *prontos;

} SJF_t;

/* Estrutura que representa um escalonador do tipo STCF. */
typedef struct __sclnrSTCF
{

    lista_t *prontos;

} STCF_t;

/* Estrutura que representa um escalonador do tipo Round Robin. */
typedef struct __sclnrRounRobin
{

    lista_t *prontos;

} RoundRobin_t;

void FIFO_init(FIFO_t *eclnr, lista_t *processes);
void FIFO_pronto(FIFO_t *eclnr, const process_t process);
process_t FIFO_proximo(FIFO_t *eclnr);
void SJF_init(SJF_t *eclnr, lista_t *processes);
void SJF_pronto(SJF_t *eclnr, const process_t process);
process_t SJF_proximo(SJF_t *eclnr);
void RoundRobin_init(RoundRobin_t *eclnr, lista_t *processes);
void RoundRobin_pronto(RoundRobin_t *eclnr, const process_t process);
process_t RoundRobin_proximo(RoundRobin_t *eclnr);