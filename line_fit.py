import numpy as np
import matplotlib.pyplot as plt

sweep = np.load("data/sweep.npy")
r_s = 400
r_e = 500
coefficients = np.polyfit(sweep[r_s:r_e, 0], sweep[r_s:r_e, 1], 1)
polynomial = np.poly1d(coefficients)
print(coefficients)
x = np.linspace(sweep[r_s:r_e, 0].min(), sweep[r_s:r_e, 0].max())
y = polynomial(x)
plt.plot(sweep[r_s:r_e, 0], sweep[r_s:r_e, 1])
plt.plot(x, y, '-', label='Fitted line')
plt.show()