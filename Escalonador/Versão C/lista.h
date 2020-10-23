#pragma once
#include <stdbool.h>
#include "process.h"

typedef struct __lista__node{

    process_t processo;
    struct __lista__node *next;

} lista_node;

typedef struct __lista{

    size_t size;
    lista_node *first;
    lista_node *last;

} lista_t;

void lista_init(lista_t *lista);
void lista_push(lista_t *lista, const process_t process);
process_t lista_pop_front(lista_t *lista);
process_t lista_pop(lista_t *lista, const size_t index);
unsigned lista_min(const lista_t *lista);
bool lista_is_empty(const lista_t *lista);
process_t lista_front(const lista_t *lista);
void lista_free(lista_t *lista);
