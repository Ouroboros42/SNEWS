import json
import numpy as np
import matplotlib.pyplot as plt
from typing import List

import Likelihood_Fits_And_Maxima as fits

def plotDataAndCurve(L_data, T_data, ax, number_of_points_to_evaluate = 1000, degree = 9):
    coefficients = np.polyfit(T_data, L_data, degree)
    points = np.linspace(T_data[0], T_data[-1], number_of_points_to_evaluate)
    L_fitted = np.polyval(coefficients, points)

    ax.plot(T_data, L_data, "o", label="Likelihood data points")
    ax.plot(points, L_fitted, label="Fitted Curve")
    ax.set_xlabel("Time difference (s)")
    ax.set_ylabel("Likelihood")
    ax.legend()
    plt.show()


def plotDataZoomedAroundTrueValue(L_data, T_data, True_T, bound, ax):
    upper_bound = True_T + bound
    lower_bound = True_T - bound

    Time_zoomed = []
    L_zoomed = []

    for i, data in enumerate(T_data):
        if lower_bound <= data <= upper_bound:
            Time_zoomed.append(data)
            L_zoomed.append(L_data[i])

    highest_value_index = np.argmax(L_zoomed)
    second_highest_value_index = np.argmax([L for i, L in enumerate(L_zoomed) if i != highest_value_index])

    best_offset = Time_zoomed[highest_value_index]
    second_best_offset = Time_zoomed[second_highest_value_index]

    ax.plot(Time_zoomed, L_zoomed, "o")
    ax.axvline(x=True_T, linestyle="--", label=f"expected: {True_T}", color="black")
    ax.axvline(x=best_offset, linestyle="--", label=f"max 1: {best_offset}", color="red")
    ax.axvline(x=second_best_offset, linestyle="--", label=f"max 2: {second_best_offset}", color="green")
    ax.legend()
    plt.show()


def movingAverageAndNoiseFiltered(L_data, T_data, ax: List[plt.Axes], window_half_width = 5, noise_bound = 1):

    L_cleaned, T_cleaned = fits.cleanWithMovingAverage(L_data, T_data, window_half_width)
    ax[0].plot(T_cleaned, L_cleaned, "o", label="Moving Average")
    ax[0].plot(T_data, L_data, "o", label="Actual Data")
    ax[0].set_xlabel("Time difference (s)")
    ax[0].set_ylabel("Likelihood")
    ax[0].legend()

    L_final, T_final = fits.cleanWithNoiseFilter(L_data, T_data, averaging_window_half_width, noise_bound)
    ax[1].plot(T_final, L_final, "o", label="Noise Filtered")
    ax[1].set_xlabel("Time difference (s)")
    ax[1].set_ylabel("Likelihood")
    ax[1].legend()
    plt.show()




