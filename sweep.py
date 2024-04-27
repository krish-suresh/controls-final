import serial, time
import numpy as np
import matplotlib.pyplot as plt 
import datetime

arduino = serial.Serial()
arduino.port = "/dev/ttyACM0"
arduino.baudrate = 9600
arduino.timeout = 1
arduino.setDTR(False)
arduino.setRTS(True)
arduino.open()
arduino.close()
time.sleep(2)
arduino.open()

arduino.write((0).to_bytes(1, 'big'))
arduino.write((254).to_bytes(1, 'big'))
measurement = arduino.readline()
time.sleep(3)
# breakpoint()
data = []
for i in reversed(range(0, 100)):
    # v = -12 * i/255
    v = -i / 100 * 255
    # arduino.write((0).to_bytes(1, 'big'))
    arduino.write((i).to_bytes(1, 'big'))
    measurement = arduino.readline()
    print(f"{v:.2f} \t {measurement}")
    if measurement:
        data.append([v, int(measurement.strip())])

# for i in range(0, 100):
#     # v = 12 * i/255
#     v = i
#     arduino.write((i).to_bytes(1, 'big'))
#     arduino.write((0).to_bytes(1, 'big'))
#     measurement = arduino.readline()
#     print(f"{v:.2f} \t {measurement}")
#     if measurement:
#         data.append([v, int(measurement.strip())])


# Get current datetime
now = datetime.datetime.now()
date_str = now.strftime("%Y%m%d_%H%M%S")

# Add datetime to numpy save
np.save(f"sweep_{date_str}.npy", data)

# Add datetime to plot
plt.plot(data[:, 0], data[:, 1])
plt.title(f"Sweep - {date_str}")
plt.xlabel("Voltage (V)")
plt.ylabel("Hall Effect Sensor Reading")

# Add datetime to savefig
plt.savefig(f"sweep_{date_str}.png")
plt.show()