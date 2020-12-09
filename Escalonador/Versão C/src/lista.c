#include <stdlib.h>
#include <stdbool.h>
#include "../include/utils.h"
#include "../include/lista.h"

void lista_init(lista_t *lista)
{

    lista->first = NULL;
    lista->last = NULL;
}

void lista_push(lista_t *lista, process_t *process)
{

    lista_node *node;
    printf("Process Name: %s\n", process->nome);
    node = (lista_node *)malloc(sizeof(lista_node));
    alloc_check(node, "Falha em alocar nó da lista");

    if (lista->first)
        lista->last->next = node;
    else
        lista->first = node;

    lista->size++;
    node->next = NULL;
    lista->last = node;
    node->processo = process;
}

process_t *lista_pop_front(lista_t *lista)
{

    process_t *return_ = NULL;

    if (lista->first)
    {

        if (lista->first->next)
        {

            return_ = lista->first->processo;
            lista->first = lista->first->next;
            return return_;
        }
        else
        {

            return_ = lista->first->processo;
            lista->first = lista->last = NULL;
            return return_;
        }
    }

    return NULL;
}

process_t *lista_pop(lista_t *lista, const size_t index)
{

    process_t *return_ = NULL;
    lista_node *node, *previus;
    node = lista->first;

    if (lista == NULL)
        return NULL;

    if (lista->size <= index)
        return NULL;

    size_t i = 0UL;
    if (node)
    {

        if (index == 0UL)
        {

            return_ = node->processo;
            lista->first = lista->first->next;
            return return_;
        }

        ++i;
        previus = node;
        node = node->next;

        while (node)
        {

            if (index == i)
            {

                return_ = node->processo;
                previus->next = node->next;
                return return_;
            }

            previus = node;
            node = node->next;
            ++i;
        }
    }

    return NULL;
}

size_t lista_min(const lista_t *lista)
{

    size_t i = 0;
    size_t minSize = 0;
    size_t minIndex = 0;
    lista_node *node = NULL;

    node = lista->first;

    while (node)
    {

        if (node->processo->tam < minSize)
            minSize = node->processo->tam, minIndex = i;

        ++i;
        node = node->next;
    }

    return minIndex;
}

bool lista_is_empty(const lista_t *lista)
{

    return (lista->first == NULL);
}

process_t *lista_front(const lista_t *lista)
{

    return lista->first->processo;
}

void lista_free(lista_t *lista)
{

    lista_node *previus = NULL;
    lista_node *node = lista->first;

    while (node)
    {

        previus = node;
        node = node->next;
        free(previus);
    }

    lista->first = NULL;
}