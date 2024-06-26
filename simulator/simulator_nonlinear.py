import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Button, Slider

#### Constants
# Measured from the arduino
dt_micros = 350  # microseconds per loop
dt = dt_micros / 10**6  # seconds
t_start = 0  # seconds
t_end = 5.05  # seconds
num_points = int(np.ceil((t_end - t_start) / dt))
t_list = np.linspace(t_start, t_end, num_points)

K_P = 18000.0
K_D = 4.5

M_BIG_MAGNET = 0.5920  # g
M_MED_MAGNET = 0.2307  # g
M_SMALL_MAGNET = 0.0521  # g
M_NUT = 1.3406  # g

NUM_BIG_MAGNET = 3
NUM_MED_MAGNET = 5
NUM_SMALL_MAGNET = 20
NUM_NUT = 3

MIN_DIST = 0.00001  # m
MAX_DIST = 0.15  # m

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
C_1 = -0.0000036625377  # Tuned based on experimental data
C_2 = (-(N**2) * u_0 * A**2) / 2.0

def simulate(K_P, K_D):
    v = np.zeros(num_points)
    i = np.zeros(num_points)
    x = np.zeros(num_points)
    x[0] = x_0
    x[1] = x_0
    i[0] = i_0
    v[0] = v_0
    e_prev = 0

    for t in range(0, num_points - 2):
        e = x[t] - x_0
        v_i = np.clip(K_P * e + K_D * (e - e_prev)/dt + v_0, -12.0, 12.0)
        v[t + 1] = v_i
        i[t + 1] = i[t] + dt * (v_i - R * i[t]) / L
        x[t + 2] = np.clip(
            (C_1 / 2.0 + C_2 * i[t] ** 2) * ((dt**2) / (m * x[t] ** 2))
            - x[t]
            + (g * (dt**2))
            + 2 * x[t + 1],
            MIN_DIST,
            MAX_DIST,
        )
    i[-1] = i[-2] 
    return i, x

i, x = simulate(K_P, K_D)

fig, axs = plt.subplots(2)

line_i, = axs[0].plot(t_list, i, label='Current')
axs[0].set(xlabel='Time [s]', ylabel='Current [A]')
axs[0].legend()

line_x, = axs[1].plot(t_list, x, label=f"Non-linear model V={v_0:.2f}V")
# axs[1].hlines(
#     GROUND,
#     t_list[0],
#     t_list[-1],
#     colors="r",
#     linestyles="dashed",
#     label="Ground Position",
# )
axs[1].set(xlabel='Time [s]', ylabel='Magnet Position [m]')
axs[1].legend()

fig.subplots_adjust(bottom=0.25)

axkp = fig.add_axes([0.25, 0.1, 0.65, 0.03])
kp_slider = Slider(
    ax=axkp,
    label='KP',
    valmin=0,
    valmax=200000,
    valinit=K_P,
)
axkd = fig.add_axes([0.25, 0.05, 0.65, 0.03])
kd_slider = Slider(
    ax=axkd,
    label='KD',
    valmin=0,
    valmax=30,
    valinit=K_D,
)

def update(val):
    i, x = simulate(kp_slider.val, kd_slider.val)
    line_i.set_ydata(i)
    line_x.set_ydata(x)
    # axs[0].set_ylim(min(i), max(i))
    # axs[1].set_ylim(min(x), max(x))
    fig.canvas.draw_idle()

axs[1].set_ylim(MIN_DIST, x_0*2)

kp_slider.on_changed(update)
kd_slider.on_changed(update)

# plt.tight_layout()
plt.savefig("nonlinear_model.png")
plt.show()
