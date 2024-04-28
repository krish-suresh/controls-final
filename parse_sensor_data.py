import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

data = pd.read_csv('data/sensor_data_200pwm.csv')

# Transform the data
x_data = data['Analog Reading'].to_numpy()
y_data = data['Gap Distance (mm) from sensor'].to_numpy()/1000.0
pwm_value = .8447 * 200 + 400
x_data = x_data - pwm_value


# Fit a polynomial of degree 2 to the data
coefficients = np.polyfit(x_data, y_data, 8)
polynomial = np.poly1d(coefficients)


def exp_decay(x, a, b, c):
    # return a * np.exp(-b * x) + c
    return np.log((x - c)/a)/-b


# Fit the function to the data
popt_exp, pcov_exp = curve_fit(exp_decay, x_data, y_data)



# Plot the fitted polynomial
x = np.linspace(min(x_data), max(x_data), 1000)

START_IDX = 0
END_IDX = 508
with open('PD_controller/sensor_data.h', 'w') as f:
    f.write("#ifndef SENSOR_DATA_H\n")
    f.write("#define SENSOR_DATA_H\n")
    f.write(f"float SENSOR_LOOKUP_TABLE[{(END_IDX-START_IDX)//2  + 1}] = {{\n")
    for i in range(START_IDX, END_IDX, 2):
        f.write(f"\t{polynomial(i)},\n")
    f.write(f"\t{polynomial(END_IDX)}\n")
    f.write("};\n")
    f.write("#endif\n")

plt.plot(x_data, y_data, 'o', label='data')
plt.plot(x, polynomial(x), 'b-', label='fit: %s' % polynomial)
plt.plot(x_data, exp_decay(x_data, *popt_exp), label='Exponential Decay Fit', color='red')

plt.ylabel('Gap Distance (mm) from sensor')
plt.xlabel('Analog Reading')
plt.legend()
plt.show()
