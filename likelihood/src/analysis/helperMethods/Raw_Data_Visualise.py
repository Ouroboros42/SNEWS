import json
import numpy as np
import matplotlib.pyplot as plt

def plot_data_and_evaluate_and_plot_polynomial_curve(L_data, T_data, ax, number_of_points_to_evaluate = 1000, degree = 9):
    coefficients = np.polyfit(T_data, L_data, degree)
    points = np.linspace(T_data[0], T_data[-1], number_of_points_to_evaluate)
    L_fitted = np.polyval(coefficients, points)

    ax.plot(T_data, L_data, "o", label="Likelihood data points")
    ax.plot(points, L_fitted, label="Fitted Curve")
    ax.set_xlabel("Time difference (s)")
    ax.set_ylabel("Likelihood")
    ax.legend()


def plot_data_zoomed_in_around_True_T(L_data, T_data, True_T, bound, ax):
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


def plotFirstNTrialsWithCurveFits_And_ZoomedInMaximas(data, num_plots, True_T):
    for i in range(num_plots):
        key = str(i)
        if key in data:
            Likelihoods = data[key]["Likelihoods"]
            TimeDifferences = data[key]["Offsets"]

            fig, ax = plt.subplots(2, 1, figsize=(8, 8))

            plot_data_and_evaluate_and_plot_polynomial_curve(Likelihoods, TimeDifferences, ax[0])
            plot_data_zoomed_in_around_True_T(Likelihoods, TimeDifferences, True_T, 0.01, ax[1])

    plt.show()
    return
