import time
def factorial_via_prime_decomposition(number):
    isPrime = [True] * (number + 1)
    result = 1

    for p in range(2, number + 1):
        if isPrime[p]:
            # prime sieve
            j = p + p
            while j <= number:
                isPrime[j] = False
                j += p
            # highest power k of prime p is in n! is given by
            # k = floor(n / p) + floor(n / p^2) + floor(n / p^3) + ...
            k = 0
            copy = p
            while copy <= number:
                k += number // copy
                copy = copy * p
            # factorial is product of primes raised to their highest power
            result =  result *  (p ** k)

    return result


def factorial_naive(number):
    result = 1
    for i in range(2, number + 1):
        result = result * i
    return result



# benchmark on number of factorials computed
print("benchmark on number of factorials computed")

start = time.time()
answers_prime_decomposition = []
for i in range(1000):
    answers_prime_decomposition.append(factorial_via_prime_decomposition(i))
end = time.time()
print("factorial_via_prime_decomposition: ", end - start)


start = time.time()
answers_naive = []
for i in range(1000):
    answers_naive.append(factorial_naive(i))
end = time.time()
print("factorial_naive: ", end - start)


#############################


# benchmark on size of factorials computed
print("benchmark on size of factorials computed")

start = time.time()
answers_prime_decomposition = []
for i in range(1000):
    answers_prime_decomposition.append(factorial_via_prime_decomposition(1000))
end = time.time()
print("factorial_via_prime_decomposition: ", end - start)


start = time.time()
answers_naive = []
for i in range(1000):
    answers_naive.append(factorial_naive(1000))
end = time.time()
print("factorial_naive: ", end - start)

