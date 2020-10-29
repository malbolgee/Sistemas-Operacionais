#define _GNU_SOURCE

#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define PAGESIZE 4096
#define SALTO (PAGESIZE / sizeof(int))

double calc_media(double *v, size_t size);

int main(int argc, char **argv)
{

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0, &mask);
    sched_setaffinity(0, sizeof(cpu_set_t), &mask);

    int niter;

    #if 0
     char *string = "saida1.txt";
    #else
        char *string = "saida2.txt";
    #endif

    clock_t ini, fim;
    FILE *fp = fopen(string, "w");

    niter = 10000;
    for (int z = 10; z < 500; ++z)
    {

        double tempo_sec[niter];
        memset(tempo_sec, 0, sizeof(double) * niter);

        int v[(PAGESIZE * z) >> 2];

        for (int i = 0; i < niter; ++i)
        {

            ini = clock();
            for (size_t j = 0; j < z; ++j)
                v[j * SALTO] += 1; // <-- Acesso ao vetor;
            
            fim = clock();
            tempo_sec[i] = (double)(fim - ini) / CLOCKS_PER_SEC;
        }

        double media = calc_media(tempo_sec, niter);
        fprintf(fp, "%d %.15lf\n", z, media);
    }

    return 0;
}

double calc_media(double *v, size_t size)
{

    double media = 0.0;
    for (size_t i = 0; i < size; ++i)
        media += v[i];

    return media / size;
}