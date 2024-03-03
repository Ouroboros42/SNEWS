import os
import pathlib
import numpy as np

def display(True_Lag, values_1, bounds_1, values_1_errors, values_2, bounds_2, values_2_errors, method_ids, verbose=False):
    print("\n\n")
    print(f"True Lag: {True_Lag}")

    names = ["raw data", "curve fitting", "moving average", "noise filter", "noise filter + moving average"]
    score_1 = np.sum(bound[0] <= True_Lag <= bound[1] for bound in bounds_1)
    score_2 = np.sum(bound[0] <= True_Lag <= bound[1] for bound in bounds_2)

    print("\n")
    print(f"Method {method_ids[0]}: {names[method_ids[0] - 1]}:")
    print(f"Average Lag: {np.mean(values_1)}")
    print(f"Average interval size: {np.mean(values_1_errors)}")
    print(f"Success rate: {score_1}")

    print("\n")
    print(f"Method {method_ids[1]}: {names[method_ids[1] - 1]}:")
    print(f"Average Lag: {np.mean(values_2)}")
    print(f"Average error: {np.mean(values_2_errors)}")
    print(f"Success rate: {score_2}")
    print("\n\n")

    return score_1, score_2


def displayVerbose(True_Lag, values_1, errors_1, values_2, errors_2):
    # format upto 3 decimal places and display the actual numbers
    print("\n\n")
    xx1 = [f"{x:.5f}" for x in values_1]
    xx2 = [f"{x:.5f}" for x in errors_1]
    print(f"method 1 estimates: {xx1}")
    print(f"method 1 intervals: {xx2}")
    print("\n\n")
    xx1 = [f"{x:.5f}" for x in values_2]
    xx2 = [f"{x:.5f}" for x in errors_2]
    print(f"method 2 estimates: {xx1}")
    print(f"method 2 intervals: {xx2}")
    print("\n\n")



def readParameters(json_file):
    True_Lag = json_file["True-Time-Diff"]
    detector1 = json_file["detector1"]
    detector2 = json_file["detector2"]
    numTrials = json_file["num-Trials"]
    sweep_range = json_file["sweep-range"]
    return True_Lag, detector1, detector2, numTrials, sweep_range