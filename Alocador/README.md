Códigos para o trabalho do 'Alocador Pessoal' na disciplina de Sistemas Operacionais da UFAM.

Para compilar basta, dentro do diretório /src/, executar o comando:

```sh
$ make
```

Um executável chamado '*main*' será gerado.

Dentro do diretório /test/ há um *script* bash para gerar as saídas de acordo com os *workloads* do diretório /workloads/. Mas também é possível informar um *workload* diretamente ao programa para gerar as saídas apenas executando `$ ./main <WORKLOAD_FILE>`, o comportamento padrão é imprimir os resultados na saída padrão, por tanto, é aconselhável redirecionar a saída para um arquivo: `$ ./main <WORKLOAD_FILE> > output`.

Talvez seja necessário mudar a permissão de execução do *script*. Para isso basta executar:

```sh
$ chmod a+x gerarResultado.sh
$ ./gerarResultado.sh
```

Os resultados gerados serão colocados dentro do diretório /results/.

Também há um *script* para a geração dos gráficos: *gerarGraficos.sh*. A dinâmica é a mesma do *script* dos resultados, a única diferença é que é necessário informar ao *script* o *workload* para gera os gráficos:

```sh
$ ./gerarGraficos <NUMERO_WORKLOAD>
```

Em que NUMERO_WORKLOAD varia de `[0-2]`. Os gráficos gerados serão colocados no diretório /graphs/ e estarão identificados por nomes sugestivos.

Alguns dos gráficos gerados com os *workloads* padrões disponibilizados pelo professor só podem ser visualizados de maneira adequada mudando a escala do eixo ![equation](https://latex.codecogs.com/svg.latex?y) para uma escalada logarítimica. Para isso, será necessário adicionar o parâmetro `scale=log` na função `plotFromFile(...)` em plota.py.

Os Workloads são arquivos de texto cujo conteúdo simula o processo de alocação e desalocação de memória. Um arquivo de *workload* consiste de uma linha com dois inteiros ![equation](https://latex.codecogs.com/svg.latex?N) e ![equation](https://latex.codecogs.com/svg.latex?P), que são, respectivamente, a quantidade de memória total disponível e a política de alocação a ser usada. Logo em seguida, na próxima linha, há um inteiro ![equation](https://latex.codecogs.com/svg.latex?T), que indica a quantidade de linhas que virão.

Cada linha é identificada por um caracter ![equation](https://latex.codecogs.com/svg.latex?N), que indica a operação a ser realizada. Se ![equation](https://latex.codecogs.com/svg.latex?C) é 'A', então a operação é uma alocação e a linha vai ter mais dois inteiros, ![equation](https://latex.codecogs.com/svg.latex?U) e ![equation](https://latex.codecogs.com/svg.latex?L), que indicam, respectivamente o ![equation](https://latex.codecogs.com/svg.latex?id) da memória alocada e o tamanho em *bytes* dessa memória.

Se ![equation](https://latex.codecogs.com/svg.latex?C) é  'S', então a operação é de desalocação e a linha vai conter apeas o ![equation](https://latex.codecogs.com/svg.latex?id) de um bloco de memória alocado(ou não) previamente.

Os nomes dos arquivos de *workload* tem o padrão 'wxpol' em que o '*x'* é o número do *workload* e '*pol*' é a política a ser empregada. Por tanto, um exemplo de nome para um arquivo de *workload* seria: w0bft, indicando que é o *workload* 0 usando a política *bestfit*.
