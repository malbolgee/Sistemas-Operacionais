#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/utils.h"

void alloc_check(const void *mem, const char *message)
{

    if (mem == NULL)
        perror(message), exit(1);
}

void ensure(bool expression, const char *message, void (*callback)(void))
{

    if (expression == false)
        fprintf(stderr, "%s", message), callback ? callback() : exit(1);
}
