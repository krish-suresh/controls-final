from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt

CSV_PATH = Path(__file__).parent / "data/arduino_pwm_oscope.csv"

data = {}
curr_key = None

with open(CSV_PATH, "r") as file:
    for line in file.readlines():
        if not line[0].isnumeric():
            curr_key = line.strip()
            data[curr_key] = []
        else:
            data[curr_key].append([float(x) for x in line.strip().split(",")])

for key, value in data.items():
    data[key] = np.array(value)

plt.plot(data["arduino"][:, 0], data["arduino"][:, 1], "o")
plt.plot(data["teensy"][:, 0], data["teensy"][:, 1], "o")
plt.legend(["Arduino", "Teensy"])
plt.xlabel("Commanded Duty Cycle [raw counts]")
plt.ylabel("Experimental Duty Cycle [%]")
plt.show()
