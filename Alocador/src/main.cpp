#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <cstring>
#include "../include/aloca.h"

int main(int argc, char **argv)
{

    int tam, pol, n;

    tam = pol = n = 0;
    std::cin >> tam >> pol >> n;
    meualoc mem = meualoc(tam, pol);

    char *ids[n];
    memset(ids, 0, sizeof(char *) * n);

    unsigned fails = 0;

    while (n--)
    {

        char op;
        size_t id;
        std::cin >> op;

        if (op == 'A')
        {
            
            ushort siz;
            std::cin >> id >> siz;
            ids[id] = mem.aloca(siz);

            if (ids[id] == NULL)
                ++fails;

            printf("%d ", fails);
            mem.imprimeDados();
        }
        else
        {
            std::cin >> id;
            mem.libera(ids[id]);
            printf("%d ", fails);
            mem.imprimeDados();
        }

    }

    return 0;
}