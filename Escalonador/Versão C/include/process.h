#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdio.h>
#include <stdbool.h>

/* Estrutura que representa um processo. */
typedef struct __process
{

    char nome[10];    /** Nome do processo*/
    unsigned io;      /** Chance de I/O. */
    size_t tam;       /** Tamanho do processo (tempo de execução). */
    int prio;         /** Prioridade do processo. */
    unsigned chegada; /** Tempo de chegada. */
    bool done_io;     /** Indica se processo fez I/O ou não. */

} process_t;

/** 
 * "Executa" um processo. 
 * 
 * @param process Um ponteiro para um objeto process.
 * @param quantum Um inteiro sem sinal representando o quantum que o processo tem. 
 *  
 * @return Um inteiro sem sinal representando o quantum restante do processo.
 * 
 */
unsigned run(process_t *process, unsigned quantum);

/** 
 * Cria um processo.
 * 
 * @param pnome  Uma string com o nome do processo.
 * @param pio    Um inteiro representando a chance de I/O de um processo.
 * @param ptam   Um inteiro representando o tamanho de um processo (tempo de execução).
 * @param prio   Um inteiro representando a prioridade desse processo.
 * @param ptempo Um inteiro representando o tempo de chagada desse processo.
 * 
 * @return Um ponteiro para um objeto process_t criado.
 * 
*/
process_t *createProcess(const char *pnome, const unsigned pio, const size_t ptam,
                         const unsigned pprio, const unsigned ptempo);
#endif