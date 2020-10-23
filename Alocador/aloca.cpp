#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <utility>
#include "aloca.h"

#ifdef __DEBUG__
    #define DEBUG
#endif

meualoc::meualoc(int tamanhoMemoria, int policitaMem)
{

    if (policitaMem > NEXTFIT)
        policitaMem = NEXTFIT;

    this->ptr_nextfit = 0;
    this->politica = policitaMem;
    this->memoria = (char *)malloc(sizeof(char) * (tamanhoMemoria));
    memset(this->memoria, 0, sizeof(char) * (tamanhoMemoria));
    this->livres.push_back(std::make_pair(this->memoria, tamanhoMemoria));

    #ifdef DEBUG
        puts("Blocos livres depois de alocação:");
        for (auto t : this->livres)
            printf("Endereço: %p tamanho: %hu\n", t.first, t.second);
    #endif
}

char *meualoc::aloca(unsigned short tamanho)
{

    return (this->*functions[this->politica])(tamanho);
}

char *meualoc::bestfit(unsigned short tamanho)
{

    #ifdef DEBUG
        printf("%s\n", __func__);
    #endif

    tamanho += 4;
    int index = 0;
    bool found = false;
    ushort menorTamaho = 0xFFFF;

    for (int i = 0; i < this->livres.size(); ++i)
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

    #ifdef DEBUG
        printf("%s\n", __func__);
    #endif

    tamanho += 4;
    int index = 0;
    bool found = false;

    size_t size = this->livres.size();

    for (int i = 0; i < size; ++i)
        if (tamanho <= this->livres[i].second)
            found = true, index = i, i = size;

    // Se não encontrou um bloco livre, retorna null;
    if (!found)
        return NULL;

    return auxfit(index, tamanho);
}

char *meualoc::nextfit(unsigned short tamanho)
{

    #ifdef DEBUG
        printf("%s\n", __func__);
    #endif

    tamanho += 4;
    bool found = false;

    size_t size = this->livres.size();

    for (int i = this->ptr_nextfit; i < size; ++i)
        if (tamanho <= this->livres[i].second)
            found = true, this->ptr_nextfit = i, i = size;

    if (!found)
    {
        this->ptr_nextfit = 0;
        for (int i = this->ptr_nextfit; i < size; ++i)
            if (tamanho <= this->livres[i].second)
                found = true, this->ptr_nextfit = i, i = size;
    }

    if (!found)
        return NULL;

    return auxfit(this->ptr_nextfit, tamanho);
}

char *meualoc::auxfit(int index, unsigned short tamanho)
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

        //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29
        // [1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

        #ifdef DEBUG
            printf("Tamanho somado ao endereço: %hu\n", tamanho);
        #endif

        this->livres[index].first += tamanho;
        this->livres[index].second -= tamanho;
    }

    #ifdef DEBUG
        puts("Blocos livres depois de alocação:");
        for (auto t : this->livres)
            printf("Endereço: %p tamanho: %hu\n", t.first, t.second);
    #endif

    return mem + 0x4;
}

char *meualoc::verifica(char *ponteiro, int posicao)
{

    if (*(unsigned short *)(ponteiro - 0x4) < posicao)
        return NULL;

    if (*(unsigned short *)(ponteiro - 0x2) == NUMERO_MAGICO)
        return ponteiro + posicao;
    else
        return NULL;
}

int meualoc::libera(char *ponteiro)
{

    ushort used = 0;
    ushort magic = 0;

    TAKE_USED(used, ponteiro)
    TAKE_MAGIC(magic, ponteiro)

    if (magic != NUMERO_MAGICO)
        return 0;

    SET_USED(0xFFFF, ponteiro - 0x4)
    SET_MAGIC(0xFFFF, ponteiro - 0x4)

    this->livres.push_back(std::make_pair(ponteiro - 0x4, used));
    coalesce();

    return 1;
}

void meualoc::coalesce()
{

    #ifdef DEBUG
        puts("Endereços dos espaços livres antes da ordenaçao:");
        for (auto t : this->livres)
            printf("Endereço: %p tamanho: %hu\n", t.first, t.second);
    #endif

    std::sort(this->livres.begin(), this->livres.end());

    #ifdef DEBUG
        puts("Endereços dos espaços livres depois da ordenaçao:");
        for (auto t : this->livres)
            printf("Endereço: %p tamanho: %hu\n", t.first, t.second);
    #endif

    // 0x000000ff[10]0x00000100[15]0x00000102[15]...

    //   0xff               0xf               0x10a
    // [<char *, ushort>, <char *, ushort>, <char *, ushort>]

    //   0xf               0xff      10       0x10a
    // [<char *, ushort>, <char *, ushort>, <char *, ushort>]

    int i = 1;
    size_t size = this->livres.size();

    while (i < size)
    {

        char *p = this->livres[i - 1].first;
        ushort t = this->livres[i - 1].second;

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

            #ifdef DEBUG
                printf("Olhando: %p\n", p + t);
            #endif

            // Aglutinação acontece aqui.
            this->livres.erase(this->livres.begin() + i);
            this->livres[i - 1].second += x;
        }
        else
            ++i;
    }

    ptr_nextfit = 0;
}

void meualoc::imprimeDados()
{

    int sum = 0;
    int maior = -1;

    size_t size = this->livres.size();

    #ifdef DEBUG
        puts("Endereços livres e seus tamanhos:");
        for (auto t : this->livres)
            printf("Endereço: %p tamanho: %hu\n", t.first, t.second);
    #endif

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