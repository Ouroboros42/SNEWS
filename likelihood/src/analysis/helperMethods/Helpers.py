import os
import pathlib
import numpy as np

def display(True_Lag, values_1, values_1_errors, values_2, values_2_errors, verbose=False):
    if verbose:
        displayVerbose(True_Lag, values_1, values_1_errors, values_2, values_2_errors)
    print("\n\n")
    print(f"True Lag: {True_Lag}")
    print("\n")
    print(f"Average Lag from method 1: {np.mean(values_1)}")
    print(f"Average error from method 1: {np.mean(values_1_errors)}")
    print("\n")
    print(f"Average Lag from method 2: {np.mean(values_2)}")
    print(f"Average error from method 2: {np.mean(values_2_errors)}")
    print("\n\n")
    return


def displayVerbose(True_Lag, values_1, errors_1, values_2, errors_2):
    # format upto 3 decimal places and display the actual numbers
    print("\n\n")
    xx1 = [f"{x:.5f}" for x in values_1]
    xx2 = [f"{x:.5f}" for x in errors_1]
    print(f"method 1 estimates: {xx1}")
    print(f"method 1 sigmas: {xx2}")
    print("\n\n")
    xx1 = [f"{x:.5f}" for x in values_2]
    xx2 = [f"{x:.5f}" for x in errors_2]
    print(f"method 2 estimates: {xx1}")
    print(f"method 2 sigmas: {xx2}")
    print("\n\n")



def readParameters(json_file):
    True_Lag = json_file["True-Time-Diff"]
    inst = json_file["inst"]
    detector1 = json_file["detector1"]
    detector2 = json_file["detector2"]
    numTrials = json_file["num-Trials"]
    sweep_range = json_file["sweep-range"]
    return True_Lag, detector1, detector2, inst, numTrials, sweep_range