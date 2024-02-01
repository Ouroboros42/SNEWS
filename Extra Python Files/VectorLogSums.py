class LogVector:

    def __init__(self, max_value):
        self.max_value = max_value
        self.LogVector = []
        self.LogFactorialVector = []

    # create vector of log values from 1 to max_value
    def createLogVector(self):
        self.LogVector = [np.log(i) for i in range(1, self.max_value + 1)]

    def createLogFactorialVector(self):
        self.LogFactorialVector = [sum(self.LogVector[:i]) for i in range(1, self.max_value + 1)]

    def verifyFactorials(self, precision) -> str:
        global i
        try:
            for i in range(1, self.max_value + 1):
                if math.log(math.factorial(i)) - self.LogFactorialVector[i - 1] > precision:
                    print(math.log(math.factorial(i)) - self.LogFactorialVector[i - 1])
                    return "False at " + str(i)
        except OverflowError:
            return "OverflowError after " + str(i)
        return "True"

    def printFactorialVector(self):
        for i in range(1, 100):
            print(i, round(np.exp(self.LogFactorialVector[i - 1])))






# benchmark

import time
import math
import numpy as np

max_value = 3000

ob = LogVector(max_value)
ob.createLogVector()

start = time.time()
ob.createLogFactorialVector()
end = time.time()

print(ob.verifyFactorials(0.001))
print("Time taken to create LogFactorialVector: ", end - start)

