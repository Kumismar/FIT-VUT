import z3

a, b = z3.Bools("a b")

solver = z3.Solver()

solver.add(z3.And(z3.Not(a), z3.Not(b)) == z3.Not(z3.Or(a, b)),
           z3.Not(z3.And(a, b)) == z3.Or(z3.Not(a), z3.Not(b)))
print(solver.check())
