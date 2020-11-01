#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <cstring>
#include <limits>
#include <sstream>
#include <cstdio>
#include "../include/aloca.h"

enum IDS
{
    NALOCADO,
    ALOCADO,
    DALOCADO
};

enum PRTOPT
{
    ALOCACAO,
    DALOCACAO,
    OUTROS
};

int main(int argc, char **argv)
{


    FILE *fp = fopen(argv[2], "r");

    if (fp == NULL)
        return 1;

    size_t printOption = atol(argv[1]);
    
    int tam, pol, n;
    tam = pol = n = 0;
    fscanf(fp, "%d %d %d%*c", &tam, &pol, &n);
    meualoc mem = meualoc(tam, pol);

    char *ids[n];                                                /** Vetor de ponteiros que guarda os ponteiros já alocados. */
    memset(ids, 0, sizeof(char *) * n);

    size_t sumTamAlocado = 0;                                    /** Soma dos tamanhos, em bytes, que obtiveram sucesso na alocação. */
    size_t sumTamNAlocado = 0;                                   /** Soma dos tamanhos, em bytes, que falharam na alocação. */
    size_t qtsSucessoAlocado = 0;                                /** Quantidade de alocações que foram bem-sucedidas. */
    size_t tamMaiorAlocado = std::numeric_limits<size_t>::min(); /** O tamanho da maior memória alocada. */

    size_t qtsDAlocadoFalha = 0;                                 /** Quantidade de dealocações que falharam. */
    size_t qtsDAlocadoFalhaJaDAlocado = 0;                       /** Quantidade de dealocações que falharam porque o ponteiro já estava dealocado. */
    size_t qtsDAlocadoFalhaNAlocado = 0;                         /** Quantidade de dealocações que falharam porque o ponteiro não estava dealocado. */

    size_t qtsSucessoDAlocado = 0;                               /** Quantidade de dealocações que foram bem-sucedidas. */

    size_t qtsAlocacoes = 0;                                     /** Quantidade de instruções de alocação. */
    size_t qtsDAlocacoes = 0;                                    /** Quantidade de instruções de dealocação. */

    std::unordered_map<size_t, char> s;                          /**  Hashmap que guarda o id de um ponteiro e a situação desse ponteiro (Alocado, dealocado ou não-alocado). */

    while (n--)
    {

        char op;
        size_t id;
        fscanf(fp, "%c%*c", &op);

        if (op == 'A')
        {
            
            ushort siz;
            fscanf(fp, "%ld %hu%*c", &id, &siz);

            ids[id] = mem.aloca(siz);

            if (ids[id] != NULL)
            {

                s[id] = ALOCADO;
                sumTamAlocado += siz;
                ++qtsSucessoAlocado;

                if (siz > tamMaiorAlocado)
                    tamMaiorAlocado = siz;
            }
            else
                s[id] = NALOCADO;

            ++qtsAlocacoes;

            if (printOption == ALOCACAO)
                printf("%ld %ld\n", qtsSucessoAlocado, qtsAlocacoes - qtsSucessoAlocado);
            else if (printOption == DALOCACAO)
                printf("%ld %ld %ld %ld\n", qtsSucessoDAlocado, qtsDAlocacoes - qtsSucessoDAlocado, qtsDAlocadoFalhaJaDAlocado, qtsDAlocadoFalhaNAlocado);
        }
        else
        {

            ushort magic = 0;
            fscanf(fp, "%ld%*c", &id);

            if (mem.libera(ids[id]))
            {

                ids[id] = NULL;
                s[id] = DALOCADO;
                ++qtsSucessoDAlocado;
            }
            else
            {

                if (s[id] == DALOCADO)
                    ++qtsDAlocadoFalhaJaDAlocado;
                else if (ids[id] == NULL || s[id] == NALOCADO)
                    ++qtsDAlocadoFalhaNAlocado;

                ushort magic = 0;
                TAKE_MAGIC(magic, ids[id]);
            }

            ++qtsDAlocacoes;

            if (printOption == ALOCACAO)
                printf("%ld %ld\n", qtsSucessoAlocado, qtsAlocacoes - qtsSucessoAlocado);
            else if (printOption == DALOCACAO)
                printf("%ld %ld %ld %ld\n", qtsSucessoDAlocado, qtsDAlocacoes - qtsSucessoDAlocado, qtsDAlocadoFalhaJaDAlocado, qtsDAlocadoFalhaNAlocado);
        }
    }

    if (printOption == OUTROS)
        printf("%ld %ld %ld %ld\n", qtsAlocacoes, sumTamAlocado / qtsSucessoAlocado, tamMaiorAlocado, qtsDAlocacoes);

    fclose(fp);

    return 0;
}