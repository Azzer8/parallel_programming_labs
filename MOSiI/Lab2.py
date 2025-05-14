import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft, ifft, fftshift

# signal
x = np.array([-1, 2, 0, 1, 3, 3, 2, 0])

# Z-form:
# X(z) = 1 + 2z^(-1)+z^(-3), where z = exp(-jwT)

# Forward FFT
N = 512

xFFT = fftshift(fft(x, N))
# Magnitude spectrum
xA = np.abs(xFFT)
# Phase spectrum
xF = np.angle(xFFT)

# Complex
xReal = np.real(xFFT)
xImag = np.imag(xFFT)

# List of signals
xT = [x, xA, xF, np.array([xReal, xImag]).T]
lst_title = ['Signal', 'Spectrum', 'Phase', 'Real / Imag']

# Plot results
fig = plt.figure(figsize=(12, 6), dpi=80)
for i, sig in enumerate(xT):
    plt.subplot(2, 2, i+1)
    plt.ylabel('Level')
    plt.title(lst_title[i])
    if i == 0:
        plt.stem(sig, basefmt='C0')
        plt.xlabel('Time samples')
        plt.xlim([-0.25, len(x)-0.75])
    else:
        plt.plot(sig)
        plt.xlabel('Freq samples')
        plt.xlim([0, N-1])
    plt.grid()
plt.tight_layout()
plt.show()


##############################################################
N = 9    # Signal period
M = 4    # Number of repeats

# Signal
x = np.zeros(N)
x[0:3] = 1
y = np.tile(x, M)

# Forward FFT
NFFT = 1024
xFFT = fft(y, NFFT)
# Magnitude spectrum
yA = np.abs(xFFT)
print('Max amplitude = sum of ones * M = {}'.format(yA.max()))
yA /= np.max(yA)
# Phase spectrum
yF = np.angle(xFFT)


# FFT for input signal
xFFT = fft(x, NFFT)
# Magnitude spectrum
xA = np.abs(xFFT)
xA /= np.max(xA)

# FFT for repeats
xFFT = fft(x, NFFT//N)
# Magnitude spectrum
xM = np.abs(xFFT)
xM /= np.max(xM)
xM = np.tile(xM, N)
#List of signals
xT = [yA]
lst_title = ['Spectrum']

# Plot results
fig = plt.figure(figsize=(10, 5), dpi=100)
for i, sig in enumerate(xT):
    plt.subplot(2, 1, int(2**i))
    plt.ylabel('Level')
    plt.title(lst_title[i])
    plt.plot(sig, '-', linewidth=2.5, label='Result Spectrum y(t)')
    plt.plot(xA, '--', linewidth=1.5, label='Input Spectrum x(t)')
    plt.plot(xM, '-.', linewidth=1.5, label='Repeats by M = %d' % M)
    plt.xlabel('Freq samples')
    plt.xlim([0, NFFT-1])
    plt.legend(loc='upper right')
    plt.grid()
plt.tight_layout()
plt.show()
