# Author: Ondrej Koumar, xkouma02

import sys
import math


def diagonals_init():
    """Ke kazdemu policku priradi cislo, ktere rika, kolik klauzuli se ma vygenerovat pri generovani zleva doprava, shora dolu.

    Funkce postupne zmensuje sachovnici a generuje pouze prvni radek a prvni sloupec pro aktualni velikost.
    """
    global x
    global indices

    for n in range(N, 0, -1):
        # pro prvni radek kazde sachovnice vygeneruj [n-1,...,0]
        for i, j in zip(range(1, n+1), range(n-1, -1, -1)):

            x.update({indices[0]: j})
            indices.pop(0)

        # pro prvni sloupec kazde sachovnice vygeneruj [n-1,...,0]
        # mimo prvni prvek - ten uz se pridal v radku
        # zacatek sachovnic je na diagonale - 1, N+2, 2N+3, 3N+4, etc., navic je tam offset radku
        for i, j in zip(range((N - n + 1)*N + (N - n + 1), N**2, N), range(n-2, -1, -1)):
            x.update({i: j})
            indices.remove(i)


def diagonals_init_2():
    """Stejne jako diagonals_init(), ale vytvori slovnik pro diagonaly pro generovani zprava doleva, shora dolu.
    """
    global y
    global indices2
    for n in range(N, 0, -1):
        for i, j in zip(range(1, n+1), range(0, n)):
            y.update({indices2[0]: j})
            indices2.pop(0)

        for i, j in zip(range((N - (n-1))*N + n, N**2+1, N), range(n-2, -1, -1)):
            y.update({i: j})
            indices2.remove(i)


if len(sys.argv) < 2 or len(sys.argv) > 3:
    print(f"Usage: {sys.argv[0]} N [filename]\n")
    exit(-1)

if len(sys.argv) == 3:
    f = open(sys.argv[2], 'w')
else:
    f = sys.stdout

N = int(sys.argv[1])

if N <= 0:
    sys.stderr.write("N musi byt vetsi nez 0\n")
    sys.exit(1)

n_of_clauses = 2*(N + N*(math.comb(N, 2)))

sequence = [*range(1, N + 1)] + [*range(N-1, 0, -1)]
diags = 0
for i in sequence:
    diags += math.comb(i, 2)
n_of_clauses += 2*diags

f.write(f"p cnf {N**2} {1 if N == 1 else n_of_clauses}\n")

# radky
for i in range(1, N**2, N):
    # aspon jeden ze vsech
    to_write = ""
    for j in range(i, i+N):
        to_write += f"{j} "
    f.write(to_write + "0\n")
    # nekompatibilni dvojice
    for j in range(i, i+N-1):
        for k in range(j+1, i+N):
            f.write(f"-{j} -{k} 0\n")

# sloupce
for i in range(1, N+1):
    # aspon jeden ze vsech
    to_write2 = ""
    for j in range(i, N**2 + 1, N):
        to_write2 += f"{j} "
    f.write(to_write2 + "0\n")
    # nekompatibilni dvojice
    for j in range(i, N**2 - N + 1, N):
        for k in range(j + N, N**2 + 1, N):
            f.write(f"-{j} -{k} 0\n")

# diagonaly
indices = [*range(1, N**2 + 1)]
indices2 = indices.copy()
x = {}
y = {}

diagonals_init()
diagonals_init_2()

for i, c in x.items():
    tmp = i + N + 1
    for c2 in range(c):
        f.write(f"{-i} {-tmp} 0\n")
        tmp += N + 1

for i, c in y.items():
    tmp = i + N - 1
    for c2 in range(c):
        f.write(f"{-i} {-tmp} 0\n")
        tmp += N - 1

f.close()
