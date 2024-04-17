import numpy as np
import matplotlib.pyplot as plt


# Constants
# Measured from the arduino
dt_micros = 350
# Seconds
dt = dt_micros / 10**6


M_BIG_MAGNET = 0.5920
M_MED_MAGNET = 0.2307
M_SMALL_MAGNET = 0.0521
M_NUT = 1.3406
NUM_BIG_MAGNET = 0
NUM_MED_MAGNET = 0
NUM_SMALL_MAGNET = 1
NUM_NUT = 0

m = (
    M_BIG_MAGNET * NUM_BIG_MAGNET
    + M_MED_MAGNET * NUM_MED_MAGNET
    + M_SMALL_MAGNET * NUM_SMALL_MAGNET
    + M_NUT * NUM_NUT
) / 1000  # kg
x_0 = .005
R = 28.5  # Ohms
g = 9.8  # m/s^2
u_0 = 4 * np.pi * 10**(-7)
# TODO: Check this value
N = 1500  # number of turns
EM_DIAMETER = 0.034 # m
A = np.pi * (EM_DIAMETER / 2.0) ** 2  # m^2
i_0 = np.sqrt((x_0**2 + 1)/(N**2*u_0*A**2))
L = 0.051337  # H
v_t = 0

t_start = 0
t_end = .05
num_points = int(np.ceil((t_end - t_start) / dt))
t_list = np.linspace(t_start, t_end, num_points)


i = np.zeros(num_points)
x = np.zeros(num_points)
x[0] = x_0
x[1] = x_0
i[0] = i_0
C_1 = 0.0
C_2 = (-N**2 * u_0 * A**2)/2.0
for t in range(0, num_points - 2):
    i[t + 1] = i[t] + dt * (v_t - R * i[t]) / L
    x[t + 2] = x[t + 1] + (2*x[t + 1] - x[t] + (dt**2)*(2*m*g*x_0*x[t]+x_0**2 + C_1/2.0 + 2*C_2*i_0*i[t] - i_0**2 ))/(m*x_0**2)


plt.plot(t_list, x)
plt.show()