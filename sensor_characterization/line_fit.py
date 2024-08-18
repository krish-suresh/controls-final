import numpy as np
import matplotlib.pyplot as plt

sweep = np.load("data/sweep_20240430_174019.npy")
coefficients = np.polyfit(sweep[:, 0], sweep[:, 1], 1)
polynomial = np.poly1d(coefficients)
V_Q = 5*coefficients[1]/1023 # Volts
print(f"V_Q = {V_Q}")
u_0 = 4 * np.pi * 10e-7
L = 0.017 # m
print(coefficients[0])
print(coefficients[0] * L/u_0)
# y_data = ((((pwm_sweep_data[:, 1])/1023) * 5000) - V_Q*1000) / (12.5*(1+0.0012*(20-25)))

x = np.linspace(sweep[0, 0], sweep[-1, 0])
y = polynomial(x)
plt.plot(sweep[:, 0] + 150, sweep[:, 1], '.', label='Raw Data')
plt.plot(x + 150, y, '-', label=f'Fitted Line m={coefficients[0]:.2f} b={coefficients[1]:.2f}')
plt.xlabel('PWM Input [raw]')
plt.ylabel('Hall-Effect Sensor Measurement [raw]')
plt.title('Electromagnet Linearization')
plt.legend()
plt.savefig('data/line_fit.png')    
plt.show()
