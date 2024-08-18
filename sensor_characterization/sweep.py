import serial, time
import numpy as np
import matplotlib.pyplot as plt 
import datetime

arduino = serial.Serial()
arduino.port = "/dev/ttyACM0"
arduino.baudrate = 115200
arduino.setDTR(False)
arduino.setRTS(True)
arduino.open()
arduino.close()
time.sleep(2)
arduino.open()

arduino.write((0).to_bytes(1, 'big'))
arduino.write((254).to_bytes(1, 'big'))
time.sleep(3)

PWM_START = 0
PWM_END = 255
data = []
for i in range(0, PWM_END-PWM_START):
    v = 12 * i/(PWM_END-PWM_START)
    arduino.write((i).to_bytes(1, 'big'))
    reading = arduino.read(2)
    if len(reading) < 2:
        print("Incomplete data received")
    high_byte, low_byte = reading[0], reading[1]
    measurement = (high_byte << 8) | low_byte
    print(f"{v:.2f} \t {measurement}")
    if measurement:
        data.append([i, measurement])

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
np.save(f"data/sweep_{date_str}.npy", data)

data = np.array(data)
# Add datetime to plot
plt.plot(data[:, 0], data[:, 1])
plt.title(f"Sweep - {date_str}")
plt.xlabel("PWM Value")
plt.ylabel("Hall Effect Sensor Reading")

# Add datetime to savefig
plt.savefig(f"data/sweep_{date_str}_s{PWM_START}_e{PWM_END}.png")
plt.show()
