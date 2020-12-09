#ifndef __THREADHANDLER_H__
#define __THREADHANDLER_H__
#include <stdio.h>
#include <pthread.h>

#define NUMBER_MAX 4
#define NUMBER_SIZE 6
#define BUFFER_SIZE 100

/**
 * 
 * Cria nthreads em threads e escreve em output o conteúdo de buffers.
 * 
 * @param threads   É um vetor de threads.
 * @param buffers   É um vetor de buffers.
 * @param _nthreads É a quantidade de threads que devem ser criadas. 
 * 
*/
void threadHandle(pthread_t *threads, char *buffers[], size_t _nthreads);

/**
 * 
 * Comprime um arquivo de texto na codificação RUN-LENGTH.
 * 
 * @param arg É um ponteiro para um buffer.
 * @return    Um buffer comprimido.
 * 
*/
void *compress(void *arg);

/**
 * 
 * Ajusta os erros de quebra na string compactada.
 * 
 * @param buffers   Um vetor de buffers com as strings compacatadas.
 * @param _nthreads O número de threads usadas para compactar.
 * 
*/
char *stringFix(char *buffers[], size_t _nthreads);

/**
 * 
 * Pega uma substring de buffer no formato xc, em que
 * 'x' é um número de n algarismos e 'c' é um caracter.
 * 
 * @param buffer uma string compactada na codificação RUN-LENGTH.
 * @param index  um inteiro representando o índice onde começar a iteração.
 * @return       uma substring de buffer no formato descrito acima.
 * 
 * @see takeFirst()
 * 
*/
char *takeLast(char *buffer, int index);

/**
 * 
 * Pega uma substring de buffer no formato xc, em que
 * 'x' é um número de n algarismos e 'c' é um caracter.
 * 
 * @param buffer uma string compactada na codificação RUN-LENGTH.
 * @param index  um inteiro representando o índice onde começar a iteração.
 * @return       uma substring de buffer no formato descrito acima.
 * 
 * @see takeLast()
 * 
*/
char *takeFirst(char *buffer, int index);

#endif