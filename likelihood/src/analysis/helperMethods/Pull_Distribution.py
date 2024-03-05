import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.stats import norm

from helperMethods import Helpers as hp


def gaussian(x, mean, std):
    return norm.pdf(x, mean, std)


def normalCurveFit(y_values, x_values, ax):
    p, cov = curve_fit(gaussian, x_values, y_values, p0=[np.mean(x_values), np.std(x_values)])
    mean, sigma = p[0], p[1]
    mean_error, sigma_error = np.sqrt(np.diag(cov))

    Title = f"Fit results: mean = {mean:.3f} \u00B1 {mean_error:.3f}, std = {sigma:.3f} \u00B1 {sigma_error:.3f}"

    x_points = np.linspace(x_values[0], x_values[-1], 1000)
    ax.plot(x_points, gaussian(x_points, mean, sigma))

    ax.set_title(Title, fontsize=20)
    ax.set_xlabel("(x - True Lag) / sigma")
    ax.set_ylabel("Probability Density")

    return mean, sigma


def createPlot(data_points, True_value, hist_range, bin_width, method_id, output_folder = None):
    fig, ax = plt.subplots(1, 1, figsize=(10, 10))

    # create histogram
    hist_bins = np.arange(hist_range[0], hist_range[1], bin_width)
    y_values, x_bins, _ = ax.hist(data_points, bins=hist_bins, density=True)
    x_values = 0.5 * (x_bins[1:] + x_bins[:-1])

    # fit a normal curve to the histogram using 2 different scipy methods
    mean_1, std_1 = normalCurveFit(y_values, x_values, ax)
    mean_2, std_2 = norm.fit(data_points)

    # print details
    name = hp.fetchMethodName(method_id)
    print("\n")
    print(f"Pull Distribution details for method: {name}")
    print(f"Used {len(hist_bins)} bins of width: {bin_width:.4f}")
    print(f"Mean: {mean_1:.3f} and std: {std_1:.3f} of the normal curve using scipy.optimize.curve_fit")
    print(f"Mean: {mean_2:.3f} and std: {std_2:.3f} of the normal curve using scipy.stats")
    print("\n")

    if output_folder:
        plt.savefig(output_folder / f"{name}_pull_distribution.png")
        return
    else:
        return mean_1, std_1



def makeDistribution(values, sigmas, True_Lag, method_id, out_folder):
    data_points = [(v - True_Lag) / s for v, s in zip(values, sigmas)]

    # guess an appropriate range and bin width for the pull distribution
    mean = np.mean(data_points)
    std = np.std(data_points)
    hist_range = (mean - 5 * std, mean + 5 * std)
    bin_width = (hist_range[1] - hist_range[0]) / np.sqrt(len(data_points))

    return createPlot(data_points, True_Lag, hist_range, bin_width, method_id, out_folder)



