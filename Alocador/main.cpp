#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <cstring>
#include "aloca.h"

int main(int argc, char **argv)
{

    meualoc c = meualoc(60, NEXTFIT);

    char *p1 = c.aloca(16);
    char *p2 = c.aloca(16);
    char *p3 = c.aloca(16);
    char *p4 = c.aloca(16);

    if (p1 != nullptr)
        printf("p1 foi\n");
    else
        printf("p1 não foi :/\n");

    if (p2 != nullptr)
        printf("p2 foi\n");
    else
        printf("p2 não foi :/\n");

    if (p3 != nullptr)
        printf("p3 foi\n");
    else
        printf("p3 não foi :/\n");
    
    if (p4 != nullptr)
        printf("p4 foi\n");
    else
        printf("p4 não foi :/\n");

    // if (c.verifica(p1, 25) != NULL)
    //     puts("p1 + 25 está alocado.");
    // else
    //     puts("p1 + 25 não está alocado.");

    // if (c.verifica(p1, 16) != NULL)
    //     puts("p1 + 16 está alocado.");
    // else
    //     puts("p1 + 16 não está alocado.");

    puts("Endereço de p1:");
    printf("%p\n", p1);
    puts("Endereço de p2:");
    printf("%p\n", p2);
    puts("Endereço de p3:");
    printf("%p\n", p3);

    // c.libera(p1);

    p4 = c.aloca(16);

    if (p4 != nullptr)
        printf("p4 foi\n");
    else
        printf("p4 não foi :/\n");

    c.libera(p3);
    c.libera(p2);

    c.imprimeDados();

    return 0;
}