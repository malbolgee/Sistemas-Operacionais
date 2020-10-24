#define BESTFIT 0
#define FIRSTFIT 1
#define NEXTFIT 2

typedef unsigned short ushort;

/**
 * 
 * Número 'mágico' que indica
 * um bloco de memória alocado.
 * 
*/
#define NUMERO_MAGICO ((ushort)0xCDB0)

#define SET_USED(X, C)        \
	((*(C)) |= (X & 0x00FF)); \
	(*(C + 1) |= ((X & 0xFF00) >> 8));

#define SET_MAGIC(X, C)         \
	(*(C + 2) |= (X & 0x00FF)); \
	(*(C + 3) |= ((X & 0xFF00) >> 8));

#define TAKE_USED(X, C)           \
	(X |= ((*(C - 4)) & 0x00FF)); \
	((X |= ((*(C - 3)) << 8)));

#define TAKE_MAGIC(X, C)          \
	(X |= ((*(C - 2)) & 0x00FF)); \
	((X |= ((*(C - 1)) << 8)));

class meualoc
{

public:
	/**
	 * 
	 * Construtor da classe que aloca um bloco inicial de memória 
	 * para ser gerenciado.
	 * 
	 * @param tamanhoMemoria É a quantidade em bytes de memória a ser alocada;
	 * @param politicaMem É a política de escolha do bloco livre;
	 * 
	 * @note É alocado 10 bytes a mais do que o pedido;
	 * 
	*/
	meualoc(int tamanhoMemoria, int politicaMem);

	/**
	 * 
	 * Aloca tamanho bytes em memória e atualiza a lista de blocos livres.
	 * 
	 * @param tamanho Tamanho em bytes para ser alocado.
	 * @return Um ponteiro para o começo do bloco alocado ou null em caso de falha.
	 *  
	*/
	char *aloca(unsigned short int tamanho);

	/**
	 * 
	 * Verifica se a posição dada pelo ponteiro está alocada.
	 * 
	 * @param ponteiro É um ponteiro para um bloco de memória.
	 * @param posicao É um offset para o ponteiro.
	 * 
	 * @return Um ponteiro para a posição alocada ou null caso a região não esteja alocada.
	 * 
	*/
	char *verifica(char *ponteiro, int posicao = 0);

	/**
	 * 
	 * Libera um bloco alocado por aloca().
	 * 
	 * @param ponteiro Ponteiro a ser desalocado.
	 * @return 1 se foi possível desalocar com sucesso e 0 caso contrário.
	 * 
	*/
	int libera(char *ponteiro);

	/**
	 * 
	 * Imprime o número de elementos na lista de blocos livres.
	 * 
	 *  - O tamanho do maior bloco livre.
	 *  - A média dos tamanhos dos blocos livres.
	 * 
	*/
	void imprimeDados();

	/**
	 * 
	 * Destrutor da classe.
	 * 
	 * Desaloca a variável memória.
	 * 
	*/
	~meualoc();

private:
	/** 
	 * 
	 * Ponteiro que indinca onde a
	 * procura por um bloco livre deve começar. 
	 * 
	*/
	size_t ptr_nextfit;

	/**
	 * 
	 * Indica a política de gerenciamento
	 * de alocação usada pelo alocador;
	 * 
	*/
	int politica;

	char *memoria; /** Bloco de memória a ser gerenciado. */

	/**
	 * 
	 * Vector que guarda os blocos livres.
	 * 
	 * Cada posição guarda um par <char *, ushort>
	 * que são, respectivamente, um ponteiro para o 
	 * começo do bloco livre e o tamanho, em bytes, 
	 * desse bloco;
	 *  
	*/
	std::vector<std::pair<char *, ushort>> livres;

	/**
	 * 
	 * Função usa a política bestfit para fazer a alocação 
	 * de tamanho bytes.
	 * 
	 * @param tamanho Tamanho em bytes para ser alocado.
	 * @return Um ponteiro para o começo do bloco alocado ou null caso não haja espaço livre.
	 * @see firstfit() nextfit()
	 * 
	*/
	char *bestfit(unsigned short tamanho);

	/**
	 * 
	 * Função usa a política firstfit para fazer a alocação 
	 * de tamanho bytes.
	 * 
	 * @param tamanho Tamanho em bytes para ser alocado.
	 * @return Um ponteiro para o começo do bloco alocado ou null caso não haja espaço livre.
	 * @see bestfit() nextfit()
	 * 
	*/
	char *firstfit(unsigned short tamanho);

	/**
	 * 
	 * Função usa a política nextfit para fazer a alocação 
	 * de tamanho bytes.
	 * 
	 * @param tamanho Tamanho em bytes para ser alocado.
	 * @return Um ponteiro para o começo do bloco alocado ou null caso não haja espaço livre.
	 * @see bestfit() firstfit()
	 * 
	*/
	char *nextfit(unsigned short tamanho);

	/**
	 * 
	 * Função auxiliar usada por bestfit(), nextfit() e firstfit()
	 * para gerência dos espaços livres e definição 
	 * dos valores de número mágico e tamanho do bloco alocado.
	 * 
	 * @param index É o índice do bloco livre encontrado na lista de blocos livres.
	 * @param tamanho Tamanho em bytes a ser alocado.
	 * @return Um ponteiro para o começo do bloco alocado.
	 * 
	*/
	char *auxfit(size_t index, unsigned short tamanho);

	/**
	 * 
	 * Função que tenta aglutinar blocos livres adjacentes.
	 * 
	 * @see libera().
	 * 
	*/
	void coalesce();

	typedef char *(meualoc::*__alloc_fn_t)(unsigned short);
	__alloc_fn_t functions[3] = {&meualoc::bestfit,
								 &meualoc::firstfit,
								 &meualoc::nextfit};
};