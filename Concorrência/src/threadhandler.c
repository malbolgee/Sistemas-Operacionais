#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/utils.h"
#include "../include/threadhandler.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

char *stringFix(char *buffers[], size_t _nthreads)
{

    // "10c10a", "20a", "5a15b", "10b10p", "10p10b"
    // "10c30a", "5a15b", "10b10p", "10p10b"
    // "10c35a15b", "10b10p", "10p10b"
    // "10c35a25b10p", "10p10b"
    // "10c35a25b20p10b"
    // "10c35a25b20p10b"

    // "10c10a", "7k13a", "10l10b", "10b10p"
    // "10c10a7k13a", "10l10b", "10b10p"
    // "10c10a7k13a10l10b", "10b10p"
    // "10c10a7k13a10l20b10p"

    // "10c10a", "7k10a3l", "7l13b", "7b10p"
    // "10c10a7k10a3l", "7l13b", "7b10p"
    // "10c10a7k10a10l13b", "7b10p"
    // "10c10a7k10a10l20b10p"

    unsigned short rlen1 = 0;
    unsigned short rlen2 = 0;
    unsigned short rlent = 0;

    char *strOut = (char *)calloc((BUFFER_SIZE << 1) * _nthreads, sizeof(char));

    strcat(strOut, buffers[0]);
    for (size_t i = 1; i < _nthreads; ++i)
    {

        int j, k;
        char aux[NUMBER_SIZE] = {0};

        int len = strlen(strOut);
        int sz = len - 1;
        char c = strOut[sz];

        j = k = 0;
        // Pega os dígitos da quantidade do primeiro caracter em buffer[i]
        while (isdigit(buffers[i][j]))
            aux[k++] = buffers[i][j++];

        rlen1 = strtol(aux, NULL, 10);

        // Se for igual ao caracter do buffer anterior, quer dizer que a string foi quebrada.
        // Se não foi, então apenas concateno a string do buffer[i] inteira.
        if (c == buffers[i][j])
        {

            sz--;
            k = NUMBER_MAX;
            // Pega a quantidade do caracter do buffer anterior.
            while (isdigit(strOut[sz]))
                aux[k--] = strOut[sz--];

            rlen2 = strtol(&aux[k + 1], NULL, 10);
            rlent = rlen1 + rlen2;

            sprintf(aux, "%hu%c", rlent, c);

            // Posiciona o ponteiro na string.
            len -= 2;
            while (isdigit(strOut[len]))
                --len;

            // Ao fazer isso, retiro o valor desatualizado da quantidade do último caracater.
            // Ex: "4c10b" => "4c", 10b foi retirado da string.
            strOut[len + 1] = 0;
            // Atualiza com a nova quantidade.
            strcat(strOut, aux);
            // Coloca o resto da string (sem a quantidade antiga).
            strcat(strOut, &buffers[i][j + 1]);
        }
        else
            strcat(strOut, buffers[i]);
    }

    return strOut;
}

void threadHandle(pthread_t *threads, char *buffers[], size_t _nthreads)
{

    for (size_t k = 0; k < _nthreads; ++k)
        pthread_create(&threads[k], NULL, compress, (void *)buffers[k]);

    for (size_t k = 0; k < _nthreads; ++k)
        pthread_join(threads[k], (void **)&buffers[k]);
}

void *compress(void *arg)
{

    pthread_mutex_lock(&lock);
    char *s = (char *)arg;
    size_t len = strlen(s);
    char _string[len + 1];
    memmove(_string, s, len + 1);
    pthread_mutex_unlock(&lock);

    unsigned short count;
    char number[NUMBER_SIZE] = {0};
    char *string = (char *)calloc(BUFFER_SIZE << 1, sizeof(char));
    alloc_check(string, "Não foi possível alocar memória");

    for (size_t i = 0, j = 0; i < len; ++i)
    {

        count = 1;
        while (i + 1 < len && _string[i] == _string[i + 1])
            ++count, ++i;

        sprintf(number, "%hu", count);
        strcat(string, number);

        j += strlen(number);
        string[j++] = _string[i];
    }

    free(arg);
    return (void *)string;
}

char *takeLast(char *buffer, int index)
{

    size_t i = NUMBER_MAX + 1;
    char *out =(char *)calloc(BUFFER_SIZE << 1, sizeof(char));
    out[i--] = buffer[index--];

    while (isdigit(buffer[index]))
        out[i--] = buffer[index--];

    return &out[i + 1];
}

char *takeFirst(char *buffer, int index)
{

    size_t i = 0;
    char *out = (char *)calloc(BUFFER_SIZE << 1, sizeof(char));
    out[i++] = buffer[index++];

    while (isdigit(buffer[index]))
        out[i++] = buffer[index++];

    out[i] = buffer[index];

    return out;
}