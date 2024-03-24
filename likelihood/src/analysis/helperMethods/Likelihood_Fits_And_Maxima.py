import numpy as np
import matplotlib.pyplot as plt
import json
import copy


## ------------------- Smoothing Methods -------------------

def smoothWithMovingAverage(L_data, T_data, window_half_width):
    numPoints = len(L_data)
    L_smoothed = []
    T_smoothed = []
    for i in range(window_half_width, numPoints - window_half_width):
        new_point = np.mean(L_data[i - window_half_width: i + window_half_width])
        L_smoothed.append(new_point)
        T_smoothed.append(T_data[i])

    return L_smoothed, T_smoothed


def smoothWithNoiseFilter(L_data, T_data, window_half_width, noise_bound_in_sigma = 1.0):
    # smooth data with moving average before
    L_smoothed, T_smoothed = smoothWithMovingAverage(L_data, T_data, window_half_width)

    L_data = L_data[window_half_width: -window_half_width]
    T_data = T_data[window_half_width: -window_half_width]

    # data points too far away from the smoothed curve are considered noise and removed
    deviation = [L_data[i] - L_smoothed[i] for i in range(len(L_smoothed))]
    sigma = np.std(deviation)
    bound = noise_bound_in_sigma * sigma

    Likelihoods = []
    Time_Diffs = []
    for i, dev in enumerate(deviation):
        if abs(dev) < bound:
            Likelihoods.append(L_data[i])
            Time_Diffs.append(T_data[i])

    return Likelihoods, Time_Diffs


## ------------------- Legacy Methods -------------------
## Raw data analysis is not recommended because the error bounds are not reliable

def findRawDataMaximaAndError(L_data, T_data, True_Lag, error_bound = 0.5, ax: plt.Axes = None):
    max_L_position = np.argmax(L_data)
    max_L_value = L_data[max_L_position]
    best_Lag = T_data[max_L_position]

    Avg_window = 3

    # extreme horrible edge case (should not happen)
    i = max(max_L_position - 1, 0)
    while i >= Avg_window:
        mean = np.mean(L_data[i - Avg_window : i])
        if mean < max_L_value - error_bound:
            break
        i -= 1

    j = min(max_L_position + 1, len(L_data) - 1)
    while j < len(L_data) - Avg_window:
        mean = np.mean(L_data[j : j + Avg_window])
        if mean < max_L_value - error_bound:
            break
        j += 1

    assert i <= max_L_position <= j

    if ax:
        ax.plot(T_data, L_data, "o", label="Likelihood data points")
        ax.axvline(x=best_Lag, linestyle="--", label="Best Lag")
        ax.axvline(x=T_data[i], linestyle="--", label="Error Bound")
        ax.axvline(x=T_data[j], linestyle="--")
        if True_Lag:
            ax.axvline(x=True_Lag, linestyle="--", label="True T", color="black")
        ax.set_xlabel("Time difference (s)")
        ax.set_ylabel("Likelihood")
        ax.legend()

    return best_Lag, T_data[i], T_data[j]


def findErrorUsingDifferenceFromMaxValue(coefficients, points, error_bound, max_recurse: int = 5):
    L_fitted = np.polyval(coefficients, points)
    max_L_position = np.argmax(L_fitted)
    max_L_value = L_fitted[max_L_position]
    best_Lag = points[max_L_position]

    points_within_error_bound = [p for i, p in enumerate(points) if (max_L_value - L_fitted[i]) <= error_bound]

    if max_recurse == 0:
        return best_Lag, points_within_error_bound[0], points_within_error_bound[-1]

    if len(points_within_error_bound) < 2:
        points = np.linspace(points[max_L_position - 1], points[max_L_position + 1], 20)
        return findErrorUsingDifferenceFromMaxValue(coefficients, points, error_bound, max_recurse - 1)
    else:
        return best_Lag, points_within_error_bound[0], points_within_error_bound[-1]



## ------------------- Curve Fitting and Error Methods -------------------

def validationDebug(f, max, err1, err2):
    print(f"Left {f(max) - f(err1)} and Right {f(max) - f(err2)}")

def showCoefficients(coefficients, error_coefficients):
    for c, e in zip(coefficients, error_coefficients):
        r = e / c * 100
        print(f"coefficient is {c:.4f} with relative error {r:.4f}%")




def getQuadraticFit(Likelihood, bounds, max_pos, error_bound = 0.5):
    points = np.linspace(bounds[0], bounds[1], 5000)
    L_points = Likelihood(points)
    L_max = Likelihood(max_pos)

    rangeForQuadratic = [p for i, p in enumerate(points) if (L_max - L_points[i]) <= error_bound]
    dataForQuadratic = [Likelihood(p) for p in rangeForQuadratic]

    quad_coefficients, quad_cov = np.polyfit(rangeForQuadratic, dataForQuadratic, 2, cov=True)

    quad_error = np.sqrt(quad_cov[0][0])
    if quad_error / quad_coefficients[0] > 0.5:
        print("WARNING: Relative error of the quadratic coefficient is too high")

    return quad_coefficients


def findErrorUsingSecondDerivative(coefficients, points, maxima_guess, quadratic_approx = False):
    # info is Fisher's information. variance of the maximum likelihood estimator is 1/Fisher's information

    Likelihood = np.polynomial.polynomial.Polynomial(coef=coefficients[::-1])
    Likelihood_prime = Likelihood.deriv()
    Likelihood_double_prime = Likelihood_prime.deriv()

    roots = Likelihood_prime.roots()
    roots = [np.real(r) for r in roots if (r >= points[0] and r <= points[-1]) and np.isreal(r)]
    roots = [r for r in roots if Likelihood_double_prime(r) < 0]

    if len(roots) == 1:
        closest_value = roots[0]
    else:
        closest_value = min(roots, key= lambda x : abs(x - maxima_guess))

    if quadratic_approx:
        quad_coefficients = getQuadraticFit(Likelihood, (points[0], points[-1]), closest_value)
        info = -1 * 2 * quad_coefficients[0]
    else:
        info = -1 * Likelihood_double_prime(closest_value)

    if info < 0:
        print("WARNING: Second derivative is positive, but expected to be negative at the maximum")

    return closest_value, np.sqrt(1/np.abs(info))




## Default degree of the polynomial is 10
## Error-bound is the difference between the Likelihood values at the maximum and a small distance away
## When this difference is 0.5, this distance is the 1-sigma error bound.
def polynomialFit(
        L_data,
        T_data,
        True_Lag,
        number_of_points_to_evaluate = 1000,
        degree = 10,
        error_bound = 0.5,
        ax: plt.Axes = None,
        plot_raw_data = True,
  ):

    coefficients, cov = np.polyfit(T_data, L_data, degree, cov=True)
    offset_points = np.linspace(T_data[0], T_data[-1], number_of_points_to_evaluate)
    L_fitted = np.polyval(coefficients, offset_points)

    # Legacy method based on function falling off from the maximum by 0.5
    # best, err1, err2 = findErrorUsingDifferenceFromMaxValue(coefficients, copy.deepcopy(offset_points), error_bound)

    # showCoefficients(coefficients, np.sqrt(np.diag(cov)))

    # New method based on second derivative
    maxima_guess = offset_points[np.argmax(L_fitted)]
    error_on_coeffs = np.sqrt(np.diag(cov))
    best, error = findErrorUsingSecondDerivative(coefficients, offset_points, maxima_guess, False)
    err1, err2 = best - error, best + error

    if ax:
        ax.plot(offset_points, L_fitted, label="Fitted Curve")
        ax.axvline(x=best, linestyle="--", label="Best Lag and Error Bounds")
        ax.axvline(x=err1, linestyle="--")
        ax.axvline(x=err2, linestyle="--")
        ax.axvline(x=True_Lag, linestyle="--", label="True Lag", color="black")
        if plot_raw_data:
            ax.plot(T_data, L_data, "o", label="Likelihood data points")
        ax.set_xlabel("Time difference (s)")
        ax.set_ylabel("Likelihood")
        ax.legend()

    return best, err1, err2