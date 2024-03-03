import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.stats import norm


def gaussian(x, mean, std):
    return norm.pdf(x, mean, std)


def normalCurveFit(y_values, x_values, ax):
    p, cov = curve_fit(gaussian, x_values, y_values, p0=[np.mean(x_values), np.std(x_values)])
    mean, sigma = p[0], p[1]
    mean_error, sigma_error = np.sqrt(np.diag(cov))

    Title = f"Fit results: mean = {mean:.3f} \u00B1 {mean_error:.3f}, std = {sigma:.3f} \u00B1 {sigma_error:.3f}"

    x_points = np.linspace(x_values[0], x_values[-1], 1000)
    ax.plot(x_points, gaussian(x_points, mean, sigma), label=Title)
    ax.legend()

    return mean, sigma


def createPlot(data_points, True_value, hist_range, bin_width, name ="", output_folder = None):
    fig, ax = plt.subplots(1, 1, figsize=(10, 10))

    # create histogram
    hist_bins = np.arange(hist_range[0], hist_range[1], bin_width)
    y_values, x_bins, _ = ax.hist(data_points, bins=hist_bins, label=f"{name} Pull Distribution", density=True)
    x_values = 0.5 * (x_bins[1:] + x_bins[:-1])

    assert np.isclose(np.sum(y_values) * bin_width, 1.0)

    mean, std = normalCurveFit(y_values, x_values, ax)

    # alternative curve fit using scipy.stats (just to check agreement)
    mean2, std2 = norm.fit(data_points)

    # debug
    print("Pull Distribution for ", name)
    # print(f"{len(hist_bins)} bins of width: {bin_width:.4f} and range: {hist_range[0]:.4f} to {hist_range[1]:.4f}")
    print(f"Mean: {mean:.3f} and std: {std:.3f} of the normal curve using scipy.optimize.curve_fit")
    print(f"Mean: {mean2:.3f} and std: {std2:.3f} of the normal curve using scipy.stats")
    print("\n")

    # plot details
    ax.set_xlabel("(estimate - actual value) / error")
    ax.set_ylabel("Frequency")
    ax.legend()

    if output_folder:
        plt.savefig(output_folder / f"{name}_pull_distribution.png")
        return
    else:
        return mean, std



def makeDistribution(estimates, errors, True_Lag, name, out_folder):
    data_points = [(estimate - True_Lag) / error for estimate, error in zip(estimates, errors)]

    mean = np.mean(data_points)
    std = np.std(data_points)

    # These are just guesses
    hist_range = (mean - 5 * std, mean + 5 * std)
    bin_width = (hist_range[1] - hist_range[0]) / np.sqrt(len(data_points))

    return createPlot(data_points, True_Lag, hist_range, bin_width, name, out_folder)



