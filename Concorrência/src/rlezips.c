#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/utils.h"

void usage(void);
void compress(FILE *, FILE *);

int main(int argc, char **argv)
{

    if (argc != 3)
        usage();

    FILE *input = fopen(argv[1], "rb");
    alloc_check(input, "Falha ao abrir arquivo");

    FILE *output = fopen(argv[2], "w");
    alloc_check(output, "Falha ao abrir arquivo");

    compress(input, output);

    fclose(input);
    fclose(output);

    return 0;
}

void usage(void)
{

    puts("main <arqentrada> <arqsaida>");
    exit(1);
}

void compress(FILE *input, FILE *output)
{

    char c1, c2;
    unsigned short count = 1;

    c1 = c2 = 0;
    fread(&c1, sizeof(char), 1, input);

    c2 = c1;
    while (fread(&c1, sizeof(char), 1, input))
    {

        if (c1 == c2)
            ++count;
        else
        {

            fprintf(output, "%hu%c", count, c2);

            c2 = c1;
            count = 1;
        }
    }

    fprintf(output, "%hu%c", count, c2);
}