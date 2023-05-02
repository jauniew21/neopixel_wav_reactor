from math import log10
from scipy.io import wavfile
import wave
import matplotlib.pyplot as plt
import numpy as np

filename = 'test-audio.wav'
rate, data = wavfile.read(filename)
wavv = wave.open(filename, 'rb')
print(data)

length = data.shape[0] / rate
print(f"length = {length}s")
length = int(length)

print(f"rate = {rate}")
print(data[wavv.getnframes()-1][1])
print(f"frames = {wavv.getnframes()}")

data_per_sec = []
cont_count = 0
for i in range(1, length+1): # for each second
    data_per_sec.append([])

    for j in range(0, rate): # for each amp
        data_per_sec[i-1].append(data[cont_count][1])
        cont_count += 1

for i in range(0, length):
    print('[', data_per_sec[i][0], ' ', data_per_sec[i][rate-1], ']')

avg_db_per_sec = []
for i in range(0, length):
    avg_db_per_sec.append(sum(data_per_sec[i]) / len(data_per_sec[i]))
    avg_db_per_sec[i] = abs(20 * log10(abs(avg_db_per_sec[i])))
    print(avg_db_per_sec[i])

# for i in range(0, length):
#     print(data[i][1])

#plotting the audio time and amplitude
# time = np.linspace(0., length, data.shape[0])
# plt.plot(time, data, label="Audio")
# plt.legend()
# plt.xlabel("Time [s]")
# plt.ylabel("Amplitude")
# plt.show()

# --------------------------------------
# sending parsed wav data to the arduino
# --------------------------------------
def sendWav(avg_db_per_sec: list):
    ser.write(str(len(avg_db_per_sec)).encode())

    for i in range(0, length):
        time.sleep(0.8)
        ser.write(str(avg_db_per_sec[i]).encode())

# ---------------------------
# sending info to the arduino
# ---------------------------

import serial
import time

print()

serPort = "COM7"
baudRate = 9600

ser = serial.Serial(serPort, baudRate, timeout=.1)
print ("Serial port " + serPort + " opened Baudrate " + str(baudRate))
time.sleep(1.6)
ser.write('r'.encode())
sendWav(avg_db_per_sec)
time.sleep(1.6)

ser.close()

# Rebooting, no idea why I have to open and close it again, but at least it doesn't get stuck
ser = serial.Serial(serPort, baudRate, timeout=.1)
ser.close()