#ifndef __ECLNR_H__
#define __ECLNR_H__

#include "./lista.h"
#include "./process.h"

/** Objeto pase para representação dos escalonadores. */
typedef struct __sclnr
{
    lista_t *prontos; /** Lista de processos prontos. */
} sclnr_t;

typedef sclnr_t FIFO_t;       /** Estrutura que representa um escalonador do tipo FIFO. */
typedef sclnr_t SJF_t;        /** Estrutura que representa um escalonador do tipo SJF. */
typedef sclnr_t STCF_t;       /** Estrutura que representa um escalonador do tipo STCF. */
typedef sclnr_t RoundRobin_t; /** Estrutura que representa um escalonador do tipo Round Robin. */

/** 
 * Inicializa um escalonador preenchendo a lista de processos. 
 * @param eclnr     Um ponteiro para um escalonador.
 * @param processes Um ponteiro para uma lista de processos.
 * 
 */
void eclnrInit(sclnr_t *eclnr, lista_t *processes);

/**
 * 
 * Adiciona um processo à lista de processos do escalonador.
 * @param eclnr   Um ponteiro para uma escalonador.
 * @param process Um ponteiro para um objeto process_t.
 * 
*/
void eclnrPronto(sclnr_t *eclnr, process_t *process);

/**
 * 
 * Seleciona o próximo processo para ser executado.
 * @param sclnr Um ponteiro para um escalonador.
 * 
 * @return Um ponteiro para um objeto process_t.
 * 
*/
process_t *eclnrProximo(sclnr_t *eclnr);

#endif