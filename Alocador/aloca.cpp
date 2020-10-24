#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>
#include "aloca.h"

#ifdef __DEBUG__
#define DEBUG
#endif

meualoc::meualoc(int tamanhoMemoria, int policitaMem)
{

    if (policitaMem > NEXTFIT)
        policitaMem = NEXTFIT;

    this->ptr_nextfit = 0UL;
    this->politica = policitaMem;
    this->memoria = (char *)malloc(sizeof(char) * (tamanhoMemoria));
    memset(this->memoria, 0, sizeof(char) * (tamanhoMemoria));
    this->livres.push_back(std::make_pair(this->memoria, tamanhoMemoria));
}

char *meualoc::aloca(unsigned short tamanho)
{

    return (this->*functions[this->politica])(tamanho);
}

char *meualoc::bestfit(unsigned short tamanho)
{

    if ((ushort)(tamanho + (ushort)0x4) < tamanho)
        return NULL;

    tamanho += 4;
    size_t index = 0UL;
    bool found = false;
    ushort menorTamaho = 0xFFFF;

    for (size_t i = 0UL; i < this->livres.size(); ++i)
    {

        if (tamanho <= this->livres[i].second)
        {

            found = true;
            if (this->livres[i].second < menorTamaho)
                index = i, menorTamaho = this->livres[i].second;
        }
    }

    if (!found)
        return NULL;

    return auxfit(index, tamanho);
}

char *meualoc::firstfit(unsigned short tamanho)
{

    if ((ushort)(tamanho + (ushort)0x4) < tamanho)
        return NULL;

    tamanho += 4;
    size_t index = 0UL;
    bool found = false;
    size_t size = this->livres.size();

    for (size_t i = 0UL; i < size; ++i)
        if (tamanho <= this->livres[i].second)
            found = true, index = i, i = size;

    // Se não encontrou um bloco livre, retorna null;
    if (!found)
        return NULL;

    return auxfit(index, tamanho);
}

char *meualoc::nextfit(unsigned short tamanho)
{

    // Overflow?
    if ((ushort)(tamanho + (ushort)0x4) < tamanho)
        return NULL;

    tamanho += 4;
    bool found = false;
    size_t size = this->livres.size();

    for (size_t i = this->ptr_nextfit; i < size; ++i)
        if (tamanho <= this->livres[i].second)
            found = true, this->ptr_nextfit = i, i = size;

    if (!found)
        return NULL;

    return auxfit(this->ptr_nextfit, tamanho);
}

char *meualoc::auxfit(size_t index, unsigned short tamanho)
{

    /**
     * 
     * Usa as macros para colocar nos bytes
     * mais significativos do bloco encontrado:
     *  - o tamanho em bytes do bloco alocado e 
     *  - o número mágico.
     *  
    */
    SET_USED(tamanho, this->livres[index].first)
    SET_MAGIC(NUMERO_MAGICO, this->livres[index].first)

    char *mem = this->livres[index].first;

    /*
     * Se o tamanho da memória que se quer alocar for
     * igual a quantidade de memória disponível,
     * então é necessário retirar por completo
     * esse bloco de espaço livre da lista de espaços
     * livres.
     * 
    */
    if (tamanho == this->livres[index].second)
        this->livres.erase(this->livres.begin() + index);
    else
    {
        /* 
         * Aqui ocorre o spliting;
         * 
         * O espaço de memória que se quer 
         * alocar é menor do que o disponível,
         * então pega um 'pedaço' do  espaço
         * disponível para a nova alocação;
         * 
         * Também é preciso informar onde começa o 
         * novo bloco de espaço livre (ponteiro para esse bloco),
         * para isso, soma-se o tamanho real alocado
         * ao ponteiro que indica o começo do bloco
         * livre.
         * 
        */
        this->livres[index].first += tamanho;
        this->livres[index].second -= tamanho;
    }

    return mem + 0x4;
}

char *meualoc::verifica(char *ponteiro, int posicao)
{

    ushort used = 0;
    ushort magic = 0;

    TAKE_USED(used, ponteiro)
    TAKE_MAGIC(magic, ponteiro)

    /**
     * 
     * Se a posição, a partir do ponteiro base
     * está fora dos limites, então essa é uma
     * posição que não está alocada para esse
     * ponteiro.
     * 
    */

    if (used - 0x4 <= posicao)
        return NULL;

    /**
     * 
     * Se o número mágico está definido
     * então **provavelmente** essa região está alocada.
     * 
    */
    if (magic == NUMERO_MAGICO)
        return ponteiro + posicao;
    else
        return NULL;
}

int meualoc::libera(char *ponteiro)
{

    if (ponteiro == NULL)
        return 0;

    if (verifica(ponteiro) == NULL)
        return 0;

    /**
     * 
     * Como a região está sendo desalocada,
     * é preciso redefinir o número mágico e
     * o tamanho do bloco usado, caso contrário
     * podem ocorrer falsos positivos em verificações
     * de disponibilidade futuras.
     * 
    */

    ushort used = 0;
    TAKE_USED(used, ponteiro)

    SET_USED(0xFFFF, ponteiro - 0x4)
    SET_MAGIC(0xFFFF, ponteiro - 0x4)

    this->livres.push_back(std::make_pair(ponteiro - 0x4, used));
    coalesce();

    return 1;
}

void meualoc::coalesce()
{

    std::sort(this->livres.begin(), this->livres.end());

    size_t i = 1UL;
    size_t size = this->livres.size();

    while (i < size)
    {

        char *p = this->livres[i - 1UL].first;
        ushort t = this->livres[i - 1UL].second;

        /**
         * 
         * Verifica se o ponteiro mais o offset
         * do tamanho do bloco é igual ao endereço
         * do próximo espaço livre, se for, então
         * esses blocos de espaços livres são adjacentes
         * e precisam ser aglutinados.
         * 
        */
        if (p + t == this->livres[i].first)
        {

            --size;
            ushort x = this->livres[i].second;

            // Aglutinação acontece aqui.
            this->livres.erase(this->livres.begin() + i);
            this->livres[i - 1UL].second += x;
        }
        else
            ++i;
    }

    /**
     * 
     * Reseta o ponteiro da nextfit porque
     * não faz mais sentido depois dos elementos
     * da lista de livres terem mudado de posição. 
     * 
    */
    ptr_nextfit = 0UL;
}

void meualoc::imprimeDados()
{

    int sum = 0;
    int maior = -1;

    size_t size = this->livres.size();

    for (auto t : this->livres)
    {

        sum += t.second;
        if (t.second > maior)
            maior = t.second;
    }

    printf("Número de blocos não alocados: %ld\n", size);
    printf("O tamanho do maior bloco vazio: %d\n", maior < 0 ? 0 : maior);
    printf("A média dos tamanhos dos blocos vazios: %.2lf\n", (double)sum / (size ? size : 1));
}

meualoc::~meualoc()
{

    free(this->memoria);
}