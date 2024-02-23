import copy
import numpy as np
import json
import matplotlib.pyplot as plt


def findDataMaximaAndErrors(L_data, T_data, error_bound = 0.5, ax=None):
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


def LikelihoodFitPolynomial(L_data, T_data, number_of_points_to_evaluate=1000, degree=9, error_bound = 0.5, ax=None):
    coefficients = np.polyfit(T_data, L_data, degree)
    offset_points = np.linspace(T_data[0], T_data[-1], number_of_points_to_evaluate)
    L_fitted = np.polyval(coefficients, offset_points)

    best_Lag, error_left, error_right = findErrorOnCurveRecursively(coefficients, copy.deepcopy(offset_points), error_bound, 5)

    if  ax:
        ax.plot(offset_points, L_fitted, label="Fitted Curve")
        ax.axvline(x=best_Lag, linestyle="--", label="Best Lag")
        ax.axvline(x=error_left, linestyle="--", label="Error Bound")
        ax.axvline(x=error_right, linestyle="--")
        ax.set_xlabel("Time difference (s)")
        ax.set_ylabel("Likelihood")
        ax.legend()

    return best_Lag, error_left, error_right


def makeEstimates(json_file, number_of_trials = 1000):

    curve_estimates = []
    data_estimates = []

    for i in range(number_of_trials):
        key = str(i)
        if key in json_file:
            Likelihoods = json_file[key]["Likelihoods"]
            TimeDifferences = json_file[key]["Offsets"]

            # display every 100th Trial
            fig = ax = draw = None
            if i % 100 == 0:
                draw = True
                fig, ax = plt.subplots(2, 1, figsize=(10, 10))

            data_results = findDataMaximaAndErrors(Likelihoods, TimeDifferences, ax= ax[0] if draw else None)
            data_estimates.append(data_results)

            curve_results = LikelihoodFitPolynomial(Likelihoods, TimeDifferences, ax=ax[1] if draw else None)
            curve_estimates.append(curve_results)

    return curve_estimates, data_estimates


# ------------------- Main -------------------

def main():
    data_file_path = "Trials/500_runs_SNOPvsSK_23-02-2024_01-11-31.json"
    with open(data_file_path) as data_file:
        data = json.load(data_file)

    curve_estimates, data_estimates = makeEstimates(data)

    print(f"True Lag: {data['True-Time-Diff']}")
    print(f"Fitted curve estimates: {curve_estimates[0]}")
    print(f"Data estimates: {data_estimates[0]}")




if __name__ == "__main__":
    main()
    plt.show()