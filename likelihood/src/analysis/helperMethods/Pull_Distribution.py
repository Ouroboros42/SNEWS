import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit, minimize
from scipy.stats import norm, shapiro

from random import gauss

from helperMethods import Helpers as help

def gaussian(x, mean, std):
    return norm.pdf(x, mean, std)

def gaussian_neg_log_likelihood(params, data):
    like = np.sum(np.log(gaussian(data, *params)))
    return - like

def max_likelihood_norm_fit(data):
    initial_guess = np.asarray([0, 1]) # mean 0, std 1
    optimize_result = minimize(gaussian_neg_log_likelihood, initial_guess, args=(data,))

    params = optimize_result.x
    params_err = np.sqrt(np.diag(optimize_result.hess_inv))

    return ((params[0], params[1]), (params_err[0], params_err[1]))

def createHistogram(data_points, True_value, hist_range, bin_width, method_id, output_folder = None):
    fig, ax = plt.subplots(1, 1, figsize=(5, 2.5), dpi=1000)

    # create histogram
    hist_bins = np.arange(hist_range[0], hist_range[1], bin_width)
    y_values, x_bins, _ = ax.hist(data_points, bins=hist_bins, density=True, label="$g$-Distribution")

    # fit a normal curve to the histogram using 2 different scipy methods
    ((mean, sigma), (mean_error, sigma_error)) = max_likelihood_norm_fit(data_points)

    direct_mean = np.mean(data_points)
    direct_std = np.std(data_points)

    direct_mean_err = direct_std / np.sqrt(len(data_points))
    direct_std_err = direct_std / np.sqrt(2 * len(data_points) - 2)

    shapiro_stat, shapiro_p = shapiro(data_points)

    Title = rf"$\bar{{g}} = {mean:.3f} \pm {mean_error:.3f}$, $\sigma_g = {sigma:.3f} \pm {sigma_error:.3f}$"

    x_points = np.linspace(x_bins[0], x_bins[-1], 1000)
    ax.plot(x_points, gaussian(x_points, mean, sigma), label="Normal Best-Fit", linewidth=3.0)

    ax.set_title(Title)
    ax.set_xlabel(r"$g=(\hat\tau-\tau_0) / \hat\sigma_{\tau}$")
    ax.set_ylabel("Probability Density")
    ax.legend(loc="upper right")

    print(f"Used {len(hist_bins)} bins of width: {bin_width:.4f}\n")
    print(f"MLE - Mean: {mean:.3f} \u00B1 {mean_error:.3f} Sigma: {sigma:.3f} \u00B1 {sigma_error:.3f}")
    print(f"Direct - Mean: {direct_mean:.3f} \u00B1 {direct_mean_err:.3f} Sigma: {direct_std:.3f} \u00B1 {direct_std_err:.3f}")
    print(f"g is normally distributed at p = {shapiro_p} (W={shapiro_stat})")

    if output_folder:
        name = help.fetchMethodName(method_id)
        plt.savefig(output_folder / f"{name}_pull_distribution.png", bbox_inches="tight")

def main(values, sigmas, True_Lag, method_id, out_folder):
    data_points = [(v - True_Lag) / s for v, s in zip(values, sigmas)]

    # guess an appropriate range and bin width for the pull distribution
    mean = np.mean(data_points)
    std = np.std(data_points)

    hist_range = (mean - 5 * std, mean + 5 * std)
    bin_width = (hist_range[1] - hist_range[0]) / np.sqrt(len(data_points))

    createHistogram(data_points, True_Lag, hist_range, bin_width, method_id, out_folder)



