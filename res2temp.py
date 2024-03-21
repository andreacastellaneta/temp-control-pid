import numpy as np
from scipy.optimize import curve_fit


# Steinhart-Hart function
def steinhart_hart(R, A, B, C):
    return 1 / (A + B * np.log(R) + C * (np.log(R) ** 3))


# Resistance and temperature data in the reference table
resistance_data = [81000, 53500, 35899, 25000, 17550, 12540, 9100, 6710, 5070, 3850, 2940, 2271, 1770, 1414, 1122, 997,
                   896, 797, 719, 643, 582, 533, 483]  # Lista delle resistenze misurate
temperature_data = [30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 175, 180, 185, 190, 195, 200,
                    205, 210]  # Lista delle temperature corrispondenti

# Convert temperatures in kelvin
temperature_data_kelvin = [temp + 273.15 for temp in temperature_data]

# Nonlinear regression to estimate the coefficients of the fitting curve
initial_guess = [1.0e-03, 1.0e-04, 1.0e-07]  # Valori iniziali per A, B, e C
coefficients, covariance = curve_fit(steinhart_hart, resistance_data, temperature_data_kelvin, p0=initial_guess)

# Extract coefficients A, B, e C
A, B, C = coefficients

print("Coefficient A:", A)
print("Coefficient B:", B)
print("Coefficient C:", C)
