import numpy as np
import matplotlib.pyplot as plt
from scipy import fft
import json

import argparse

parser = argparse.ArgumentParser("Likelihood_Analysis")
parser.add_argument("source_file", type=str)
args = parser.parse_args()

data_file_path = args.source_file
with open(data_file_path) as data_file: 
    data = json.load(data_file)

t = data["Time-Difference"]
L = data["Likelihood"]

N = len(t)
assert N == len(L)

h1 = data["Binned"]["Signal-1"]
h2s = data["Binned"]["Signals-2"]
window_width = data["Binned"]["Window-Width"]

fig, ((ax11, ax21), (ax12, ax22), (ax13, ax23)) = plt.subplots(3, 2)

bins = np.linspace(0, window_width, len(h1) + 1)
def plot_signal_hist(ax, hist):
    ax.set_xlim(0, window_width)
    ax.stairs(hist, bins)

plot_signal_hist(ax11, h2s[0])
plot_signal_hist(ax12, h2s[1])
plot_signal_hist(ax13, h2s[2])
plot_signal_hist(ax21, h1)

ax22.scatter(t, L)

plt.show()