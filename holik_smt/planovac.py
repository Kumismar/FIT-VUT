from z3 import *

# seznam dnu obsahujici seznam smen obsahujici usporadane dvojice reprezentujici dva zamestnance na smenu
zamci = [[
    (Int("z_%s_%s" % (i, j*2)), Int("z_%s_%s" % (i, j*2 + 1))) for j in range(3)
] for i in range(7)]

dva_zamci = [And(1 <= zamci[i][j][k], zamci[i][j][k] <= 9)
             for i in range(7)
             for j in range(3)
             for k in range(2)]

dva_unikatni_zamci = [zamci[i][j][0] != zamci[i][j][1]
                      for i in range(7)
                      for j in range(3)]

jedna_smena_za_den = [zamci[i][j][k] != zamci[i][m][n]
                      for i in range(7)
                      for j in range(3)
                      for k in range(2)
                      for m in range(j+1, 3)
                      for n in range(2)]

# priklad pridani omezeni ze zamestnanec 1 nechce farat v nedeli
# nechce_farat_v_nedeli = [zamci[6][j][k] != 1
#                          for j in range(3) for k in range(2)]

# Hazelo mi to SAT bez smen pro 8 a 9, tak jsem jeste pridal, ze kazdy musi mit aspon 4
min_4_smeny = [And([
    Sum([If(zamci[i][j][k] == z, 1, 0) for i in range(7)
        for j in range(3) for k in range(2)]) >= 4
    for z in range(1, 10)]
)]

s = Solver()
s.add(dva_zamci + dva_unikatni_zamci + jedna_smena_za_den + min_4_smeny)
print(s.check())
print(s.model())
