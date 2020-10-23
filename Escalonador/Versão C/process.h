#include <stdio.h>
#pragma once

/* Estrutura que representa um processo. */
typedef struct __process{

    char nome[10];
    unsigned io;
    size_t tam;
    int prio;
    unsigned chegada;
    bool done_io;

} process_t;

unsigned run(process_t *process, unsigned quantum);
process_t createProcess(const char *pnome, const unsigned pio, const size_t ptam, const unsigned pprio, const unsigned ptempo);
