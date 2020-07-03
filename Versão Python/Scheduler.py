import numpy as np
import random
from math import floor
from heapq import heappush, heappop


class Scheduler(object):
    """ Classe base para representar um tipo Escalonador."""

    def __init__(self, vprontos=[]):
        self.prontos = vprontos

    def pronto(self, Processo, io=False, born=False):
        self.prontos.append(Processo)

    def io_blocked(self, Processo, io=False, born=False):
        pass

    def io_blocked_add(self, Process, block_time):
        pass

    def proximo(self):
        return self.prontos.pop(0)


class MLFQ(Scheduler):
    """  Recebe uma lista de processo inicial e inicializa uma lista de filas
    que representam diferentes níveis de prioridade para o escalonador.
    Essa lista inicialmente é colocada no mais alto nível de prioridade.

    lines: Quantidade de filas que o escalonador vai ter;
    boost: De quanto em quanto tempo o escalonador vai dar um boost;
    quamtum_lines: o quantum de cada fila;"""

    def __init__(self, vprontos=[], lines=7, boost=50, quantum_lines=None):
        super().__init__(vprontos)
        self.name = "EscalonadorMLFQ"
        self.filas = []
        self.boost = boost
        self._auxboost = boost
        self.lines = lines
        self.quantum = quantum_lines
        self.filas.append(vprontos)
        if (self.lines >= 1):
            for _ in range(self.lines - 1):
                self.filas.append([])

    def pronto(self, Processo, io=False, born=False):
        """ Adiciona um processo pronto à fila de processos de prioridade adequada.

        io: Indica se um processo está saindo de I/O;
        born: Indica se um processo acabou de ser criado;"""
        if (born == True):
            self.filas[0].append(Processo)
            return
        if (Processo.prio < (self.lines - 1)):
            if (io == False):
                Processo.prio += 1

            self.filas[Processo.prio].append(Processo)

    def proximo(self):
        """ Procura nas filas de prioridade um processo pronto.
        Se existir um processo nas filas de mais alta prioridade,
        este será escolhido. Se as filas estiverem vazias, retorna None."""

        for item in self.filas:
            if (len(item) != 0):
                return item.pop(0)

        return None

    def boost_line(self, time_spent):
        """ Função de BOOST do MLFQ. Depois de um tempo S definido,
        todos os processos que estão em filas de menor prioridade
        são realocados para a fila de máxima prioridade.

        time_spent: Indica a quantidade de tempo que será subtraída
        do tempo total para boost;"""

        if (self.boost - time_spent <= 0):
            self.boost = self._auxboost
            for i in range(1, len(self.filas)):
                for j in self.filas[i]:
                    j.prio = 0

                self.filas[0] += self.filas[i]
                self.filas[i] = []
        else:
            self.boost -= time_spent

    # TODO: Implementar método para processos bloqueados


class LOTERIA(Scheduler):
    """ Recebe uma lista de processos e uma quantidate total de tickets (bilhetes).
    Essa lista de processos vai ser ordenada de forma descrecente por quantidade de tickets
    que, nesse contexto, faz o papel de prioridade do processo. """

    # TODO: é preciso implementar o sistema de quantum das filas;
    def __init__(self, vprontos=[], total_tickets=400):
        super().__init__(vprontos)
        self.name = "escalonadorLOTERIA"
        self.total_tickets = total_tickets
        self._total_tickets_aux = total_tickets
        self._io_blocked_list = []
        self.prontos.sort(key=lambda a: a.prio, reverse=True)

    def pronto(self, Processo, io=False, born=False):
        self.prontos.append(Processo)
        self.prontos.sort(key=lambda a: a.prio, reverse=True)

    def proximo(self):
        total = 0
        number = random.randint(0, self._total_tickets_aux)

        for i in range(len(self.prontos)):
            total += self.prontos[i].bilhetes
            if (number <= total):
                return self.prontos.pop(i)

        return None

    def io_blocked(self):
        i, lim = 0, len(self._io_blocked_list)

        while (i < lim):
            self._io_blocked_list[i][1] -= 1
            if (self._io_blocked_list[i][1] == 0):
                self.pronto(self._io_blocked_list[i][0], io=True)
                del self._io_blocked_list[i]
                lim -= 1
            else:
                i += 1

    def io_blocked_add(self, Process, block_time):
        self._io_blocked_list.append([Process, block_time + 1])


class STRIDE(Scheduler):
    """ Escalonador por STRIDE. Recebe uma lista de processos. Inicialmente,
    todos os processos têm 0 strides, que pode ser visto como a prioridade de um processo.
    É usada uma lista de prioridade para deixar sempre o processo com a menor quantidade de stride
    no topo da lista."""

    def __init__(self, vprontos=[]):
        super().__init__(vprontos)
        self.name = "escalonadorSTRIDE"

    def pronto(self, Processo, io=False, born=False):
        Processo.prio += Processo.bilhetes
        heappush(self. prontos, Processo)

    def proximo(self):
        return heappop(self.prontos)


class FIFO(Scheduler):
    """ Representa um escalonador do tipo FIFO. Recebe uma lista de processos
    e usa os métodos como foram definidos na classe mãe."""

    def __init__(self, vprontos=[]):
        super().__init__(vprontos)
        self.name = "EscalonadorFIFO"


class SJF(Scheduler):
    """ Representa um escalonador do tipo SJF (Shortest Job First).
    Recebe uma listade processos e, na hora de escolher qual vai rodar,
    escolhe o que tem menor tamanho (quantidade de tempo que irá rodar). """

    def __init__(self, vprontos=[]):
        super().__init__(vprontos)
        self.name = "EscalonadorSJF"

    def proximo(self):
        return self.prontos.pop(self.getMin())

    # TODO: Verificar se é possível não achar nenhum processo para retornar
    def getMin(self):
        minIndex, minTam = 0, 0x3f3f3f
        for (idx, item) in enumerate(self.prontos):
            if (item.size < minTam):
                minIndex, minTam = idx, item.size

        return minIndex


class RoundRobin(Scheduler):
    """ Rrepresenta um escalonador do tipo Round Robin. Recebe uma lista de processos.
    Utiliza todos os métodos como definidos na classe mãe. A difrença desse escalonador 
    para o FIFO é apenas o valor de quantum que será utilizado. Uma vez que no FIFO não existe
    esse conceito."""

    def __init__(self, vprontos=[]):
        super().__init__(vprontos)
        self.name = "EscalonadorRoundRobin"
