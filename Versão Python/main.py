from Process import Processo
# from Scheduler import *
import matplotlib as plt
import numpy as np
from math import floor
import random
from collections import defaultdict


def run(echlr, nprocs, total=0, quantum=None, block_time=2, maximum_ts=100, chance_new=60, chance_io=30, min_time=4, max_time=10, total_tickets=None, unfairness=False):
    """ Realiza a simulação de um sistema operacional.

    echlr: É um escalonador;\n
    total: Total de timeslices dos processos;\n
    nprocs: Quantidade inicial de processos rodando;\n
    quantum: Quantum padrão para simulação;\n
    block_time: Quantidade de timeslices que um processo passa bloquado por I/O;\n
    maximum_ts: Quantidade total de timeslices que a simulação deve ter;\n
    chance_new: Chance padrão de surgir um novo processo;\n
    chance_io: Chance de padrão de um processo entrar em I/O;\n
    min_time & max_time: Indicam um range de tamanho em timeslices que um novo processo pode ter;\n
    unfairness: Indica se a medida de unfairness deve ser calculada, a unfairness vai sempre ser calculada\n
    entre os dois primeiros processos que acabarem;"""

    _io = False
    _tempo = 0
    _t_exec = 0
    _t_wait = 0
    _time = total
    _t_wait_dict = defaultdict(lambda: [0, False])

    if (isinstance(unfairness, bool)):
        if (unfairness == True):
            _uf = 0
            _unfairness_times = [0, 0]
    else:
        raise TypeError(
            "O Parâmetro 'unfairness' precisa ser um valor booleano True/False.")

    """ Dependendo do valor de 'total', a avaliação do while é diferente:
    Se o valor passado for menor ou igual a 0, significa que o escalonador é __provavelmente__ um que funciona por loteria,
    então o tempo total é avaliado 'de baixo para cima'. Caso contrário, o tempo é avaliado de 'cima para baixo'."""
    while (_time < maximum_ts if total <= 0 else _time > 0):

        # TODO: Crítico: Resolver o problema dos tickets aqui -> Por enquanto tá 0, mas tem que calcular direito!
        if ((_time < maximum_ts if total <= 0 else _time > 0) and random.randint(1, 100) < chance_new):
            p = Processo('P' + str(nprocs + 1), random.randint(1, chance_io),
                         random.randint(min_time, max_time), 0, _tempo, 0)
            echlr.pronto(p, born=True)
            nprocs += 1

        p = echlr.proximo()
        if (p is not None):
            _rodou, _io = p.roda(quantum)

            # Para calcular o tempo de resposta
            if (_t_wait_dict[p.name][1] == False):
                _t_wait_dict[p.name][0] = _tempo
                _t_wait_dict[p.name][1] = True

            _tempo += _rodou

            if (_io == True and p.size > 0):
                echlr.io_blocked_add(p, block_time)
            elif (p.size > 0):
                echlr.pronto(p)
            else:
                _t_exec += _tempo
                if (unfairness == True):
                    _unfairness_times[_uf] = _tempo
                    _uf += 1
                    if (_uf >= 2):
                        unfairness = False

            if (total <= 0):
                _time += _rodou
            else:
                _time -= _rodou

        # Pode cair aqui se não houver nenhum processo pronto, ou porque todos terminaram
        # ou porque todos estão bloqueados, caso aconteça, aumenta o tempo total gasto em 1 timeslice.
        else:
            _tempo += 1

        # Nos escalonadores que aceitam I/O, verifica quais processos
        # Estão prontos para desbloquear
        echlr.io_blocked()

    for value in _t_wait_dict.values():
        _t_wait += value[0]

    # É possível que o cálculo do tempo possa ficar errado caso o teste feito seja com um tempo específico
    # Menor do que o tempo máximo especificado (padrão ou não), alguns processos ficarão com tempo de término 0 e a média
    # Vai ser calculada erroneamente.
    _result = (_t_exec / nprocs, _t_wait / nprocs)
    print(echlr.name, "Tempo de Execução -> ",
          _result[0], "Tempo de Resposta -> ", _result[1])

    if (unfairness == True):
        print(' ' + echlr.name, " Unfarness -> ",
              _unfairness_times[0] / _unfairness_times[1])

    return _result


def make_process_list(amount, total=None, sizes=[], io_chance=[], size=10, io=0, random_size=False,
                      random_io=False, total_tickets=None, big_constant=100, ticket=False, random_tickets=False, tickets=[]):
    """ Cria uma lista de processos.

    OBS: Todos os processos sempre começam com 0 (a mais alta) de prioridade em qualquer cenário;

    amount: Quantidade de processos;
    total: Tamanho total dos processos em timeslices;
    sizes: Uma lista com o tamanho dos processos;
    io_chance: Uma lista com a chance de I/O dos processos;
    size: Tamanho padrão de um processo em timeslices;
    io: Chance padrão de I/O de um processo;
    random_size: Indica o uso de tamanhos de processos de forma randomizada, se True
    descarta ambos lista de tamanhos e tamanho padrão;
    random_io: Indica o uso de chances de I/O de processos de forma randomizada, se True
    descarta ambos lista de I/O;
    total_tickets: Quantidade de bilhetes totais de uma lista de processos;
    big_constant: Um número constante que é usado para se obter o stride de um processo (quando necessário);
    ticket: Indica o uso do sistema de bilhetes para a lista de processos;
    random_tickets: Indica se serão usados bilhetes gerados randomicamente, se True,
    descarta a lista de bilhetes passada;
    tickets: Uma lista com valores de bilhetes para os processos.

    Retorna uma tripla com o tamamho total dos processos criados, uma lista de processos e a quantidade de tickets restantes.
    """

    # Lida com o tamanho dos processos;
    # Se o parâmetro 'random_size' é True, então será gerado um array de tamanhos aleatórios
    # Em um range que vai de 1 até 'size';

    if (not isinstance(amount, int) or amount <= 0):
        raise TypeError(
            "O parâmetro 'amount' precisa ser um inteiro positivo maior do que 0.")

    if (not isinstance(io, int) or io < 0):
        raise TypeError("O parâmetro 'io' precisa ser um inteiro positivo.")

    if (not isinstance(size, int) or size < 1):
        raise TypeError(
            "O parâmetro 'size' precisa ser um inteiro positivo maior ou igual a 3.")

    if (not isinstance(random_size, bool)):
        raise TypeError(
            "O parâmetro 'random_size' precisa ser um valor booleano True/False.")
    elif (random_size == True):
        sizes = np.random.randint(1, high=size, size=amount)
    elif (isinstance(sizes, np.ndarray)):
        if (sizes.length == 0):
            sizes = np.array([size] * amount)
    else:
        if (not sizes):
            sizes = np.array([size] * amount)
        else:
            if (len(sizes) != amount):
                raise ValueError(
                    "A quantidade de valores de tamanhos é diferente da quantidade total de processos.")
            else:
                sizes = np.array(sizes)

    # Lida com as chances de I/O dos processos;
    if (not isinstance(random_io, bool)):
        raise TypeError(
            "O parâmetro 'random_io' precisa ser um valor booleano True/False.")
    if (random_io == True):
        io_chance = np.random.randint(100, size=amount)
    elif (isinstance(io_chance, np.ndarray)):
        if (io_chance.length == 0):
            io_chance = np.array([io] * amount)
    else:
        if (not io_chance):
            io_chance = np.array([io] * amount)
        else:
            if (len(io_chance) != amount):
                raise ValueError("A quantidade de valores de I/O é diferente da quantidade total de processos.")
            else:
                io_chance = np.array(io_chance)

    # Lida com a geração de bilhetes
    # 'total_tickets' só serve para gerar os bilhetes que serão distribuídos para os processos
    # Essa variável não é passada para nenhum processo (mas o que sobrar precisa ser retornado);
    # Para previnir que um processo fique com a quantidade máxima de bilhetes disponíveis,
    # Um processo só pode ficar com no máximo 75% da quantidade total de bilhetes disponíveis.
    if (not isinstance(ticket, bool)):
        raise TypeError(
            "O parâmetro 'ticket' precisa ser um valor booleano True/False.")
    elif (random_tickets == True):
        if (total_tickets is not None):
            tickets = []
            for _ in range(amount):
                number = random.randint(1, int(floor(total_tickets * 0.75)))
                total_tickets -= number
                if (total_tickets > 0):
                    tickets.append(number)
                else:
                    raise ValueError(
                        "A quantidade de bilhetes total não foi suficiente para gerar bilhetes para todos os processos.")
        else:
            raise TypeError(
                "Ao se gerar bilhetes aleatoriamente a quantidade total de bilhetes disponível não pode ser indefinida.")
    elif (ticket == False):
        tickets = [0] * amount
    elif (not isinstance(tickets, list)):
        raise TypeError("Para 'tickets', passe uma lista do python.")
    elif (not tickets):
        raise ValueError(
            "O parâmetro 'ticket' é True, mas uma lista vazia de bilhetes foi fornecida.")
    elif (len(tickets) != amount):
        raise ValueError(
            "A quantidade de bilhetes fornecida é diferente da quantidade de processos.")

    # Lida com o tamanho total dos processos;
    # As vezes, para fins de testes, o usuário pode passar uma quantidade específica de timeslices
    # Diferente da soma do tamanho dos processos;
    if (total is None):
        total_size = sizes.sum()  # Posso fazer isso porque tenho certeza que sizes é um ndarray
    else:
        if (isinstance(total, int) and total > 0):
            total_size = total
        else:
            raise TypeError(
                "O parâmetro 'total' precisa ser um inteiro positivo maior do que 0.")

    # Para separar os strides dos processos, divide-se o big_constant pela quantidade de bilhetes atribuídas àquele processo.
    # Entretanto, se a quantidade total de bilhetes disponíveis for maior do que a big_constant, é garantido que um processo
    # Qualquer recebeu uma quantidade de tickets menor do que big_constant e, por tanto, a divisão do jeito que está implementada
    # Daria 0. Mesmo com essa verificação aqui, é necessário verificar novamente dentro de Process.
    if (ticket == True):
        if (not isinstance(big_constant, int) or big_constant <= 0):
            raise ValueError(
                "O parâmetro 'big_constant' precisa ser um inteiro positivo maior do que 0.")
        if (total_tickets >= big_constant):
            raise ValueError(
                "O parâmetro 'big_constant' precisa ser maior do que a quantidade total de bilhetes disponíveis.")

    processess = []
    for i in range(amount):
        processess.append(
            Processo('P' + str(i), io_chance[i], sizes[i], 0, 0, big_constant, tickets=tickets[i]))

    return total_size, processess, total_tickets


random.seed(0)
# Exemplos de uso:

# 1 -> Cria uma lista com 3 processos, nenhum outro parâmetro foi passado, o que significa que todos os valores estão como padrão.
# Todos os processsos têm tamanho 10, 0% chance de I/O. Os processão não usarão tickets;
pc_list = make_process_list(3)
print(pc_list)

# 2 -> Cria uma lista com 3 processos com tamanhos randômicos.
pc_list = make_process_list(3, random_size=True)
print(pc_list)

# 3 -> Cria uma lsita com 3 processo com ambos tamanhos e chances de I/O randômicos.
pc_list = make_process_list(3, random_size=True, random_io=True)
print(pc_list)

# 4 -> Nesse exemplo, é gerada uma lista com 3 processos, criando bilhetes de forma aleatória de um total de 400 bilhetes.
# Ao final, uma tripla é devolvida contendo a soma total dos tamanhos dos processos, uma lista com os processos e a quantidade de bilhetes restante.
pc_list = make_process_list(3, random_tickets=True, total_tickets=400)
print(pc_list)

# 5 -> Uma lista onde todos os processos têm tamanho 100
pc_list = make_process_list(3, size=100)
print(pc_list)

# 6 -> Nesse exemplo, uma lista com os tamanhos é passada.
pc_list = make_process_list(3, sizes=[10, 20, 30])
print(pc_list)
