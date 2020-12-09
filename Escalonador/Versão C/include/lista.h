#ifndef __LISTA_H__
#define __LISTA_H__

#include <stdbool.h>
#include "./process.h"

/** Representa um nó de uma lista.*/
typedef struct __lista__node
{

    process_t *processo;        /** Objeto process_t. */
    struct __lista__node *next; /** Ponteiro para o próximo elemento. */

} lista_node;

/** Representa uma lista. */
typedef struct __lista
{

    size_t size;       /** Tamanho da lista. */
    lista_node *first; /** Ponteiro para o primeiro elemento. */
    lista_node *last;  /** Ponteiro para o último elemento. */

} lista_t;

/**
 * Inicializa uma lista vazia.
 * 
 * @param lista Um ponteiro para um objeto lista_t.
 * 
*/
void lista_init(lista_t *lista);

/**
 * Adiciona um elemento na lista.
 * 
 * @param lista   Um ponteiro para um objeto lista_t.
 * @param process Um ponteiro para um objeto process_t.
 * 
*/
void lista_push(lista_t *lista, process_t *process);

/**
 * Retira o primeiro elemento da lista.
 * 
 * @param lista Um ponteiro para um objeto lista_t.
 * @return No sucesso, retorna um objeto process_t com informações sobre o processo, em falha,
 * returna um objeto process_t vazio.
 * 
 */
process_t *lista_pop_front(lista_t *lista);

/**
 * Retira o elemento no índice index na lista.
 * 
 * @param lista Um ponteiro para um objeto lista_t.
 * @param index Um inteiro representando o índice em que se encontra o elemento a ser retirado.
 * 
 * @return no sucesso, retorna um objeto process_t com informações sobre o processo. Em falha,
 * retorna null.
 *  
*/
process_t *lista_pop(lista_t *lista, const size_t index);

/** 
 * Retorna o índice do menor elemento de uma lista.
 * 
 * @param lista Um ponteiro para uma lista.
 * 
 * @return Um inteiro representando o índice do menor elemeneto da lista. 
 * 
 */
size_t lista_min(const lista_t *lista);

/**
 * Verifica se uma lista está vazia.
 * @param lista Um ponteiro par uma lista.
 * 
 * @return true se a lista está vazia, false se há pelo menos um elemento.
 * 
*/
bool lista_is_empty(const lista_t *lista);

/**
 * Retorna o primeiro elemento sem retirá-lo da lista.
 * 
 * @param lista Um ponteiro para uma lista.
 * 
 * @return Um ponteiro para um objeto process_t.
 * 
*/
process_t *lista_front(const lista_t *lista);

/** 
 * Libera lista da memória. 
 * @param lista Um ponteiro para um objeto lista_t
 *  
 */
void lista_free(lista_t *lista);

#endif