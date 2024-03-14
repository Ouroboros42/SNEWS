import os
import pathlib
import numpy as np

def display(True_Lag, values, bounds, sigmas, method_id, verbose=False, format_upto = 2):
    score = np.sum(bound[0] <= True_Lag <= bound[1] for bound in bounds)
    method_name = fetchMethodName(method_id)

    print("\n")
    print(f"Method: {method_name} \n")
    print(f"True Lag: {True_Lag * 1000:.{format_upto}f} ms \n")
    print(f"Avg estimate: {np.mean(values) * 1000:.{format_upto}f} ms")
    print(f"Avg error: {np.mean(sigmas) * 1000:.{format_upto}f} ms")
    print(f"Success rate: {score}/{len(bounds)}")


    if verbose:
        print("\n Actual Estimate Values: ")
        print(f"Estimates: {values}")
        print(f"Sigmas: {sigmas}")



def readParameters(json_file):
    True_Lag = json_file["True-Time-Diff"]
    detector1 = json_file["detector1"]
    detector2 = json_file["detector2"]
    numTrials = json_file["num-Trials"]
    sweep_range = json_file["sweep-range"]
    return True_Lag, detector1, detector2, numTrials, sweep_range


def fetchMethodName(method_id):
    # method names array for display
    names = ["Raw Data", "Curve Fitting", "Moving Average", "Noise Filter", "Noise Filter + Moving Average"]

    if method_id >= len(names):
        print(f"WARNING: Method {method_id} has no name. Check fetchMethodName method in Helpers.py")
        return "Unknown"

    return names[method_id - 1]