import time
import numpy as np
import matplotlib.pyplot as plt
import json
import argparse

parser = argparse.ArgumentParser("Likelihood_Analysis")
parser.add_argument("source_file", type=str)
args = parser.parse_args()

data_file_path = args.source_file

detector_names = data_file_path.split('_')[1]

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
L_data = Likelihoods - np.min(Likelihoods)
offsets_data = offsets
degree = 9

coefficients, cov = np.polyfit(offsets_data, L_data, degree, cov=True)
errors = np.sqrt(np.diag(cov))

# print the coefficients upto 5 decimal places
# for i in range(len(coefficients)):
#     print(f"coefficient_{i}: {coefficients[i]:.5f} with error: {errors[i]*100/coefficients[i]:.5f} %")

# evaluate the fitted polynomial for various offsets
number_of_points_to_evaluate = 1000
points = np.linspace(offsets[0], offsets[-1], number_of_points_to_evaluate)
L_fitted = np.polyval(coefficients, points)


# ------------------- Plotting -------------------

fig1, ax1 = plt.subplots(2, 1, figsize=(10, 10))
plt.suptitle(f"Likelihood Datapoints and curve for {detector_names}")

# plotting the data alone, and then the data with the fitted curve
ax1[0].plot(offsets, L_data, "o", label="Likelihood data points", color="blue")
ax1[0].set_xlabel("Time difference (s)")
ax1[0].set_ylabel("Likelihood")
ax1[0].legend()

ax1[1].plot(offsets, L_data, label="Likelihood data points", color="blue")
ax1[1].plot(points, L_fitted, label="Likelihood fit", linestyle="--", color="red")
# ax1[1].axvline(x=True_Lag, color="black", linestyle="--", label="Expected Maxima")
ax1[1].set_xlabel("Time difference (s)")
ax1[1].set_ylabel("Likelihood")
ax1[1].legend()


# ------------------- Find Actual Lag Time -------------------

# Estimate from the original likelihoods
Lag_from_actual_data = offsets_data[np.argmax(L_data)]

# Easy estimate from the curve
Lag_estimate_from_curve = points[np.argmax(L_fitted)]

# Estimate from the derivative
curve_derivative1 = np.polyder(coefficients)
roots = np.roots(curve_derivative1)
roots_to_consider = [root.real for root in roots if (np.isreal(root) and offsets_data[0] <= root.real <= offsets_data[-1])]
# print(f"Roots: {roots}")
# print(f"Real roots: {real_roots}")
possible_extrema = [np.polyval(coefficients, root) for root in roots_to_consider]
Lag_estimate_from_derivative = roots_to_consider[np.argmax(possible_extrema)]

# just visually check he roots are not stupid values
# fig2, ax2 = plt.subplots(1, 1, figsize=(10, 5))
# ax2.plot(points, L_fitted, label="Likelihood fit", linestyle="--", color="blue")
# ax2.plot(real_roots, possible_extrema, "o", label="Extrema", color="red")


# plot the final results
fig3, ax3 = plt.subplots(1, 1, figsize=(10, 5))
plt.title(f"Likelihood Curve and Different Maxima for {detector_names}")
ax3.plot(points, L_fitted, label="Likelihood fit", linestyle="--", color="red")
ax3.axvline(x=True_Lag, color="black", linestyle="--", label="Expected Maxima")
ax3.axvline(x=Lag_estimate_from_curve, color="green", linestyle="--", label="Estimated Maxima from curve maximum value")
ax3.axvline(x=Lag_estimate_from_derivative, color="brown", linestyle="--", label="Estimated Maxima from curve derivative")
ax3.axvline(x=Lag_from_actual_data, color="orange", linestyle="--", label="Estimated Maxima from actual data")
ax3.set_xlabel("Time difference (s)")
ax3.set_ylabel("Likelihood")
ax3.legend()


# save results
results = {
    "Analysis of:" : data_file_path,
    "Detectors": detector_names,
    "True-Lag": True_Lag,
    "Estimated-Lag-from-actual-data": Lag_from_actual_data,
    "Estimated-Lag-from-curve-maximum-value": Lag_estimate_from_curve,
    "Estimated-Lag-from-derivative": Lag_estimate_from_derivative
}

# print results in a newline
for key, value in results.items():
    print(f"{key}: {value}")

directory = "src\\analysis\\singleRunResults\\"
now = time.strftime("%Y%m%d-%H%M%S")
fig1.savefig(directory + f"Likelihood_Points_And_Curve_{detector_names}_{now}.png")
fig3.savefig(directory + f"Likelihood_Curve_And_Different_Maxima_{detector_names}_{now}.png")