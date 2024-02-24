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


def plotLandTaroundTrueTAndSeeWhereActualDataMaximaOccur(L_data, T_data, True_T, bound):
    zoomed_in = []
    zoomed_in_L = []
    for i, T in enumerate(T_data):
        if bound > abs(T - True_T):
            zoomed_in.append(T)
            zoomed_in_L.append(L_data[i])

    assert len(zoomed_in) == len(zoomed_in_L)

    plt.figure()
    plt.plot(zoomed_in, zoomed_in_L, "o", label="Zoomed in")
    plt.axvline(x=True_T, linestyle="--", label="True T", color="black")

    max1 = T_data[np.argmax(L_data)]
    print(f"Max 1: {max1}")
    L_copy = [L_data[i] for i in range(len(L_data)) if i != np.argmax(L_data)]
    assert len(L_copy) == len(L_data) - 1
    max2 = T_data[np.argmax(L_copy)]
    print(f"Max 2: {max2}")

    plt.axvline(x=max1, linestyle="--", label="Max 1", color="red")
    plt.axvline(x=max2, linestyle="--", label="Max 2", color="green")
    plt.legend()




def main(jsonfile, num_plots):
    with open(jsonfile) as data_file:
        data = json.load(data_file)

    for i in range(num_plots):

        Likelihoods = data["Likelihood"]
        TimeDifferences = data["Time-Difference"]

        plotDataAndEvaluateAndPlotCurve(Likelihoods, TimeDifferences)
        plotLandTaroundTrueTAndSeeWhereActualDataMaximaOccur(Likelihoods, TimeDifferences, data["True-Time-Difference"], 0.01)

        # key = str(i)
        # if key in data:
        #     Likelihoods = data[key]["Likelihoods"]
        #     TimeDifferences = data[key]["Offsets"]
        #
        #     plotDataAndEvaluateAndPlotCurve(Likelihoods, TimeDifferences)
        #     plotLandTaroundTrueTAndSeeWhereActualDataMaximaOccur(Likelihoods, TimeDifferences, data["True-Time-Diff"], 0.01)

    plt.show()
    return


if __name__ == "__main__":
    data_file_path = "ldist_SNOP-vs-SK_src=121_t=24-02-2024_14-36-34.json"
    main(data_file_path, 1)