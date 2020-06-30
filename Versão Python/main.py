from Process import Processo
from Scheduler import *
import matplotlib as plt
import numpy as np
from math import floor


def nameYielder(total):

    i = 0
    while (total >= 0):
        nome = 'P' + str(i)
        i += 1
        total -= 1
        yield nome


def run_simple(echlr, total, nprocs, quantum=None):

    tempo = 0
    t_exec = 0
    t_wait = 0
    t_wait_dict = {nome: [0, False] for nome in nameYielder(nprocs)}

    while (total > 0):
        p = echlr.proximo()
        if (p is not None):
            rodou, _ = p.roda(quantum)

            if (t_wait_dict[p.nome][1] == False):
                t_wait_dict[p.nome][0] = tempo
                t_wait_dict[p.nome][1] = True

            tempo += rodou

            if (p.tam > 0):
                echlr.pronto(p)
            else:
                t_exec += tempo

            total -= tempo

        else:
            tempo += 1

    for value in t_wait_dict.values():
        t_wait += value[0]

    result = (t_exec / nprocs, t_wait / nprocs)
    print(echlr, "Tempo de Execução -> ",
          result[0], "Tempo de Resposta -> ", result[1])
    return result


def run_complex(echlr, total, time, nprocs, block=2, quantum=2, maximo=100, chance_new=60, chance_io=30, minTime=4, maxTime=10):

    bloqueados = []
    nprocs_aux = nprocs + 1
    while (tempo < maximo):

        if (tempo < maximo and random.randint(1, 100) < chance_new):
            p = Processo('P' + str(nprocs_aux), random.randint(1, chance_io),
                         random.randint(minTime, maxTime), 0, tempo, 8)
            echlr.pronto(p, born=True)
            nprocs_aux += 1
        p = echlr.proximo()

        if (p is not None):
            rodou, fez_io = p.roda(quantum)

            if (fez_io and p.tam > 0):
                bloqueados.append([p, block + 1])
            elif (p.tam > 0):
                echlr.pronto(p)

            total - rodou
            tempo += rodou

        else:
            tempo += 1

        i = 0
        lim = len(bloqueados)

        while (i < lim):
            bloqueados[i][1] -= 1
            if (bloqueados[i][1] == 0):
                echlr.pronto(bloqueados[i][0], fez_io=True)
                del bloqueados[i]
                lim -= 1
            else:
                i += 1


def make_process_list(amount, total=None, sizes=[], io_chance=[], size=10, io=0, random_size=False,
                      random_io=False, total_tickets=None, big_constant=100, ticket=False, random_tickets=False, tickets=[]):
    """ Cria uma lista de processos.

    OBS: Todos os processos sempre começam com 0 de prioridade em qualquer cenário;

    amount: Quantidade de processos;
    total: Tamanho total dos processos em timeslices;
    sizes: Uma lista com o tamanho dos processos;
    io_chance: Uma lista com a chance de I/O dos processos;
    size: Tamanho padrão de um processo em timeslices;
    io: Chance padrão de I/O de um processo;
    random_size: Indica o uso de tamanhos de processos de forma randomizada, se True
    descarta ambos lista de tamanhos e tamanho padrão;
    random_io: Indica o uso de chances de I/O de processos de forma randomizada, se True
    descarta ambos lista de I/O e I/O padrão;
    total_tickets: Quantidade de bilhetes totais de uma lista de processos;
    big_constant: Um número constante que é usado para se obter o stride de um processo (quando necessário);
    ticket: Indica o uso do sistema de bilhetes para a lista de processos;
    random_tickets: Indica se serão usados bilhetes gerados randomicamente, se True,
    descarta a lista de bilhetes passada;
    tickets: Uma lista com valores de bilhetes para os processos.

    Retorna uma tupla com o tamamho total dos processos criados e uma lista de processos.
    """

    # Lida com o tamanho dos processos;
    # Se o parâmetro 'random_size' é True então será gerado um array de tamanhos aleatórios
    # Em um range que vai de 3 até 'size', se 'size' é 0 ou None, o número 10 é usado como padrão (Nenhuma exception é levantada).;
    if (random_size == True):
        sizes = np.random.randint(
            3, high=10 if size is None or size == 0 else size, size=amount)
    elif (isinstance(sizes, np.ndarray)):
        if (sizes.length == 0):
            sizes = np.array([size] * amount)
    else:
        if (not sizes):
            sizes = np.array([size] * amount)
        else:
            sizes = np.array(sizes)

    # Lida com as chances de I/O dos processos;
    if (random_io == True):
        io_chance = np.random.randint(100, size=amount)
    elif (isinstance(io_chance, np.ndarray)):
        if (io_chance.length == 0):
            io_chance = np.array([io] * amount)
    else:
        if (not io_chance):
            io_chance = np.array([io] * amount)
        else:
            io_chance = np.array(io_chance)

    # Lida com a geração de bilhetes
    # 'total_tickets' só serve para gerar os bilhetes que serão distribuídos para os processos
    # Essa variável não é passada para nenhum processo;
    # Para previnir que um processo fique com a quantidade máxima de bilhetes disponíveis,
    # Um processo só pode ficar com no máximo 75% da quantidade total de bilhetes disponíveis.
    if (ticket == False):
        tickets = [0] * amount
    elif (random_tickets == True):
        if (total_tickets is not None):
            tickets = []
            tickets_aux = total_tickets
            for _ in range(amount):
                number = random.randint(1, int(floor(tickets_aux * 0.75)))
                tickets_aux -= number
                if (tickets_aux > 0):
                    tickets.append(number)
                else:
                    raise ValueError(
                        "A quantidade de bilhetes total não foi suficiente para gerar bilhetes para todos os processos.")

        elif (total_tickets == 0):
            raise ValueError(
                "Ao se gerar bilhetes aleatoriamente a quantidade total de bilhetes disponível não pode ser 0.")
        else:
            raise TypeError(
                "Ao se gerar bilhetes aleatoriamente a quantidade total de bilhetes disponível não pode ser indefinida.")
    elif (isinstance(tickets, np.ndarray)):
        raise TypeError("Para a lista de tickets, passe uma lista do python.")
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
                "O parâmetro 'total' precisa ser um inteiro positivo diferente de 0.")

    # Para separar os strides dos processos, divide-se o big_constant pela quantidade de bilhetes atribuídas àquele processo.
    # Entretanto, se a quantidade total de bilhetes disponíveis for maior do que a big_constant, é garantido que um processo
    # Qualquer recebeu uma quantidade de tickets menor do que big_constant e, por tanto, a divisão do jeito que está implementada
    # Daria 0. Mesmo com essa verificação aqui, é necessário verificar novamente dentro de Process.
    if (ticket == True):
        if (big_constant is None or big_constant <= 0):
            raise ValueError("")
        if (total_tickets >= big_constant):
            raise ValueError(
                "O parâmetro 'big_constant' precisa ser maior do que a quantidade total de bilhetes disponíveis.")

    processess = []
    for i in range(amount):
        processess.append(
            Processo('P' + str(i), io_chance[i], sizes[i], 0, 0, big_constant, tickets=tickets[i]))

    return total_size, processess
