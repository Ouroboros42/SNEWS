import numpy as np
from numpy.polynomial import Polynomial
from numpy.polynomial.polynomial import polypow
from numpy.polynomial.polynomial import polymul

from helperMethods.Likelihood_Fits_And_Maxima import polynomialFit

import json
import matplotlib.pyplot as plt

def fitCoefficients(json_file, sample_i, degree):
    key = str(sample_i)
    L_data = json_file[key]["Likelihoods"]
    T_data = json_file[key]["Offsets"]
    return Polynomial.fit(T_data, L_data, degree)

def allFitCoefficients(json_file, num_samples, degree=10):
    coefficients = np.ndarray(num_samples, Polynomial)
    for i in range(num_samples):
        coefficients[i] = fitCoefficients(json_file, i, degree)
    return coefficients

vderiv = np.vectorize(Polynomial.deriv)

def main(json_file, true_val, values, True_Lag, num_samples=1000, output_folder=None, plot = False):
    coeffs = allFitCoefficients(json_file, num_samples)

    n = len(coeffs)

    # Method 1

    U = vderiv(coeffs)
    U1 = vderiv(U)
    U2 = vderiv(U1)

    i = - np.mean(U1)

    denom = 2 * n * i * i 

    B1 = - (np.mean(U * U1) + np.mean(U) * i + np.mean(U ** 3))

    B2 = np.mean(2 * U * U1 + U2)

    bias1 = B1(values) / denom(values)
    bias2 = B2(values) / denom(values)

    # Method 2

    avg_coeffs = np.mean(coeffs)

    k = avg_coeffs.deriv().deriv().deriv()

    bias3 = - 1 / k(values) / 2

    return bias1, bias2, bias3

    # fig, ax = plt.subplots(1, 1, figsize=(20, 10))

    # ax.plot(T_points_zero, L_points_zero, "o", label="Example points")
    # ax.plot(points, L_fitted, label="Average Curve")
    
    # plt.legend()

    # plt.show()

    # findExpectedBias(avg_coefficients, T_points_zero[0], T_points_zero[-1], num_samples)
    # findExpectedBias2(avg_coefficients, T_points_zero[0], T_points_zero[-1], num_samples)

    # if plot:
    #     points = np.linspace(T_points_zero[0], T_points_zero[-1], 1000)
    #     L_fitted = np.polyval(avg_coefficients, points)
    #     max_L_position = np.argmax(L_fitted)

    #     fig, ax = plt.subplots(1, 1, figsize=(20, 10))
    #     ax.plot(T_points_zero, L_points_zero, "o", label="Likelihood data points")
    #     ax.plot(points, L_fitted, label="Fitted Curve")

    #     ax.axvline(x=True_Lag, linestyle="--", color="black")
    #     ax.axvline(x=points[max_L_position], linestyle="--", color="red")
    #     ax.axvline(x=expected_bias, linestyle="--", color="green")

    #     plt.show()

    # exit(0)




