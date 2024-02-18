import numpy as np
import matplotlib.pyplot as plt
from scipy import fft
import json

import argparse

parser = argparse.ArgumentParser("Likelihood_Analysis")
parser.add_argument("source_file", type=str)
# args = parser.parse_args()
# data_file_path = args.source_file

data_file_path = "abc"
with open(data_file_path) as data_file: 
    data = json.load(data_file)

t = data["Time-Difference"]
L = data["Likelihood"]

N = len(t)
assert N == len(L)

h1 = data["Binned"]["Signal-1"]
h2s = data["Binned"]["Signals-2"]
window_width = data["Binned"]["Window-Width"]

