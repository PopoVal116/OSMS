import numpy  as np
import sounddevice as sd
import matplotlib.pyplot as plt


Fs=1500
amp=0.3
dur=0.5
T=15
notes = {
    'C1': 261.63,#До
    'D1': 293.66,#Ре
    'E1': 329.63,#Ми
    'F1': 349.23,#Фа
    'G1': 392.00,#Соль
    'A1': 440.00,#Ля
    'B1': 493.88,#Си
    'C2': 523.25,#До(выше)
    'D2': 587.32,
    'E2': 659.26,
    'F2': 698.46,
    'G2': 784.00,
    'rest': 0
}

t=np.linspace(0, T, int(Fs*T), endpoint=False)
def note(name, duration):
    freq=notes[name]
    t=np.linspace(0, duration, int(Fs*duration), endpoint=False)
    return amp*np.sin(2*np.pi*freq*t)


melody = [
    note('C1', dur), note('A1', 2*dur), note('G1', dur), note('A1', dur),
    note('F1', dur), note('C1', dur),

    note('C1', dur), note('A1', 2*dur), note('A1', 0.5*dur), note('B1', 0.5*dur), note('G1', dur),
    note('C2', dur),

    note('C2', dur), note('D1', 2*dur), note('A1', 0.5*dur), note('B1', 0.5*dur), note('A1', 0.5*dur), note('B1', 0.5*dur),
    note('A1', dur), note('G1', dur),  note('F1', dur),

    note('F1', dur), note('A1', 2*dur), note('G1', dur), note('A1', dur), note('F1', dur),
]

song = np.concatenate(melody)
t=np.arange(len(song))/Fs

#sd.play(song, Fs)
#sd.wait()

plt.figure(figsize=(12, 4))
plt.plot(t[:1500], song[:1500], linewidth=0.4)
plt.title('Первая секунда сигнала')
plt.xlabel('t, с')
plt.ylabel('амплитуда')
plt.grid(True)

plt.figure(figsize=(12, 5))
plt.subplot(2,1 ,1)
plt.plot(t, song, 'b-', linewidth=0.2)
plt.grid(True)
plt.title('Сигнал')
plt.xlabel('t, с')

S_f = np.fft.rfft(song)
freqs = np.fft.rfftfreq(len(song), 1/Fs)

plt.subplot(2, 1, 2)
plt.plot(freqs, np.abs(S_f), 'r-')
plt.title('Амплитудный спектр')
plt.xlabel('f, Гц')
plt.ylabel('A')
plt.xlim(0, 1500)
plt.grid(True)

plt.tight_layout()
plt.show()