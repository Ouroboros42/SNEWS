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
from helperMethods import Helpers as help

## Look at the end of the file for usage instructions and in the main method to customise analysis parameters

# ------------------- Methods -------------------

def yourFavouriteMethods(Likelihoods, TimeDifferences, True_Lag, methods_id: int, draw, ax = None):
    ## Method 1: raw data (not recommended)
    ## Method 2: Curve fitting on raw data (default curve is polynomial of degree 10)
    ## Method 3: Clean with moving average (window size for averaging is 2 * half-width)
    ## Method 4: Clean with noise filter (points that deviate highly from the smoothed curve are removed. The bound for
    ## deviation is given in units of sigma, where sigma is the standard deviation of the data)

    match methods_id:
        case 1:
            res = fits.findRawDataMaximaAndError(Likelihoods, TimeDifferences, True_Lag, ax = ax if draw else None)

        case 2:
            res = fits.polynomialFit(Likelihoods, TimeDifferences, True_Lag, ax = ax if draw else None, error_bound=1)

        case 3:
            hw = 3
            L_smoothed, T_smoothed = fits.smoothWithMovingAverage(Likelihoods, TimeDifferences, hw)
            res = fits.polynomialFit(L_smoothed, T_smoothed, True_Lag, ax = ax if draw else None)

        case 4:
            hw =  3
            bound = 1
            L_smoothed, T_smoothed = fits.smoothWithNoiseFilter(Likelihoods, TimeDifferences, hw, bound)
            res = fits.polynomialFit(L_smoothed, T_smoothed, True_Lag, ax = ax if draw else None)

    # ax.set_title(f"Method {methods_id}")
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

        estimate = yourFavouriteMethods(Likelihoods, TimeDifferences, True_Lag, method_id, draw, ax)
        estimates.append(estimate)

        if output_folder and draw:
            plt.savefig(output_folder / f"Trial_{i}.png")

    return estimates


def unpackAndTestEstimates(estimates, True_Lag):
    values: List[float] = []
    sigmas: List[float] = []
    score: float = 0
    for estimate in estimates:
        assert estimate[1] <= estimate[0] <= estimate[2], f"Estimate is not consistent with its bounds: {estimate}"
        values.append(estimate[0])
        sigmas.append((estimate[2] - estimate[1]) / 2)
        score += (estimate[1] <= True_Lag <= estimate[2])

    score /= len(estimates)
    return values, sigmas, score



def makeOutputPath(detector1, detector2, numTrials, sweep_range):
    relative_path = f"TrialsResults/{detector1}_vs_{detector2}_{numTrials}_Trials_Sweep_{sweep_range}"
    base_path = pathlib.Path(__file__).parent.resolve()
    path = base_path / relative_path
    if not os.path.exists(path):
        os.mkdir(path)
    print(f"\nOutput Folder: {path}")
    return path


# ------------------- Debug -------------------

def checkPlots(json_file, True_Lag):
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

    if numTrials:
        exit(0)


# ------------------- Main -------------------

def main(json_file):
    # read parameters and make output folder
    True_Lag, detector1, detector2, numTrials, sweep_range = help.readParameters(json_file)
    out_folder = makeOutputPath(detector1, detector2, numTrials, sweep_range)

    # only for debugging (check method above for info)
    checkPlots(json_file, True_Lag)

    # analysis parameters (read between 0 - numTrials samples, and draw roughly 5-10 plots for visualisation)
    numSamplesToRead = numTrials
    numPlotsToDraw = 5

    # draw every n-th sample (set numPlotsToDraw = 0 above to disable)
    draw_every = (numSamplesToRead // numPlotsToDraw) if numPlotsToDraw > 0 else numTrials + 1

    # Look in 'yourFavouriteMethods' above for the definition of methods ids
    # id = 2 is simple curve fitting, and works best in most cases
    method_id = 2

    # read data and make estimates
    # also produces a plot every 'draw_every' samples
    estimates = readDataAndMakeEstimates(json_file,  numSamplesToRead, True_Lag, method_id, draw_every, out_folder)

    # unpack and display results
    values, sigmas, score = unpackAndTestEstimates(estimates, True_Lag)
    help.display(True_Lag, values, sigmas, score, method_id, verbose=False, format_upto=2)

    dist.main(values, sigmas, True_Lag, method_id = method_id, out_folder=out_folder)




# ------------------- Run -------------------


## Usage:
## Run: python AnalyseTrials.py <file_path>
## Example: "python AnalyseTrials.py TrialsData/IC_vs_SK_1000.json" for 1000 Trials of IceCube vs SuperK

parser = argparse.ArgumentParser("Likelihood_Analysis")
parser.add_argument("source_file", type=str)
args = parser.parse_args()

data_file_path = args.source_file

if __name__ == "__main__":
    with open (data_file_path) as data_file:
        print(f"\nReading data from: {data_file_path}")
        json_file = json.load(data_file)
        main(json_file)








