import numpy as np
import matplotlib.pyplot as plt
import json
import copy


def cleanWithMovingAverage(L_data, T_data, window_half_width):
    numPoints = len(L_data)
    L_cleaned = []
    T_cleaned = []
    for i in range(window_half_width, numPoints - window_half_width):
        new_point = np.mean(L_data[i - window_half_width: i + window_half_width])
        L_cleaned.append(new_point)
        T_cleaned.append(T_data[i])

    return L_cleaned, T_cleaned


def cleanWithNoiseFilter(L_data, T_data, window_half_width, noise_bound_in_sigma = 1.0):
    # clean data with moving average before
    L_cleaned, T_cleaned = cleanWithMovingAverage(L_data, T_data, window_half_width)

    L_data = L_data[window_half_width: -window_half_width]
    T_data = T_data[window_half_width: -window_half_width]

    deviation = [L_data[i] - L_cleaned[i] for i in range(len(L_cleaned))]
    sigma = np.std(deviation)
    bound = noise_bound_in_sigma * sigma

    Likelihoods = []
    Time_Diffs = []
    for i, dev in enumerate(deviation):
        if abs(dev) < bound:
            Likelihoods.append(L_data[i])
            Time_Diffs.append(T_data[i])

    return Likelihoods, Time_Diffs


def findRawDataMaximaAndError(L_data, T_data, True_Lag, error_bound = 0.5, ax: plt.Axes = None):
    max_L_position = np.argmax(L_data)
    max_L_value = L_data[max_L_position]
    best_Lag = T_data[max_L_position]

    Avg_window = 3

    # extreme horrible edge case (should not happen)
    i = max(max_L_position - 1, 0)
    while i >= Avg_window:
        mean = np.mean(L_data[i - Avg_window : i])
        if mean < max_L_value - error_bound:
            break
        i -= 1

    j = min(max_L_position + 1, len(L_data) - 1)
    while j < len(L_data) - Avg_window:
        mean = np.mean(L_data[j : j + Avg_window])
        if mean < max_L_value - error_bound:
            break
        j += 1

    assert i <= max_L_position <= j

    if ax:
        ax.plot(T_data, L_data, "o", label="Likelihood data points")
        ax.axvline(x=best_Lag, linestyle="--", label="Best Lag")
        ax.axvline(x=T_data[i], linestyle="--", label="Error Bound")
        ax.axvline(x=T_data[j], linestyle="--")
        if True_Lag:
            ax.axvline(x=True_Lag, linestyle="--", label="True T", color="black")
        ax.set_xlabel("Time difference (s)")
        ax.set_ylabel("Likelihood")
        ax.legend()

    return best_Lag, T_data[i], T_data[j]


def findErrorOnCurveRecursively(coefficients, points, error_bound, max_recurse: int = 5):
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


def polynomialFit(L_data, T_data, True_Lag,
                  number_of_points_to_evaluate = 1000,
                  degree = 12,
                  error_bound = 0.5,
                  ax: plt.Axes = None,
                  plot_raw_data = False
                  ):

    coefficients = np.polyfit(T_data, L_data, degree)
    offset_points = np.linspace(T_data[0], T_data[-1], number_of_points_to_evaluate)
    L_fitted = np.polyval(coefficients, offset_points)

    best, err1, err2 = findErrorOnCurveRecursively(coefficients, copy.deepcopy(offset_points), error_bound)
    assert err1 <= best <= err2

    if ax:
        ax.plot(offset_points, L_fitted, label="Fitted Curve")
        ax.axvline(x=best, linestyle="--", label="Best Lag")
        ax.axvline(x=err1, linestyle="--")
        ax.axvline(x=err2, linestyle="--")
        ax.axvline(x=True_Lag, linestyle="--", label="True T", color="black")

        if plot_raw_data:
            ax.plot(T_data, L_data, "o", label="Likelihood data points")

        ax.set_xlabel("Time difference (s)")
        ax.set_ylabel("Likelihood")
        ax.legend()

    return best, err1, err2