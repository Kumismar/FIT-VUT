import z3

vi, a, vf, d, t = z3.Reals("vi, a, vf, d, t")

solver = z3.Solver()
solver.add(a == -9,
           vi == 25,
           vf == 0,
           t == (vf - vi)/a,
           d == vi*t + (a*(t**2))/2)

print(solver.check())
print(solver.model())
