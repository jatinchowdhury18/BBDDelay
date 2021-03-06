import numpy as np
import matplotlib.pyplot as plt

FS = 48000.0
Ts = 1.0 / FS
num = int(FS / 4)
FREQ = 120

time = np.arange(num) / FS
x = np.sin(2 * np.pi * FREQ * time)

class InputFilter:
    def __init__(self, root, pole):
        self.root = root
        self.pole = np.exp(pole * Ts)
        self.x = 0

    def calc_g(self, d_n):
        return Ts * self.root * self.pole**d_n

    def process(self, u):
        self.x = self.pole * self.x + u

class OutputFilter:
    def __init__(self, root, pole):
        self.g_coef = root / pole
        self.pole = np.exp(pole * Ts)
        self.x = 0

    def calc_g(self, d_n):
        return self.g_coef * self.pole**(1-d_n)

    def process(self, u):
        self.x = self.x + u

    def update(self):
        self.x = self.pole * self.x

N_filt = 5
input_filters = [InputFilter(251.589, -46.58),
                 InputFilter(-130.428 - 4165j, -55.482 + 25.082j),
                 InputFilter(-130.428 + 4165j, -55.482 - 25.082j),
                 InputFilter(4634 - 22.873j, -26.292 - 59.437j),
                 InputFilter(4634 + 22.873j, -26.292 + 59.437j)]

output_filters = [OutputFilter(5092, -176.261),
                  OutputFilter(-11.256 - 99.566j, -51.468 + 21.437j),
                  OutputFilter(-11.256 + 99.566j, -51.468 - 21.437j),
                  OutputFilter(-13.802 - 24.606j, -26.276 - 59.699j),
                  OutputFilter(-13.802 + 24.606j, -26.276 + 59.699j)]

H0 = 0
for f in output_filters:
    H0 += f.g_coef
H0 *= -1
H0 = np.real(H0)

FS_bbd = 1600.0
Ts_bbd = 1.0 / FS_bbd
num_bbd = int(FS_bbd / 4)
time_bbd = np.arange(num_bbd) / FS_bbd
x_bbd = np.zeros(num_bbd)

queue = 0
y_old = 0
y = np.zeros_like(x)

x_in = np.zeros(N_filt, dtype=np.complex)
x_out = np.zeros(N_filt, dtype=np.complex)
g_in = np.zeros(N_filt, dtype=np.complex)
n = 0
tn = 0
for k in range(num):
    output = 0
    while tn < Ts:
        if n == 0: # even
            for i in range(N_filt):
                x_in[i] = input_filters[i].x
                g_in[i] = input_filters[i].calc_g(tn)
            queue = 0.1 * np.real(np.sum(x_in * g_in))
            n = 1
        else: # odd
            delta = queue - y_old
            y_old = queue
            for i in range(N_filt):
                output_filters[i].process(output_filters[i].calc_g(tn) * delta)
            n = 0
        tn += Ts_bbd
    tn -= Ts

    for i in range(N_filt):
        input_filters[i].process(x[k])
    
    for i in range(N_filt):
        x_out[i] = output_filters[i].x
        output_filters[i].update()
    y[k] = (H0 * y_old + np.real(np.sum(x_out))) / 8
    # print(y[k])

plt.plot(time, x)
# plt.plot(time_bbd, x_bbd, '--')
plt.plot(time, y, '--')
# plt.xlim(0.2, 0.25)
plt.ylim(-2, 2)
plt.show()
