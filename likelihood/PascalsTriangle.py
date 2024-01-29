import numpy as np
import math
def makeTriangle(m, n):
    # make array of 1s
    p = np.ones((m+n, n), dtype=int)
    for i in range(1, m+n):
        for j in range(1, n):
            p[i][j] = p[i-1][j] + p[i][j-1]

    return p



def check(p, m, n):
    for i in range(m+n):
        for j in range(n):
            if math.comb(i+j, j) == p[i][j]:
                print("True")
            else:
                print("False")
                print(i, j)
                print(math.comb(i+j, j), p[i][j])
    return




m = 10
n = 5

p = makeTriangle(10, 5)

print(p)

check(p, m, n)

