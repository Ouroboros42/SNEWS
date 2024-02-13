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

fig, ax1 = plt.subplots(1, 1)

ax1.scatter(t, L)

# L_fft = fft.fft(L)
# L_fft[0] = 0
# t_f = fft.fftfreq(len(t), t[1] - t[0])

# freq_cutoff = 1/24

# L_fft_cut = L_fft.copy()
# print(min(L_fft_cut))
# # L_fft_cut[L_fft_cut < -70] = 0 # Remove big freq
# # i_cutoff = int(N / 2 * freq_cutoff)
# # L_fft_cut[i_cutoff:N -i_cutoff] = 0

# L_filtered = fft.ifft(L_fft_cut)

# fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2)

# ax1.scatter(t, L)
# ax2.scatter(t_f[:N//2], L_fft[:N//2])
# ax3.scatter(t_f[:N//2], L_fft_cut[:N//2])
# ax4.scatter(t, L_filtered)

plt.show()