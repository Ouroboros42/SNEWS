import numpy as np
import matplotlib.pyplot as plt


def makePullDistribution(data_points, hist_range, bin_width, name):
    plt.hist(data_points, bins = np.arange(hist_range[0], hist_range[1], bin_width))
    plt.legend()
    plt.xlabel("Lag (s)")
    plt.ylabel("Frequency")
    plt.title("Pull Distribution")
    plt.show()
    return None
