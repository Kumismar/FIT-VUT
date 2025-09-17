import sys
from z3 import *

# README:
# Tento kod je sice matematicky moc pekny, ale neni moc pouzitelny pro vetsi n (n > 7 zhruba).
# Pouzil jsem treti souradnici vektoroveho soucinu, abych poznal, jestli jsou body na stejne primce (plati i pro diagonaly).
# Bohuzel, pro n = 6 uz to trvalo pres minutu a dal jsem to uz ani nenechaval dojet (a to si myslim, ze nemam uplne spatnou masinu).
# Nicmene si myslim, ze je to matematicky velmi zajimave a elegantni reseni, proto jsem jej odevzdal.

try:
    n = int(sys.argv[1])
except:
    n = 5

# Pocitam s n < 46 (uz pro n > 10 by na to stejne asi byl potreba superpocitac)
num_points = 2*n

# Body v mrizce
points = [(Int("x_%s" % i), Int("y_%s" % i)) for i in range(num_points)]
points_in_grid = [And(0 <= x, x < n, 0 <= y, y < n) for x, y in points]

s = Solver()
s.add(points_in_grid)

# Spocitej z souradnici vekt soucinu bodu a pokud z == 0, pak jsou na stejne primce (obema smery)
for i in range(num_points):
    for j in range(i + 1, num_points):
        for k in range(j + 1, num_points):
            x1, y1 = points[i]
            x2, y2 = points[j]
            x3, y3 = points[k]

            s.add(((x2 - x1)*(y3 - y1) - (y2 - y1)*(x3 - x1)) != 0)


print(s.check())
print(s.model())
