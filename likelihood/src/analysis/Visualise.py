import json

import numpy as np
import matplotlib.pyplot as plt


def plotDataAndEvaluateAndPlotCurve(L_data, T_data, number_of_points_to_evaluate = 1000, degree = 9):
    coefficients = np.polyfit(T_data, L_data, degree)
    points = np.linspace(T_data[0], T_data[-1], number_of_points_to_evaluate)
    L_fitted = np.polyval(coefficients, points)

    fig, ax = plt.subplots(1, 1, figsize=(10, 10))
    ax.plot(T_data, L_data, "o", label="Likelihood data points")
    ax.plot(points, L_fitted, label="Fitted Curve")
    ax.set_xlabel("Time difference (s)")
    ax.set_ylabel("Likelihood")
    ax.legend()

    return ax



def main(jsonfile, num_plots):
    with open(jsonfile) as data_file:
        data = json.load(data_file)

    for i in range(num_plots):
        key = str(i)
        if key in data:
            Likelihoods = data[key]["Likelihoods"]
            TimeDifferences = data[key]["Offsets"]
            plotDataAndEvaluateAndPlotCurve(Likelihoods, TimeDifferences)

    plt.show()
    return


if __name__ == "__main__":
    data_file_path = "Trials/500_runs_SNOPvsSK_23-02-2024_01-11-31.json"
    main(data_file_path, 5)