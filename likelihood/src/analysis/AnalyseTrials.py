import numpy as np
import json
import matplotlib.pyplot as plt
import os

from helperMethods import Likelihood_Fits_And_Maxima as LFM
from helperMethods import Raw_Data_Visualise as RDV
from helperMethods import Pull_Distribution as PD


# just change the path to the file you want to analyse and runthis script
directory = "TrialsData/"
file_name = "20_runs_Sweep_Range_-0.100000_UID_222_SNOPvsSK____24-02-2024_22-14-17.json"
data_file_path = directory + file_name

def makeEstimates(json_file, number_of_trials, draw_every = 1000, out_folder = None):
    curve_estimates = []
    data_estimates = []
    True_Lag = json_file["True-Time-Diff"]

    for i in range(number_of_trials):
        key = str(i)
        if key in json_file:
            Likelihoods = json_file[key]["Likelihoods"]
            TimeDifferences = json_file[key]["Offsets"]

            fig = ax = draw = None
            if i % draw_every == 0:
                fig, ax = plt.subplots(1, 2, figsize=(20, 10))
                draw = True

            data_results = LFM.findDataMaximaAndErrors(Likelihoods, TimeDifferences,
                                                       ax= ax[0] if draw else None, True_T=True_Lag)
            curve_results = LFM.LikelihoodFitPolynomial(Likelihoods, TimeDifferences,
                                                        ax=ax[1] if draw else None, True_T=True_Lag)

            data_estimates.append(data_results)
            curve_estimates.append(curve_results)

            if out_folder and draw:
                plt.savefig(out_folder + f"Trial_{i}.png")

    return curve_estimates, data_estimates


def unpackAndTestEstimates(estimates):
    values = []
    sigmas = []
    for estimate in estimates:
        values.append(estimate[0])
        assert estimate[1] <= estimate[0] <= estimate[2]
        sigmas.append((estimate[2] - estimate[1]))

    return values, sigmas


def display(True_Lag, data_values, data_sigmas, curve_values, curve_sigmas):
    print("\n\n")
    print(f"True Lag: {True_Lag}")
    print(f"Average Lag from data: {np.mean(data_values)}")
    print(f"Average error from data: {np.mean(data_sigmas)}")
    print("\n")
    print(f"Average Lag from fitted curve: {np.mean(curve_values)}")
    print(f"Average error from fitted curve: {np.mean(curve_sigmas)}")
    print("\n\n")
    return


def displayVerbose(True_Lag, data_values, data_sigmas, curve_values, curve_sigmas):
    # format upto 3 decimal places and display the actual numbers
    print("\n\n")
    xx1 = [f"{x:.5f}" for x in data_values]
    xx2 = [f"{x:.5f}" for x in data_sigmas]
    print(f"data estimates: {xx1}")
    print(f"data sigmas: {xx2}")
    print("\n\n")
    xx1 = [f"{x:.5f}" for x in curve_values]
    xx2 = [f"{x:.5f}" for x in curve_sigmas]
    print(f"curve estimates: {xx1}")
    print(f"curve sigmas: {xx2}")
    print("\n\n")


def makeOutputPath(inst, detector1, detector2, numTrials, sweep_range):
    relative_path = f"\\TrialsResults\\ID_{inst}_{detector1}_vs_{detector2}_{numTrials}_Trials_Sweep_{sweep_range}"
    path = os.getcwd() + relative_path
    if not os.path.exists(path):
        os.mkdir(path)
    print(f"\n\nOutput folder: {path}\n\n")
    return path + "/"


def makeAppropriatePullDistribution(estimates, errors, True_Lag, name, out_folder):
    data_points = [(estimate - True_Lag) / error for estimate, error in zip(estimates, errors)]

    mean = np.mean(data_points)
    std = np.std(data_points)

    # These are just guesses
    hist_range = (mean - 5 * std, mean + 5 * std)
    bin_width = hist_range[1] - hist_range[0] / len(data_points)

    PD.createDistribution(data_points, True_Lag, hist_range, bin_width, name, out_folder)

# ------------------- Main -------------------


def main(data):
    # read important values
    True_Lag = data["True-Time-Diff"]
    inst = data["inst"]
    detector1 = data["detector1"]
    detector2 = data["detector2"]
    numTrials = data["num-Trials"]
    sweep_range = data["sweep-range"]

    # Visualise first few plots (close the figures to see the next one)
    RDV.plotFirstNTrialsWithCurveFits_And_ZoomedInMaximas(data, 0, True_Lag);

    # create folder to save the plots
    out_folder = makeOutputPath(inst, detector1, detector2, numTrials, sweep_range)

    # find peak and error bounds from curve and from actual data
    num_samples_draw = 2
    draw_every = numTrials // num_samples_draw

    curve_estimates, data_estimates = makeEstimates(data, numTrials, draw_every=draw_every, out_folder=out_folder)
    curve_values, curve_sigmas = unpackAndTestEstimates(curve_estimates)
    data_values, data_sigmas = unpackAndTestEstimates(data_estimates)

    # print results
    displayVerbose(True_Lag, data_values, data_sigmas, curve_values, curve_sigmas)
    display(True_Lag, data_values, data_sigmas, curve_values, curve_sigmas)

    # decide hist_range, bin-width and make pull distribution
    makeAppropriatePullDistribution(curve_values, curve_sigmas, True_Lag, "Curve Estimates", out_folder)
    makeAppropriatePullDistribution(data_values, data_sigmas, True_Lag, "Data Estimates", out_folder)










if __name__ == "__main__":
    with open (data_file_path) as data_file:
        data = json.load(data_file)
        main(data)








