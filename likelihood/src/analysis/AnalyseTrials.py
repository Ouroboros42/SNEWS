import numpy as np
import json
import matplotlib.pyplot as plt
import argparse
import os
import pathlib
from typing import List, Tuple

from helperMethods import Likelihood_Fits_And_Maxima as fits
from helperMethods import Raw_Data_Visualise as visualise
from helperMethods import Pull_Distribution as dist
from helperMethods import Helpers as helper


# ------------------- Methods -------------------

def MyFavouriteMethods(Likelihoods, TimeDifferences, True_Lag, methods_ids, draw, ax = None):
    ## Method 1: raw data
    ## Method 2: Curve fitting on raw data
    ## Method 3: Clean with moving average
    ## Method 4: Clean with noise filter
    ## Method 5: clean with noise filter and further take moving average

    # The Function accepts an 2-Tuple of method_ids and plots the analysis for both side by side

    results = []
    i = 0;

    if 1 in methods_ids:
        res = fits.findRawDataMaximaAndError(Likelihoods, TimeDifferences, True_Lag, ax = ax[i] if draw else None)
        ax[i].set_title("Method 1")
        results.append(res)
        i += 1

    if 2 in methods_ids:
        res = fits.polynomialFit(Likelihoods, TimeDifferences, True_Lag, ax = ax[i] if draw else None, error_bound=1)
        ax[i].set_title("Method 2")
        results.append(res)
        i += 1

    if 3 in methods_ids:
        L_smoothed, T_smoothed = fits.cleanWithMovingAverage(Likelihoods, TimeDifferences, 4)
        res = fits.polynomialFit(L_smoothed, T_smoothed, True_Lag, ax = ax[i] if draw else None)
        ax[i].set_title("Method 3")
        results.append(res)
        i += 1

    if 4 in methods_ids:
        L_smoothed, T_smoothed = fits.cleanWithNoiseFilter(Likelihoods, TimeDifferences, 4, 1)
        res = fits.polynomialFit(L_smoothed, T_smoothed, True_Lag, ax = ax[i] if draw else None)
        ax[i].set_title("Method 4")
        results.append(res)
        i += 1

    if 5 in methods_ids:
        L_smoothed, T_smoothed = fits.cleanWithNoiseFilter(Likelihoods, TimeDifferences, 4, 1)
        L_smoothed_again, T_smoothed_again = fits.cleanWithMovingAverage(L_smoothed, T_smoothed, 4)

        res = fits.polynomialFit(L_smoothed_again, T_smoothed_again, True_Lag, ax = ax[i] if draw else None, plot_raw_data = True)
        ax[i].set_title("Method 5")
        results.append(res)
        i += 1

    return results[0], results[1]


def readDataAndMakeEstimates(json_file, num_samples, True_Lag, method_ids=[1, 2], draw_every=1000, output_folder=None):
    estimates_1 = []
    estimates_2 = []
    fig = ax = None

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

        res1, res2 = MyFavouriteMethods(Likelihoods, TimeDifferences, True_Lag, method_ids, draw, ax)
        estimates_1.append(res1)
        estimates_2.append(res2)

        if output_folder and draw:
            plt.savefig(output_folder / f"Trial_{i}.png")

    return estimates_1, estimates_2


def unpackAndTestEstimates(estimates):
    values: List[float] = []
    bounds: List[Tuple(float, float)] = []
    for estimate in estimates:
        assert estimate[1] <= estimate[0] <= estimate[2]
        values.append(estimate[0])
        bounds.append((estimate[1], estimate[2]))

    return values, bounds

def boundsToSigmas(bounds, values):
    sigmas_1 = [bounds[i][1] - values[i] for i in range(len(values))]
    sigmas_2 = [values[i] - bounds[i][0] for i in range(len(values))]

    assert all([sigma >= 0 for sigma in sigmas_1])
    assert all([sigma >= 0 for sigma in sigmas_2])

    sigma_estimate = [(sigma_1 + sigma_2)/2 for sigma_1, sigma_2 in zip(sigmas_1, sigmas_2)]

    return sigma_estimate

def VisualiseRawData(json_file, True_Lag):
    numTrials = 0 # set to a positive number for visual debugging
    for i in range(numTrials):
        key = str(i)
        Likelihoods = json_file[key]["Likelihoods"]
        TimeDifferences = json_file[key]["Offsets"]

        fig, ax = plt.subplots(1, 2, figsize=(20, 10))
        visualise.movingAverageAndNoiseFiltered(Likelihoods, TimeDifferences, True_Lag, ax)
        plt.show()
    # exit after visualising
    if numTrials:
        exit(0)


def makeOutputPath(detector1, detector2, numTrials, sweep_range):
    relative_path = f"TrialsResults/{detector1}_vs_{detector2}_{numTrials}_Trials_Sweep_{sweep_range}"
    base_path = pathlib.Path(__file__).parent.resolve()
    print(base_path)
    path = base_path / relative_path
    if not os.path.exists(path):
        os.mkdir(path)
    print(f"\n\nOutput folder: {path}")
    return path

# ------------------- Main -------------------

def main(json_file):
    # read parameters
    True_Lag, detector1, detector2, num_Trials, sweep_range = helper.readParameters(json_file)
    # make output folder
    out_folder = makeOutputPath(detector1, detector2, num_Trials, sweep_range)

    # visualise some plots (only for debugging, no need to save plots)
    VisualiseRawData(json_file, True_Lag)

    # analysis parameters
    num_samples_to_read = num_Trials # (max: numTrials)
    print(f"\nReading {num_samples_to_read} samples")
    num_plots_to_draw = 5
    draw_every = (num_samples_to_read // num_plots_to_draw) if num_plots_to_draw > 0 else num_Trials + 1

    # Look in MyFavouriteMethods above for the methods used. Pick Any 2 for comparison
    method_ids = [2, 3]

    # read data and make estimates
    estimates_1, estimates_2 = readDataAndMakeEstimates(json_file, num_samples_to_read, True_Lag,
                                                                    method_ids=method_ids,
                                                                    draw_every=draw_every,
                                                                    output_folder=out_folder
                                                                    )

    values_1, bounds_1 = unpackAndTestEstimates(estimates_1)
    values_2, bounds_2 = unpackAndTestEstimates(estimates_2)
    sigmas_1 = boundsToSigmas(bounds_1, values_1)
    sigmas_2 = boundsToSigmas(bounds_2, values_2)

    # print results
    helper.display(True_Lag, values_1, bounds_1, sigmas_1, values_2, bounds_2, sigmas_2, method_ids)

    # make pull distributions
    names = [f"method_{id}" for id in method_ids]
    dist.makeDistribution(values_1, sigmas_1, True_Lag, name = names[0], out_folder=out_folder)
    dist.makeDistribution(values_2, sigmas_2, True_Lag, name = names[1], out_folder=out_folder)


# ------------------- Run -------------------




parser = argparse.ArgumentParser("Likelihood_Analysis")
parser.add_argument("source_file", type=str)
args = parser.parse_args()

data_file_path = args.source_file

if __name__ == "__main__":
    with open (data_file_path) as data_file:
        print(f"\nReading data from {data_file_path}")
        json_file = json.load(data_file)
        main(json_file)








