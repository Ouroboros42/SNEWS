import time
from FactorialMethods import factorial_naive as fac_1, factorial_via_prime_decomposition as fac_2

n = 100
m = 150
x = 1
y = 1

print("\n\n\n")

# naive method
sum_naive = 0
start = time.time()
for r in range(n):
    for j in range(m):
        Temp =  fac_1(m +  n - r - j) / (fac_1(m - j) * fac_1(n - r))
        Temp *= (x ** j) * (y ** r) / (fac_1(j) * fac_1(r))
        sum_naive += Temp
end = time.time()
print("naive: ", end - start)
print("sum_naive: ", sum_naive)


# prime decomposition method
sum_prime_decomposition = 0
start = time.time()
for r in range(n):
    for j in range(m):
        Temp =  fac_2(m +  n - r - j) / (fac_2(m - j) * fac_2(n - r))
        Temp *= (x ** j) * (y ** r) / (fac_2(j) * fac_2(r))
        sum_prime_decomposition += Temp
end = time.time()
print("prime decomposition: ", end - start)
print("sum_prime_decomposition: ", sum_prime_decomposition)




