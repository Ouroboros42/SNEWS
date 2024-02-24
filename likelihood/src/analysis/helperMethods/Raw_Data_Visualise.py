import json
import numpy as np
import matplotlib.pyplot as plt

def plotData_And_EvaluateAndPlotPolynomialCurve(L_data, T_data, number_of_points_to_evaluate = 1000, degree = 9):
    coefficients = np.polyfit(T_data, L_data, degree)
    points = np.linspace(T_data[0], T_data[-1], number_of_points_to_evaluate)
    L_fitted = np.polyval(coefficients, points)

    ax.plot(T_data, L_data, "o", label="Likelihood data points")
    ax.plot(points, L_fitted, label="Fitted Curve")
    ax.set_xlabel("Time difference (s)")
    ax.set_ylabel("Likelihood")
    ax.legend()


def plotLandTaroundTrueTAndSeeWhereActualDataMaximaOccur(L_data, T_data, True_T, bound, ax):
    zoomed_in = []
    zoomed_in_L = []
    for i, T in enumerate(T_data):
        if bound > abs(T - True_T):
            zoomed_in.append(T)
            zoomed_in_L.append(L_data[i])

    assert len(zoomed_in) == len(zoomed_in_L)


    # find the 2 highest values of L_data
    max1 = T_data[np.argmax(L_data)]
    L_copy = [L_data[i] for i in range(len(L_data)) if i != np.argmax(L_data)]
    max2 = T_data[np.argmax(L_copy)]

    ax.plot(zoomed_in, zoomed_in_L, "o", label="Zoomed in")
    ax.axvline(x=True_T, linestyle="--", label=f"True t = {True_T}", color="black")
    ax.axvline(x=max1, linestyle="--", label=f"Max 1 = {max1}", color="red")
    ax.axvline(x=max2, linestyle="--", label=f"Max 2 = {max2}", color="green")
    ax.legend()


def plotFirstNTrialsWithCurveFits_And_ZoomedInMaximas(data, num_plots):
    for i in range(num_plots):
        key = str(i)
        if key in data:

            fig, ax = plt.subplots(1, 2, figsize=(10, 5))

            Likelihoods = data[key]["Likelihoods"]
            TimeDifferences = data[key]["Offsets"]

            plotDataAndEvaluateAndPlotCurve(Likelihoods, TimeDifferences, ax[0])
            plotLandTaroundTrueTAndSeeWhereActualDataMaximaOccur(Likelihoods, TimeDifferences,
                                                                 data["True-Time-Diff"],0.01, ax[1])


    plt.show()
    return
