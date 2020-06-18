#include "lista.h"
#include "eclnr.h"
#include <errno.h>
#include <err.h>
#include <unistd.h>
#include <error.h>
#include <stdlib.h>
#include <stdbool.h>

/* Inicializa uma lista vazia. */
void lista_init(lista_t *lista)
{

    lista->first = NULL;
    lista->last = NULL;
}

/* Adiciona um elemento no final da lista. */
void lista_push(lista_t *lista, const process_t process)
{

    lista_node *node;

    printf("Process Name: %s\n", process.nome);
    node = (lista_node *)malloc(sizeof(lista_node));

    if (!node)
    {

        perror("Error");
        exit(errno);
    }

    if (lista->first)
        lista->last->next = node;
    else
        lista->first = node;

    lista->size++;
    node->next = NULL;
    lista->last = node;
    node->processo = process;
}

/* Retira o primeiro elemento da lista. No sucesso retorna um objeto PROCESS_T
com infos de um processo na lista. Em falha, retorna um objeto PROCESS_T vazio. */
process_t lista_pop_front(lista_t *lista)
{

    lista_node *node;
    process_t return_ = {0};

    if (lista->first)
    {

        if (lista->first->next)
        {

            return_ = lista->first->processo;
            node = lista->first;
            lista->first = lista->first->next;
            free(node);
            return return_;
        }
        else
        {

            return_ = lista->first->processo;
            node = lista->first;
            lista->first = lista->last = NULL;
            free(node);
            return return_;
        }
    }

    return return_;
}

/* Retira o elemento no índice INDEX da LISTA e retorna um objeto PROCESS_T
com informações sobre esse processo. Se o índice não existir na lista,
retorna uma objeto PROCRESS_T vazio. */
process_t lista_pop(lista_t *lista, const size_t index)
{

    process_t return_ = {0};
    lista_node *node, *previus;
    node = lista->first;

    if (lista == NULL)
        return return_;

    if (lista->size <= index)
        return return_;

    size_t i = 0;

    if (node)
    {

        if (index == 0)
        {

            return_ = node->processo;
            lista->first = lista->first->next;
            free(node);
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
                free(node);
                return return_;
            }

            previus = node;
            node = node->next;
            ++i;
        }
    }

    return return_;
}

/* Retorna o índice do menor elemento de LISTA. Menor elemento
significa o processo com o menor tamanho (tempo de execuçução). */
unsigned lista_min(const lista_t *lista)
{

    unsigned i = 0;
    unsigned minIndex = 0;
    lista_node *node;
    unsigned minTam = 0x3f3f3f;

    node = lista->first;

    while (node)
    {

        if (node->processo.tam < minTam)
        {

            minIndex = i;
            minTam = node->processo.tam;
        }

        ++i;
        node = node->next;
    }

    return minIndex;
}

/* Verifica se a LISTA está vazia. */
bool lista_is_empty(const lista_t *lista)
{

    return (lista->first == NULL);
}

/* Retorna um objeto PROCESS_T que está no início de LISTA. */
process_t lista_front(const lista_t *lista)
{

    return lista->first->processo;
}

/* Libera LISTA da memória. */
void lista_free(lista_t *lista)
{

    lista_node *previus;
    lista_node *node = lista->first;

    while (node)
    {

        previus = node;
        node = node->next;
        free(previus);
    }

    lista->first = NULL;
}