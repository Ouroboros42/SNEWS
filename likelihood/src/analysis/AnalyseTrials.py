import numpy as np
import json
import matplotlib.pyplot as plt
import argparse

from helperMethods import Likelihood_Fits_And_Maxima as fits
from helperMethods import Raw_Data_Visualise as visualise
from helperMethods import Pull_Distribution as dist
from helperMethods import Helpers as helper


# ------------------- Methods -------------------

def MyFavouriteMethods(Likelihoods, TimeDifferences, True_Lag, draw, ax = None):
    ## Method 1: raw data
    ## Method 2: Curve fitting on raw data
    ## Method 3: Clean with moving average
    ## Method 4: Clean with noise filter

    # method 3
    clean_window_half_width = 5

    L_cleaned, T_cleaned = fits.cleanWithMovingAverage(Likelihoods, TimeDifferences, clean_window_half_width)
    res1 = fits.polynomialFit(L_cleaned, T_cleaned, ax = ax[0] if draw else None, plot_raw_data = True)

    # method 4
    window_half_width = 5
    noise_bound = 1  # in units of sigma

    L_cleaned, T_cleaned = fits.cleanWithNoiseFilter(Likelihoods, TimeDifferences, clean_window_half_width, noise_bound)
    res2 = fits.polynomialFit(L_cleaned, T_cleaned, ax = ax[1] if draw else None, plot_raw_data = True)

    return res1, res2


def readDataAndMakeEstimates(json_file, num_samples, True_Lag, draw_every = 1000, output_folder = None):
    estimates_1 = []
    estimates_2 = []
    fig = ax = None

    # Look in MyFavouriteMethods for the methods used
    method_ids = [3, 4]

    for i in range(num_samples):
        key = str(i)
        if key not in json_file:
            print(f"WARNING: Key {key} not found in the json file")
            continue

        Likelihoods = json_file[key]["Likelihoods"]
        TimeDifferences = json_file[key]["Offsets"]

        draw = (i % draw_every == 0)
        if draw:
            fig, ax = plt.subplots(1, 2, figsize=(20, 10))

        res1, res2 = MyFavouriteMethods(Likelihoods, TimeDifferences, True_Lag, draw, ax)
        estimates_1.append(res1)
        estimates_2.append(res2)

        if output_folder and draw:
            plt.savefig(output_folder / f"Trial_{i}.png")

    return estimates_1, estimates_2. method_ids


def unpackAndTestEstimates(estimates):
    values = []
    sigmas = []
    for estimate in estimates:
        values.append(estimate[0])
        assert estimate[1] <= estimate[0] <= estimate[2]
        sigmas.append((estimate[2] - estimate[1]))

    return values, sigmas


def VisualiseRawData(json_file, True_Lag):
    numTrials = 5
    for i in range(numTrials):
        key = str(i)
        Likelihoods = json_file[key]["Likelihoods"]
        TimeDifferences = json_file[key]["Offsets"]

        fig, ax = plt.subplots(1, 2, figsize=(20, 10))
        visualise.movingAverageAndNoiseFiltered(Likelihoods, TimeDifferences, ax)
        plt.show()


# ------------------- Main -------------------

def main(json_file):
    # read parameters
    True_Lag, detector1, detector2, inst, num_Trials, sweep_range = helper.readParameters(json_file)
    # make output folder
    out_folder = helper.makeOutputPath(inst, detector1, detector2, num_Trials, sweep_range)

    # visualise some plots (only for debugging, no need to save plots)
    VisualiseRawData(json_file, True_Lag)

    # analysis parameters
    num_samples_to_read = 10  # (max: numTrials)
    num_plots_to_draw = 10
    draw_every = num_samples_to_read // num_plots_to_draw

    # read data and make estimates
    estimates_1, estimates_2, method_ids = readDataAndMakeEstimates(json_file, num_samples_to_read, True_Lag,
                                                                    draw_every=draw_every,
                                                                    output_folder=out_folder
                                                                    )

    values_1, sigmas_1 = unpackAndTestEstimates(estimates_1)
    values_2, sigmas_2 = unpackAndTestEstimates(estimates_2)

    # print results
    helper.display(True_Lag, values_1, sigmas_1, values_2, sigmas_2, verbose=False)

    # make pull distributions
    names = [f"method_{id}" for id in method_ids]
    dist.makeDistribution(values_1, sigmas_1, True_Lag, name = names[0], out_folder=out_folder)
    dist.makeDistribution(values_2, sigmas_2, True_Lag, name = names[1], out_folder=out_folder)


# ------------------- Run -------------------




# parser = argparse.ArgumentParser("Likelihood_Analysis")
# parser.add_argument("source_file", type=str)
# args = parser.parse_args()
#
# data_file_path = args.source_file
#
# if __name__ == "__main__":
#     with open (data_file_path) as data_file:
#         print(f"\nReading data from {data_file_path}")
#         json_file = json.load(data_file)
#         main(json_file)








