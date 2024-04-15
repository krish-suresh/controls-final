import numpy as np
import matplotlib.pyplot as plt

sweep = np.load("data/teensy_sweep.npy")
r_s = 20
r_e = 120
coefficients = np.polyfit(sweep[r_s:r_e, 0], sweep[r_s:r_e, 1], 1)
polynomial = np.poly1d(coefficients)
print(coefficients)
x = np.linspace(sweep[r_s:r_e, 0].min(), sweep[r_s:r_e, 0].max())
y = polynomial(x)
plt.plot(sweep[r_s:r_e, 0], sweep[r_s:r_e, 1], '.', label='Raw Data')
plt.plot(x, y, '-', label=f'Fitted Line m={coefficients[0]:.2f} b={coefficients[1]:.2f}')
plt.xlabel('Voltage Input [raw]')
plt.ylabel('Hall-Effect Sensor Measurement [raw]')
plt.title('Electromagnet Linearization')
plt.legend()
plt.savefig('data/line_fit.png')    
plt.show()