import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm


def normalDistributionFit(y_values, x_values, ax):
    mean, std = norm.fit(y_values)
    p = norm.pdf(x_values, mean, std)

    ax.plot(x_values, p, label=f"Normal Distribution fitted")
    Title = "Fit results: mean = %.2f,  std = %.2f" % (mean, std)
    ax.set_title(Title)

    return mean, std


def createDistribution(data_points, True_value, hist_range, bin_width, name = "", output_folder = None):
    fig, ax = plt.subplots(1, 1, figsize=(10, 10))

    y_values, bins, _ = ax.hist(data_points, bins=np.arange(hist_range[0], hist_range[1], bin_width), label=f"{name} Pull Distribution")
    x_values = 0.5 * (bins[1:] + bins[:-1])
    assert len(y_values) == len(x_values)

    # plot details
    ax.set_xlabel("estimate - actual value / error")
    ax.set_ylabel("Frequency")
    ax.legend()

    # debug info
    print("Pull Distribution for ", name)
    print(f"number of bins: {len(bins)} with bin width: {bin_width} and range: {hist_range}")
    print(f"Total number of events binned in the histogram: {np.sum(y_values)} out of {len(data_points)}")

    # mean, std = normalDistributionFit(y_values, x_values, ax)

    if output_folder:
        plt.savefig(output_folder + f"{name}_Pull_Distribution.png")








