import numpy as np
import matplotlib.pyplot as plt

#### Constants
# Measured from the arduino
dt_micros = 350  # microseconds per loop
dt = dt_micros / 10**6  # seconds
t_start = 0  # seconds
t_end = 1.05  # seconds
num_points = int(np.ceil((t_end - t_start) / dt))
t_list = np.linspace(t_start, t_end, num_points)

K_P = 100000.0

M_BIG_MAGNET = 0.5920  # g
M_MED_MAGNET = 0.2307  # g
M_SMALL_MAGNET = 0.0521  # g
M_NUT = 1.3406  # g

NUM_BIG_MAGNET = 3
NUM_MED_MAGNET = 5
NUM_SMALL_MAGNET = 20
NUM_NUT = 3

GROUND = 0.15  # m

m = (
    M_BIG_MAGNET * NUM_BIG_MAGNET
    + M_MED_MAGNET * NUM_MED_MAGNET
    + M_SMALL_MAGNET * NUM_SMALL_MAGNET
    + M_NUT * NUM_NUT
) / 1000  # kg
x_0 = 0.005  # m
R = 28.5  # Ohms
g = 9.8  # m/s^2
u_0 = 4 * np.pi * 10 ** (-7)
# TODO: Check this value
N = 1500  # number of turns
EM_DIAMETER = 0.034  # m
A = np.pi * (EM_DIAMETER / 2.0) ** 2  # m^2

pwm_setpoint = 200
v_0 = 200.0/255.0 * 12.0
i_0 = v_0/R  # amps
L = 0.051337  # H



v = np.zeros(num_points)
i = np.zeros(num_points)
x = np.zeros(num_points)
x[0] = x_0
x[1] = x_0
i[0] = i_0
v[0] = v_0
C_1 = -0.0000036625377  # Tuned based on experimental data
C_2 = (-(N**2) * u_0 * A**2) / 2.0

for t in range(0, num_points - 2):
    e =  x[t] - x_0
    v_i = np.clip(K_P * e + v_0, -12.0, 12.0)
    v[t + 1] = v_i
    i[t + 1] = i[t] + dt * (v_i - R * i[t]) / L
    x[t + 2] = np.clip(
        (C_1 / 2.0 + C_2 * i[t] ** 2) * ((dt**2) / (m * x[t] ** 2))
        - x[t]
        + (g * (dt**2))
        + 2 * x[t + 1],
        0.00001,
        GROUND,
    )
fig, axs = plt.subplots(3)

axs[0].plot(t_list, i, label='Current')
axs[0].set(xlabel='Time [s]', ylabel='Current [A]')
axs[0].legend()

axs[1].plot(t_list, v, label='Voltage')
axs[1].set(xlabel='Time [s]', ylabel='Voltage [V]')
axs[1].legend()

axs[2].plot(t_list, x, label=f"Non-linear model V={v_0:.2f}V")
# axs[1].hlines(
#     GROUND,
#     t_list[0],
#     t_list[-1],
#     colors="r",
#     linestyles="dashed",
#     label="Ground Position",
# )
axs[2].set(xlabel='Time [s]', ylabel='Magnet Position [m]')
axs[2].legend()

plt.savefig("nonlinear_model.png")
plt.show()
