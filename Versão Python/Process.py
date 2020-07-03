import random
from math import floor


class Processo(object):
    """ Representa um processo.

    name: O nome do processo;\n\r
    io: Chance do processo fazer I/O;\n\r
    size: Tamanho do processo (timeslices);\n\r
    prio: Prioridade do processo;\n\r
    arrival: Tempo de chegada de um processo;\n\r
    big_number: Um número grande para dividir pelos tickets e se obter o stride de um processo (quando necessário);\n\r
    tickets: Quantidade de bilhetes de um processo (quando necessário);"""

    def __init__(self, name, io, size, prio, arrival, big_number, tickets=0):
        self.name = name
        self.io = io
        self.size = size
        self.prio = prio
        self.arrival = arrival
        # TODO: Ajeitar essa condição.
        try:
            self.tickets = int(floor(big_number / tickets))
        except ZeroDivisionError:
            self.tickets = tickets

    def roda(self, quantum=None):
        if(random.randint(1, 100) < self.io):
            self.size -= 1
            print(self.name, " Fez I/O, falta ", self.size)
            return 1, True

        if(quantum is None or self.size < quantum):
            quantum = self.size

        self.size -= quantum
        print(self.name, " rodou por ", quantum,
              " timeslice, faltam ", self.size)
        return quantum, False

    def __lt__(self, other):
        return self.prio < other.prio

    def __str__(self):
        return "Name: " + self.name + ", Size: " + str(self.size)
