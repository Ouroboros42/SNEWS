import numpy as np
from numpy.polynomial import Polynomial
from numpy.polynomial.polynomial import polypow
from numpy.polynomial.polynomial import polymul

import json
import matplotlib.pyplot as plt

def averageFitCoefficients(json_file, num_samples, degree=10):
    coefficients = np.zeros(degree + 1)

    for i in range(num_samples):
        key = str(i)
        L_data = json_file[key]["Likelihoods"]
        T_data = json_file[key]["Offsets"]
        coefficients += np.polyfit(T_data, L_data, degree)

    return coefficients / num_samples


def normalise(p, x_min, x_max):
    norm = p.integ()(x_max) - p.integ()(x_min)
    return p / norm


def expectedValue(f, pdf, x_min, x_max):
    integrand = polymul(f, pdf)[0]
    norm = pdf.integ()(x_max) - pdf.integ()(x_min)
    if np.isclose(norm, 1):
        print("pdf was normalised")
    else:
        print("pdf was not normalised")
    return (integrand.integ()(x_max) - integrand.integ()(x_min)) / norm


def getLogLikelihood(Likelihood, x_min, x_max, num_points = 1000, degree=10) -> Polynomial:
    points = np.linspace(x_min, x_max, 1000)
    L_points = [Likelihood(p) for p in points]
    Log_points = np.log(L_points)
    Log_coeffs = np.polyfit(points, Log_points, 10)
    return Polynomial(Log_coeffs[::-1])


def getLikelihood(Log_coeffs, x_min, x_max, num_points = 1000, degree=10) -> Polynomial:
    Log_Likelihood = Polynomial(Log_coeffs[::-1])
    points = np.linspace(x_min, x_max, num_points)

    delta = (x_max - x_min) / num_points

    # create Log_Likelihood points with a constant factor removed to avoid overflow
    Log_points = [Log_Likelihood(p) for p in points]
    factor_max = max(Log_points)
    Log_points = [Log_point - factor_max for Log_point in Log_points]

    # create Likelihood points by exponentiation and get the coefficients
    L_points = np.exp(Log_points)
    L_points = L_points / (np.sum(L_points) * delta)
    L_coeffs = np.polyfit(points, L_points, 10)


    # create the polynomial object and normalise it
    return normalise(Polynomial(L_coeffs[::-1]), x_min, x_max)



def findExpectedBias(Log_coeffs, x_min, x_max, num_samples=1000):
    Likelihood = getLikelihood(Log_coeffs, x_min, x_max)
    Log_Likelihood = getLogLikelihood(Likelihood, x_min, x_max)

    # required functions
    U = Log_Likelihood.deriv()
    U_1 = U.deriv(1)
    U_2 = U.deriv(2)

    # Calculate the expected bias
    pdf = Likelihood
    i = -1 * expectedValue(U_1, pdf, x_min, x_max)
    Term1 = 2 * expectedValue(polymul(U, U_1)[0], pdf, x_min, x_max)
    Term2 = expectedValue(U_2, pdf, x_min, x_max)
    bias = (Term1 + Term2) / (2 * num_samples * (i ** 2))
    print(f"Expected Bias: {bias:.10f}")

    return


def findExpectedBias2(Log_coeffs, x_min, x_max, num_samples=1000):
    Likelihood = getLikelihood(Log_coeffs, x_min, x_max)
    Log_Likelihood = getLogLikelihood(Likelihood, x_min, x_max)

    # required functions
    U = Log_Likelihood.deriv()
    U_1 = U.deriv(1)
    U_cubed = polypow(U, 3)[0]

    # Calculate the expected bias
    pdf = Likelihood
    i = -1 * expectedValue(U_1, pdf, x_min, x_max)
    k_30 = expectedValue(U_cubed, pdf, x_min, x_max)
    k_11 = expectedValue(polymul(U, U_1)[0], pdf, x_min, x_max) + i * expectedValue(U, pdf, x_min, x_max)
    bias = -1 * (k_30 + k_11) / (2 * num_samples * (i ** 2))
    print(f"Expected Bias 2: {bias:.10f}")

    return



def main(json_file, True_Lag, num_samples=1000, output_folder=None, plot = False):
    Test_key = "0"
    L_points_zero = json_file[Test_key]["Likelihoods"]
    T_points_zero = json_file[Test_key]["Offsets"]

    avg_coefficients = averageFitCoefficients(json_file, num_samples)

    findExpectedBias(avg_coefficients, T_points_zero[0], T_points_zero[-1], num_samples)
    findExpectedBias2(avg_coefficients, T_points_zero[0], T_points_zero[-1], num_samples)

    if plot:
        points = np.linspace(T_points_zero[0], T_points_zero[-1], 1000)
        L_fitted = np.polyval(avg_coefficients, points)
        max_L_position = np.argmax(L_fitted)

        fig, ax = plt.subplots(1, 1, figsize=(20, 10))
        ax.plot(T_points_zero, L_points_zero, "o", label="Likelihood data points")
        ax.plot(points, L_fitted, label="Fitted Curve")

        ax.axvline(x=True_Lag, linestyle="--", color="black")
        ax.axvline(x=points[max_L_position], linestyle="--", color="red")
        ax.axvline(x=expected_bias, linestyle="--", color="green")

        plt.show()

    exit(0)




