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
x_0 = .50
R = 28  # Ohms
g = 9.8  # m/s^2
u_0 = 4 * np.pi * 10**(-7)
# TODO: Check this value
N = 1500  # number of turns
EM_DIAMETER = 34 # mm
A = np.pi * (EM_DIAMETER / 100/ 2) ** 2  # m^2
i_0 = np.sqrt((x_0**2 + 1)/(N**2*u_0*A**2))
L = 0.051337  # H
v_t = 0
breakpoint()

t_start = 0
t_end = .05
num_points = int(np.ceil((t_end - t_start) / dt))
t = np.linspace(t_start, t_end, num_points)

x = np.zeros(num_points)
x[0] = x_0
x[1] = x_0
x[2] = x_0

print(num_points)
for curr_t in range(0, num_points - 3):
    d2xdt2 = (x[curr_t + 2] - 2 * x[curr_t + 1] + x[curr_t]) / dt**2
    dxdt = (x[curr_t + 1] - x[curr_t]) / dt
    x[curr_t + 3] = (
        3 * x[curr_t + 2]
        - 3 * x[curr_t + 1]
        - x[curr_t]
        - m * x_0 * R * d2xdt2 * dt**3
        + 2 * m * g * x_0 * L * dxdt * dt**3
        + 2 * m * g * x_0 * R * dt**3
        - N**2 * u_0 * A**2 * i_0 * v_t * dt**3
    ) / (m * x_0 * L)

plt.plot(t, x)
plt.xlim(t_start, t_end)
# plt.ylim(-1, 1)
plt.show()
breakpoint()
