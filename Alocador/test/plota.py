import sys
import matplotlib.pyplot as plt
import numpy as np
from typing import List


def plotFromFile(yaxis: list, xaxis: List[list], ylabel: str, xlabel: str = "Número de Instruções", title: str = "", filename: str = "file1.png", plot: bool = False, scale: str = "linear") -> None:

    plt.figure(figsize=(9.840, 6.160), dpi=300)

    plt.plot(xaxis, yaxis[0], label="Bestfit")
    plt.plot(xaxis, yaxis[1], label="Firstfit")
    plt.plot(xaxis, yaxis[2], label="Nextfit")

    plt.yscale(scale)
    plt.ylabel(ylabel)
    plt.xlabel(xlabel)

    plt.legend(loc="lower right", prop={'size': 12})
    plt.title(title)

    if (plot == True):
        plt.show()
    else:
        plt.savefig("./graphs/" + filename.split('/')[-1], dpi=300)


op = int(sys.argv[1])
filebft = open(sys.argv[2], "r")
filefst = open(sys.argv[3], "r")
filenft = open(sys.argv[4], "r")

yaxis1 = []
yaxis2 = []
yaxis3 = []
xaxis = np.linspace(1, 10000, num=10000)

ops = {0: int, 1: int, 2: int, 3: float}

labels = ["Nº De Falhas de Alocação", "Quantidade de Blocos Livres",
          "Tamanho do Maior Bloco Livre", "Média dos Tamanhos dos Blocos Livres"]

extension = ".png"
fileNames = ["gr_falhas", "gr_blocosLivres",
             "gr_tamanhoMaiorBloco", "gr_mediaTamanhoBlocos"]

print(sys.argv[2])
print(sys.argv[3])
print(sys.argv[4])
print(labels[op])
print(fileNames[op])

f = ops[op]
for fline1, fline2, fline3 in zip(filebft, filefst, filenft):
    yaxis1.append(f(fline1.split()[op]))
    yaxis2.append(f(fline2.split()[op]))
    yaxis3.append(f(fline3.split()[op]))

workload = "w" + sys.argv[2].split('/')[-1][1]
titles = {'w0': "Workload 0", 'w1': "Workload 1", 'w2': "Workload 2"}

plotFromFile([yaxis1, yaxis2, yaxis3], xaxis, labels[op],
             filename=fileNames[op] + workload + extension, title=titles[workload])
