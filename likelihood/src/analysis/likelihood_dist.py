import numpy as np
import matplotlib.pyplot as plt
import json

import argparse

parser = argparse.ArgumentParser("Likelihood_Analysis")
parser.add_argument("source_file", type=str)
# args = parser.parse_args()
# data_file_path = args.source_file

data_file_path = "abc.json"
with open(data_file_path) as data_file: 
    data = json.load(data_file)

offsets = data["Time-Difference"]
Likelihoods = data["Likelihood"]
window_width = data["Binned"]["Window-Width"]
True_Lag = data["True-Time-Difference"]

N = len(offsets)
assert N == len(Likelihoods)

h1 = data["Binned"]["Signal-1"]
h2s = data["Binned"]["Signals-2"]

# ------------------- Likelihood Fitting -------------------

# can select a smaller window to fit the likelihood distribution, currently use all the data
L_data = Likelihoods
offsets_data = offsets

coefficients = np.polyfit(offsets_data, L_data, 9)
print(coefficients)

# evaluate the fitted polynomial for various offsets
number_of_points_to_evaluate = 1000
points = np.linspace(offsets[0], offsets[-1], number_of_points_to_evaluate)
L_fitted = np.polyval(coefficients, points)


# ------------------- Plotting -------------------

fig1, ax1 = plt.subplots(2, 1, figsize=(10, 10))

# plotting the data alone, and then the data with the fitted curve
ax1[0].plot(offsets, L_data, label="Likelihood data points", color="blue")
ax1[0].set_xlabel("Time difference (s)")
ax1[0].set_ylabel("Likelihood")
ax1[0].legend()

ax1[1].plot(offsets, L_data, label="Likelihood data points", color="blue")
ax1[1].plot(points, L_fitted, label="Likelihood fit", linestyle="--", color="red")
ax1[1].axvline(x=True_Lag, color="black", linestyle="--", label="Expected Maxima")
ax1[1].set_xlabel("Time difference (s)")
ax1[1].set_ylabel("Likelihood")
ax1[1].legend()


# ------------------- Find Actual Lag Time -------------------


# Easy estimate from the curve
Lag_estimate_from_curve = points[np.argmax(L_fitted)]

# Estimate from the derivative
curve_derivative1 = np.polyder(coefficients)
roots = np.roots(curve_derivative1)
real_roots = [root.real for root in roots if np.isreal(root)]
# print(f"Roots: {roots}")
# print(f"Real roots: {real_roots}")
possible_extrema = [np.polyval(coefficients, root) for root in real_roots]
Lag_estimate_from_derivative = real_roots[np.argmax(possible_extrema)]

# just visually check he roots are not stupid values
# fig2, ax2 = plt.subplots(1, 1, figsize=(10, 5))
# ax2.plot(points, L_fitted, label="Likelihood fit", linestyle="--", color="blue")
# ax2.plot(real_roots, possible_extrema, "o", label="Extrema", color="red")


# plot the final results
fig3, ax3 = plt.subplots(1, 1, figsize=(10, 5))
ax3.plot(points, L_fitted, label="Likelihood fit", linestyle="--", color="blue")
ax3.axvline(x=True_Lag, color="black", linestyle="--", label="Expected Maxima")
ax3.axvline(x=Lag_estimate_from_curve, color="green", linestyle="--", label="Estimated Maxima from max vale in an "
                                                                            "evualted array")
ax3.axvline(x=Lag_estimate_from_derivative, color="red", linestyle="--", label="Estimated Maxima from derivative")
ax3.set_xlabel("Time difference (s)")
ax3.set_ylabel("Likelihood")
ax3.legend()

# print results
print(f"True Lag: {True_Lag}")
print(f"Estimated Lag from curve: {Lag_estimate_from_curve}")
print(f"Estimated Lag from derivative: {Lag_estimate_from_derivative}")

plt.show()