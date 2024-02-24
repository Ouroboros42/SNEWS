import numpy as np
import json
import matplotlib.pyplot as plt
from likelihood.src.analysis.helperMethods import Pull_Distribution as pd


def makeEstimates(json_file, number_of_trials = 1000, draw_every = 100):
    curve_estimates = []
    data_estimates = []

    for i in range(number_of_trials):
        key = str(i)
        if key in json_file:
            Likelihoods = json_file[key]["Likelihoods"]
            TimeDifferences = json_file[key]["Offsets"]

            #Likelihoods, TimeDifferences = returnCleanData(json_file[key]["Likelihoods"], json_file[key]["Offsets"])

            fig = ax = draw = None
            # can choose to display every 100th sample or so
            if i % draw_every == 0:
                fig, ax = plt.subplots(1, 2, figsize=(20, 10))
                draw = True

            data_results = findDataMaximaAndErrors(Likelihoods, TimeDifferences, ax= ax[0] if draw else None)
            data_estimates.append(data_results)

            curve_results = LikelihoodFitPolynomial(Likelihoods, TimeDifferences, ax=ax[1] if draw else None)
            curve_estimates.append(curve_results)

    return curve_estimates, data_estimates


def unpackAndTestEstimates(estimates):
    values = []
    sigmas = []
    for estimate in estimates:
        values.append(estimate[0])
        assert estimate[1] <= estimate[0] <= estimate[2]
        sigmas.append((estimate[2] - estimate[1]))

    return values, sigmas


def printResults(True_Lag, data_values, data_sigmas, curve_values, curve_sigmas):
    print("\n\n")
    print(f"True Lag: {True_Lag}")
    print(f"Average Lag from data: {np.mean(data_values)}")
    print(f"Average error from data: {np.mean(data_sigmas)}")
    print("\n")
    print(f"Average Lag from fitted curve: {np.mean(curve_values)}")
    print(f"Average error from fitted curve: {np.mean(curve_sigmas)}")
    print("\n\n")
    return

def printResultsAll(True_Lag, data_values, data_sigmas, curve_values, curve_sigmas):
    # format upto 3 decimal places and display the actual numbers
    xx1 = [f"{x:.5f}" for x in data_values]
    xx2 = [f"{x:.5f}" for x in data_sigmas]
    print(f"data estimates: {xx1}")
    print(f"data sigmas: {xx2}")
    xx1 = [f"{x:.5f}" for x in curve_values]
    xx2 = [f"{x:.5f}" for x in curve_sigmas]
    print(f"curve estimates: {xx1}")
    print(f"curve sigmas: {xx2}")

# ------------------- Main -------------------


def main():
    # read in data
    data_file_path = "TrialsData/1000_runs_Sweep_Range_-0.100000ICvsSK_24-02-2024_16-09-50_ID_222.json"
    with open(data_file_path) as data_file:
        data = json.load(data_file)

    # read true value
    True_Lag = data["True-Time-Diff"]

    # find peak and error bounds from curve and from actual data
    curve_estimates, data_estimates = makeEstimates(data, draw_every=100)
    curve_values, curve_sigmas = unpackAndTestEstimates(curve_estimates)
    data_values, data_sigmas = unpackAndTestEstimates(data_estimates)

    # print results
    printResults(True_Lag, data_values, data_sigmas, curve_values, curve_sigmas)
    # print actual values upto 3 decimal places
    printResultsAll(True_Lag, data_values, data_sigmas, curve_values, curve_sigmas)


    # make pull distribution (I need suggestions for bin width and range)
    hist_range = (-2, 2)
    bin_width = 0.1
    mean1, std1 = pd.createDistribution(curve_values, curve_sigmas, True_Lag, hist_range, bin_width, name="Curve Estimates")
    mean2, std2 = pd.createDistribution(data_values, data_sigmas, True_Lag, hist_range, bin_width, name="Data Estimates")












if __name__ == "__main__":
    main()
    plt.show()