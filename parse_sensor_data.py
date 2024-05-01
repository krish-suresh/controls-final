import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

data = pd.read_csv('data/sensor_data_200pwm.csv')

# Transform the data
x_data = data['Gap Distance (mm) from sensor'].to_numpy()/1000.0
y_data = data['Analog Reading'].to_numpy()

# x_data =(x_data-512)/512.0
y_data = (((y_data/1023) * 5000) - 2500) / (12.5*(1+0.0012*(20-25)))


coefficients = np.polyfit(x_data, y_data, 8)
polynomial = np.poly1d(coefficients)


def exp_decay(x, a, b, c):
    return np.log((x - c)/a)/-b

def sqrt_inv(x, a, b):
    return a / x + b


# Fit the function to the data
popt_exp, pcov_exp = curve_fit(exp_decay, x_data, y_data, p0=(1000, 1, 10))
popt_sqrt, pcov_sqrt = curve_fit(sqrt_inv, x_data, y_data)



# Plot the fitted polynomial
x = np.linspace(min(x_data), max(x_data), 1000)

START_IDX = 512
END_IDX = 1022
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
plt.plot(x, polynomial(x), label='Polynomial Fit')
# plt.plot(x_data, exp_decay(x_data, *popt_exp), label='Exponential Decay Fit', color='red')
# plt.plot(x_data, sqrt_inv(x_data, *popt_sqrt), label='Sqrt Fit', color='green')
PM_GAP = 0.005
print(f" Flux at {PM_GAP}m gap: {polynomial(PM_GAP)/1000:.5f} T")

plt.xlabel('Gap Distance (m) from sensor')
plt.ylabel('Magnetic Flux [mT]')
plt.legend()
plt.savefig('sensor_data_fit.png')
plt.show()