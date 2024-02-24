import numpy as np
import matplotlib.pyplot as plt
import json
import copy

def findDataMaximaAndErrors(L_data, T_data, error_bound = 0.5, ax=None, True_T = None):
    max_L_position = np.argmax(L_data)
    max_L_value = L_data[max_L_position]
    best_Lag = T_data[max_L_position]

    pos_left = max_L_position - 1
    while L_data[pos_left] > max_L_value - error_bound and pos_left > 0:
        pos_left -= 1

    pos_right = max_L_position + 1
    while L_data[pos_right] > max_L_value - error_bound and pos_right < len(L_data):
        pos_right += 1

    if ax:
        ax.plot(T_data, L_data, "o", label="Likelihood data points")
        ax.axvline(x=best_Lag, linestyle="--", label="Best Lag")
        ax.axvline(x=T_data[pos_left], linestyle="--", label="Error Bound")
        ax.axvline(x=T_data[pos_right], linestyle="--")
        if True_T:
            ax.axvline(x=True_T, linestyle="--", label="True T", color="black")
        ax.set_xlabel("Time difference (s)")
        ax.set_ylabel("Likelihood")
        ax.legend()

    return best_Lag, T_data[pos_left], T_data[pos_right]


def findErrorOnCurveRecursively(coefficients, points, error_bound, max_recurse):
    L_fitted = np.polyval(coefficients, points)
    max_L_position = np.argmax(L_fitted)
    max_L_value = L_fitted[max_L_position]
    best_Lag = points[max_L_position]

    points_within_error_bound = [p for i, p in enumerate(points) if (max_L_value - L_fitted[i]) <= error_bound]

    if max_recurse == 0:
        return best_Lag, points_within_error_bound[0], points_within_error_bound[-1]

    if len(points_within_error_bound) < 2:
        points = np.linspace(points[max_L_position - 1], points[max_L_position + 1], 20)
        return findErrorOnCurveRecursively(coefficients, points, error_bound, max_recurse - 1)
    else:
        return best_Lag, points_within_error_bound[0], points_within_error_bound[-1]


def LikelihoodFitPolynomial(L_data, T_data, number_of_points_to_evaluate=1000, degree=9, error_bound = 0.5, ax=None, True_T = None):
    coefficients = np.polyfit(T_data, L_data, degree)
    offset_points = np.linspace(T_data[0], T_data[-1], number_of_points_to_evaluate)
    L_fitted = np.polyval(coefficients, offset_points)

    best_Lag, error_left, error_right = findErrorOnCurveRecursively(coefficients, copy.deepcopy(offset_points), error_bound, 5)

    if ax:
        ax.plot(offset_points, L_fitted, label="Fitted Curve")
        ax.axvline(x=best_Lag, linestyle="--", label="Best Lag")
        ax.axvline(x=error_left, linestyle="--", label="Error Bound")
        ax.axvline(x=error_right, linestyle="--")
        if True_T:
            ax.axvline(x=True_T, linestyle="--", label="True T", color="black")
        ax.set_xlabel("Time difference (s)")
        ax.set_ylabel("Likelihood")
        ax.legend()

    return best_Lag, error_left, error_right