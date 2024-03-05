import numpy as np
import json
import matplotlib.pyplot as plt
import argparse
import os
import pathlib
from typing import List, Tuple

from helperMethods import Likelihood_Fits_And_Maxima as fits
from helperMethods import Visualise_Raw_Data as visualise
from helperMethods import Pull_Distribution as dist
from helperMethods import Helpers as helper


# ------------------- Methods -------------------

def MyFavouriteMethods(Likelihoods, TimeDifferences, True_Lag, methods_id: int, draw, ax = None):
    ## Method 1: raw data
    ## Method 2: Curve fitting on raw data
    ## Method 3: Clean with moving average
    ## Method 4: Clean with noise filter
    ## Method 5: clean with noise filter and further take moving average

    match methods_id:
        case 1:
            res = fits.findRawDataMaximaAndError(Likelihoods, TimeDifferences, True_Lag, ax = ax if draw else None)

        case 2:
            res = fits.polynomialFit(Likelihoods, TimeDifferences, True_Lag, ax = ax if draw else None, error_bound=1)

        case 3:
            L_smoothed, T_smoothed = fits.cleanWithMovingAverage(Likelihoods, TimeDifferences, 4)
            res = fits.polynomialFit(L_smoothed, T_smoothed, True_Lag, ax = ax if draw else None)

        case 4:
            L_smoothed, T_smoothed = fits.cleanWithNoiseFilter(Likelihoods, TimeDifferences, 4, 1)
            res = fits.polynomialFit(L_smoothed, T_smoothed, True_Lag, ax = ax if draw else None)

        case 5:
            L_smoothed, T_smoothed = fits.cleanWithNoiseFilter(Likelihoods, TimeDifferences, 4, 1)
            L_smoothed_again, T_smoothed_again = fits.cleanWithMovingAverage(L_smoothed, T_smoothed, 4)
            res = fits.polynomialFit(L_smoothed_again, T_smoothed_again, True_Lag, ax = ax if draw else None, plot_raw_data = True)

    ax.set_title(f"Method {methods_id}")
    return res



def readDataAndMakeEstimates(json_file, num_samples, True_Lag, method_id=2, draw_every=1000, output_folder=None):
    estimates = []
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
            fig, ax = plt.subplots(1, 1, figsize=(20, 10))

        res = MyFavouriteMethods(Likelihoods, TimeDifferences, True_Lag, method_id, draw, ax)
        estimates.append(res)

        if output_folder and draw:
            plt.savefig(output_folder / f"Trial_{i}.png")

    return estimates


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
    # only used for visual debugging. change numTrials to a positive number to see some plots.
    # The program will exit after visualising the plots, no analysis will be done.
    numTrials = 0
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
    path = base_path / relative_path
    if not os.path.exists(path):
        os.mkdir(path)
    print(f"\nOutput Folder: {path}")
    return path

# ------------------- Main -------------------

def main(json_file):
    # read parameters from the json_file
    True_Lag, detector1, detector2, num_Trials, sweep_range = helper.readParameters(json_file)
    # make output folder
    out_folder = makeOutputPath(detector1, detector2, num_Trials, sweep_range)

    # visualise some plots (only for debugging, check method body for details)
    VisualiseRawData(json_file, True_Lag)

    # analysis parameters
    num_samples_to_read = num_Trials # (max: numTrials)
    print(f"\nReading {num_samples_to_read} samples")
    num_plots_to_draw = 5
    draw_every = (num_samples_to_read // num_plots_to_draw) if num_plots_to_draw > 0 else num_Trials + 1

    # Look in MyFavouriteMethods above for the definition of methods_ids. Pick Any to use for analysis
    # id = 2 corresponds to simply cure fitting the raw data, and works best for pull distributions
    # This will also produce 'num_plots_to_draw' plots of the analysis method used
    method_id = 2

    # read data and make estimates
    estimates = readDataAndMakeEstimates(json_file, num_samples_to_read, True_Lag, method_id, draw_every, out_folder)

    # values are the best estimates, bounds are the 1-sigma bounds on both left and right
    # sigmas are the average of the left and right bounds
    values, bounds = unpackAndTestEstimates(estimates)
    sigmas = boundsToSigmas(bounds, values)

    # print results
    helper.display(True_Lag, values, bounds, sigmas, method_id, verbose=False, precision = 6)

    # make pull distribution
    dist.makeDistribution(values, sigmas, True_Lag, method_id = method_id, out_folder=out_folder)


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








