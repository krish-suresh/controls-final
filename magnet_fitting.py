import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.optimize import curve_fit
import numpy as np

file_path = "data/magnetcharecterization.csv"
data = pd.read_csv(file_path)

PWM = data.columns[1:]
PWM = []
X = []
F = []
for i in range(1, len(data.columns)):
    for j, val in enumerate(data[data.columns[i]]):
        PWM.append(data.columns[i])
        X.append(val)
        F.append(data[data.columns[0]][j])


# Convert PWM, X, and F to numeric values
PWM = [float(p) for p in PWM]
X = [float(x)/ 1000.0 for x in X] 
F = [float(f) for f in F]


# Define the function to fit
def func(data, a, b):
    PWM, X = data
    return a - b * (PWM) / X**3


# Prepare the data for curve_fit
data = np.vstack((PWM, X))
popt, pcov = curve_fit(func, data, F)

# Print the optimal parameters
print(f"a = {popt[0]}, b = {popt[1]}")

F_fit = func(data, *popt)
residuals = F - F_fit
total_error = np.sum(residuals**2)
print(f"Total fitting error: {total_error}")

# Plot the original data
fig = plt.figure()
ax = fig.add_subplot(111, projection="3d")
ax.scatter(PWM, X, F, color="r")

# Plot the fitted function
PWM_range = np.linspace(min(PWM), max(PWM), 1000)
X_range = np.linspace(min(X), max(X), 1000)
PWM_range, X_range = np.meshgrid(PWM_range, X_range)
F_fit = func((PWM_range, X_range), *popt)
ax.plot_surface(PWM_range, X_range, F_fit, alpha=0.5, rstride=100, cstride=100)

ax.set_xlabel("PWM")
ax.set_ylabel("X")
ax.set_zlabel("F")

plt.show()
