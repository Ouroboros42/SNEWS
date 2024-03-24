import os
import pathlib
import numpy as np

def display(True_Lag, values, sigmas, score, method_id, verbose=False, format_upto = 2):
    # All results and calculations are in seconds, but we display in ms for readability
    method_name = fetchMethodName(method_id)

    print("\n")
    print(f"Analysed {len(values)} samples")
    print(f"Method Used: {method_name}")
    print("\n")
    print(f"True Lag: {True_Lag * 1000:.{format_upto}f} ms")
    print("\n")
    print(f"Average estimate: {np.mean(values) * 1000:.{format_upto}f} ms")
    print(f"Standard devs of errs: {np.std(np.asarray(values) - True_Lag) * 1000:.{format_upto}f} ms")
    print(f"Average uncertainty: {np.mean(sigmas) * 1000:.{format_upto}f} ms")
    print(f"Success rate: {score * 100:.1f}%")
    print("\n")

    if verbose:
        print("Actual Estimate Values: ")
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
    names = ["Raw Data", "Curve Fitting", "Moving Average", "Noise Filter", "New Moving Average"]

    if method_id > len(names):
        print(f"WARNING: Method {method_id} has no name. Check fetchMethodName method in Helpers.py")
        return "Unknown"

    return names[method_id - 1]