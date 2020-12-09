#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>

#define EXPRESSION(expression) (#expression)

/** 
 * Checa se mem foi alocado corretamente imprimindo message caso um erro tenha ocorrido.
 * 
 * @param mem     Ponteiro para o endeço alocado.
 * @param message Um vetor de char com a mensagem a ser impressa.
 * 
 */
void alloc_check(const void *mem, const char *message);

/**
 * Checa se expression é true. Se expression for false, imprime message e 
 * executa callback, se este não foi null.
 * 
 * @param expression Uma expressão booleana.
 * @param message    Um vetor de char contendo uma mensagem a ser impressa em caso de erro.
 * @param callback   Uma função para ser executada em seguida.
 * 
*/
void ensure(bool expression, const char *message, void (*callback)(void));

#endif