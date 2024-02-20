import numpy as np
import json
import matplotlib.pyplot as plt
import argparse


def doLikelihoodFittingAndReturnCurveMaxima(L_data, T_data, number_of_points_to_evaluate=1000, degree=9):
    coefficients = np.polyfit(T_data, L_data, degree)
    points = np.linspace(T_data[0], T_data[-1], number_of_points_to_evaluate)
    L_fitted = np.polyval(coefficients, points)
    maxima = points[np.argmax(L_fitted)]
    return maxima, L_fitted, points


def ExtractDataFromJsonAndEstimateLag(json_file):
    name = json_file["UID"]["Name"]
    start = json_file["UID"]["Start"]
    end = json_file["UID"]["End"]
    step = json_file["UID"]["Step"]
    True_Lag = json_file["UID"]["True-Time-Difference"]

    actual_data_estimates = []
    fitted_curve_estimates = []
    fig, ax = plt.subplots(1, 1, figsize=(10, 10))

    for i in range(start, end, step):
        key = str(i)
        if key in json_file:
            Likelihoods = json_file[key]["Likelihoods"]
            TimeDifferences = json_file[key]["Offsets"]

            estimate, L_fitted, points = doLikelihoodFittingAndReturnCurveMaxima(Likelihoods, TimeDifferences)

            actual_data_estimates.append(TimeDifferences[np.argmax(Likelihoods)])
            fitted_curve_estimates.append(estimate)

            ax.plot(points, L_fitted, label=f"Fit when {name} = {key}")
            ax.axvline(x=estimate, linestyle="--")

    # plot the expected line
    ax.set_xlabel("Time difference (s)")
    ax.set_ylabel("Likelihood")
    ax.axvline(x=True_Lag, color="black", linestyle="--", label="Expected Maxima")
    ax.legend()

    return actual_data_estimates, fitted_curve_estimates, ax


# ------------------- Main -------------------

def main(data_file_path = None):
    data_file_path = "Dummy/dummy20-02-2024_14-33-24.json"

    with open(data_file_path) as data_file:
        data = json.load(data_file)

    data_estimates, curve_estimates, ax = ExtractDataFromJsonAndEstimateLag(data)

    print(f"True Lag: {data['UID']['True-Time-Difference']}")
    print(f"Average Lag from data: {np.mean(data_estimates)}")
    print(f"Average Lag from fitted curve: {np.mean(curve_estimates)}")
    print("\n\n")
    print(f"actual data estimates: {data_estimates}")
    print(f"fitted curve estimates: {curve_estimates}")

    ax.set_xlim([-0.05, 0.05])
    plt.show()

# parser = argparse.ArgumentParser("Likelihood_Analysis")
# parser.add_argument("source_file", type=str)
# args = parser.parse_args()
# data_file_path = args.source_file


if __name__ == "__main__":
    main()