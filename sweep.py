import serial, time
import numpy as np
import matplotlib.pyplot as plt 

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

data = []
for i in reversed(range(0, 255)):
    # v = -12 * i/255
    v = -i
    arduino.write((0).to_bytes(1, 'big'))
    arduino.write((i).to_bytes(1, 'big'))
    measurement = arduino.readline()
    print(f"{v:.2f} \t {measurement}")
    if measurement:
        data.append([v, int(measurement.strip())])

for i in range(0, 255):
    # v = 12 * i/255
    v = i
    arduino.write((i).to_bytes(1, 'big'))
    arduino.write((0).to_bytes(1, 'big'))
    measurement = arduino.readline()
    print(f"{v:.2f} \t {measurement}")
    if measurement:
        data.append([v, int(measurement.strip())])


data = np.array(data)
np.save("sweep.npy", data)
plt.plot(data[:, 0], data[:, 1])
plt.xlabel("Voltage (V)")
plt.ylabel("Hall Effect Sensor Reading")
plt.savefig("sweep.png")
plt.show()