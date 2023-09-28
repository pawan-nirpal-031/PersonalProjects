import z3

def solve(phi):
    s = z3.Solver()
    s.add(phi)
    s.check()
    return s.model()

if __name__=='__main__':
    # formule =  (z3.Int('x')/7==6)
    # y = z3.BitVec('y',8)
    # print(solve(y<<3==40))
    #print(solve(formule))
    z = z3.Int('z')
    n = z3.Int('n')
    formula = z3.ForAll([z],z*n==z)
    print(solve(formula))
