import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

FS = 48000.0
FREQ = 9000
omega = 2 * np.pi * FREQ

r = np.array([251589, -130428 - 4165j, -130428 + 4165j, 4634 - 22873j, 4634 + 22873j])
p = np.array([-46580, -55482 + 25082j, -55482 - 25082j, -26292 - 59437j, -26292 + 59437j])

r = np.array([5092.0, -11256.0 - 99566.0j, -11256.0 + 99566.0, -13802.0 - 24606.0j, -13802.0 + 24606.0j])
p = np.array([-176261.0, -51468.0 - 21437.0j, -51468.0 + 21437.0j, -26276.0 - 59699.0j, -26276.0 + 59699.0j])

H0 = 0
for i in range(5):
    prod = r[i]
    for k in range(5):
        if i == k:
            continue
        prod *= p[k]
    H0 += prod

print(H0)

# print(z)
# print(p)

worN=np.logspace(1, 5, 1000)
w, h = signal.freqs_zpk([], p, H0, worN)
plt.figure()
plt.semilogx(w, 20 * np.log10(abs(h)))

fc = 500.0
# freq_factor = fc / 9400
freq_factor = fc / 11000
r = r * freq_factor
p = p * freq_factor

H0 = 0
for i in range(5):
    prod = r[i]
    for k in range(5):
        if i == k:
            continue
        prod *= p[k]
    H0 += prod

w, h = signal.freqs_zpk([], p, H0, worN)
plt.semilogx(w, 20 * np.log10(abs(h)))

z, p, k = signal.butter(5, 2 * np.pi * fc, analog=True, output='zpk')
w, h = signal.freqs_zpk(z, p, k, worN)
plt.semilogx(w, 20 * np.log10(abs(h)))

plt.xlabel('Frequency')
plt.ylabel('Amplitude response [dB]')
plt.ylim(-90)
plt.grid()

# plt.figure()
# plt.plot(z.real, z.imag, 'go')
# plt.plot(p.real, p.imag, 'rx')
# plt.grid()
plt.show()
