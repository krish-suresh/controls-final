import numpy as np
import matplotlib.pyplot as plt

#### Constants
# Measured from the arduino
dt_micros = 350  # microseconds per loop
dt = dt_micros / 10**6  # seconds

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
i_0 = 0.0  # amps
L = 0.051337  # H
v_t = 12.0  # volts

t_start = 0  # seconds
t_end = 1.05  # seconds
num_points = int(np.ceil((t_end - t_start) / dt))
t_list = np.linspace(t_start, t_end, num_points)


i = np.zeros(num_points)
x = np.zeros(num_points)
x[0] = x_0
x[1] = x_0
i[0] = i_0
C_1 = 0.0  # Tuned based on experimental data
C_2 = (-(N**2) * u_0 * A**2) / 2.0

for t in range(0, num_points - 2):
    i[t + 1] = i[t] + dt * (v_t - R * i[t]) / L
    x[t + 2] = np.clip(
        (C_1 / 2.0 + C_2 * i[t] ** 2) * ((dt**2) / (m * x[t] ** 2))
        - x[t]
        + (g * (dt**2))
        + 2 * x[t + 1],
        0,
        GROUND,
    )


plt.plot(t_list, x, label=f"Non-linear model V={v_t:.2f}V")
plt.hlines(
    GROUND,
    t_list[0],
    t_list[-1],
    colors="r",
    linestyles="dashed",
    label="Ground Position",
)
plt.xlabel("Time [s]")
plt.ylabel("Magnet Position [m]")
plt.legend()
plt.savefig("nonlinear_model.png")
plt.show()
