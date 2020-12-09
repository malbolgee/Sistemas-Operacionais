#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
#include <ctype.h>
#include "../include/utils.h"
#include "../include/threadhandler.h"

void usage(void);

int main(int argc, char **argv)
{

    if (argc != 4)
        usage();

    FILE *input = fopen(argv[1], "rb");
    alloc_check(input, "Falha ao abrir arquivo");

    FILE *output = fopen(argv[2], "w");
    alloc_check(output, "Falha ao abrir arquivo");

    size_t nthreads = strtol(argv[3], NULL, 10);
    ensure(nthreads > 5, "Argumento inválido", usage);

    struct stat st;
    stat(argv[1], &st);
    size_t sz = st.st_size;

    if (sz <= BUFFER_SIZE)
    {

        char *buffer = (char *)calloc(sz, sizeof(char));
        fread(buffer, sizeof(char), sz, input);
        buffer = (char *)compress(buffer);
        fprintf(output, "%s", buffer);
        free(buffer);
    }
    else
    {

        size_t _nthreads = sz / (size_t)BUFFER_SIZE;

        if (nthreads > _nthreads)
        {

            nthreads = _nthreads + 1;
            pthread_t threads[nthreads];
            char *buffers[nthreads];

            for (size_t k = 0; k < nthreads; ++k)
                buffers[k] = (char *)calloc(BUFFER_SIZE + 1, sizeof(char));

            size_t i = 0;
            while (!feof(input))
                fread(buffers[i++], sizeof(char), BUFFER_SIZE, input);

            threadHandle(threads, buffers, nthreads);

            char *ret = stringFix(buffers, _nthreads);

            fprintf(output, "%s", ret);
            for (size_t k = 0; k < nthreads; ++k)
                free(buffers[k]);

            free(ret);
        }
        else
        {

            char *buffers[nthreads];
            pthread_t threads[nthreads];
            char *rets[_nthreads];

            for (size_t k = 0; k < nthreads; ++k)
                buffers[k] = (char *)calloc(BUFFER_SIZE + 1, sizeof(char));

            for (size_t k = 0; k < _nthreads; ++k)
                rets[k] = NULL;

            size_t i = 1;
            size_t j = 0;
            while (!feof(input))
            {

                fread(buffers[i - 1], sizeof(char), BUFFER_SIZE, input);

                if (i == nthreads)
                {

                    i = 1;
                    threadHandle(threads, buffers, nthreads);
                    rets[j++] = stringFix(buffers, nthreads);
                }
                else
                    ++i;
            }

            if (--i)
            {
                threadHandle(threads, buffers, i);
                rets[j++] = stringFix(buffers, i);
            }

            fprintf(output, "%s", stringFix(rets, j));

            for (size_t i = 0; i < nthreads; ++i)
                free(buffers[i]);

            for (size_t i = 0; i < _nthreads; ++i)
                free(rets[i]);
        }

        fclose(input);
        fclose(output);

        return 0;
    }
}

void usage(void)
{

    puts("threads <arqentrada> <arqsaida> <nthreads>");
    exit(1);
}
