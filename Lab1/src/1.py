import numpy  as np
import matplotlib.pyplot as plt
import wave
import sounddevice as sd


#-------------С аудио работа--------------
with wave.open('voice.wav', 'rb') as w:
    Fs=w.getframerate()
    N=w.getnframes()
    duraction=N/Fs
    raw=w.readframes(N)
print(Fs, duraction, N)
yv=np.frombuffer(raw, dtype=np.int16)
Fs_ras=N/10.38
print(Fs_ras)

#----Прорежиание
yv_new=yv[::10]
Fs_new=Fs//10
print(f"Было : {len(yv)} отсч., Fs = {Fs} Гц")
print(f"Стало: {len(yv_new)} отсч., Fs = {Fs_new} Гц")
sd.play(yv,  Fs)
sd.wait()
sd.play(yv_new, Fs_new)
sd.wait()

t_voise=np.arange(len(yv))/Fs
t_voise_new=np.arange(len(yv_new))/Fs_new

''' ДПФ руками для аудио
N_voice=len(yv)
F_voice=[]
for x in range(N_voice):
    s=0j
    for n in range(N_voice):
        s+=yv[n]*np.exp(-2j*np.pi*x*n/N_voice)
    F_voice.append(s)
F_voice=np.array(F_voice)

N_voice_new=len(yv_new)
F_voice_new=[]
for x in range(N_voice_new):
    s=0j
    for n in range(N_voice_new):
        s+=yv_new[n]*np.exp(-2j*np.pi*x*n/N_voice_new)
    F_voice_new.append(s)
F_voice_new=np.array(F_voice_new)
'''

#-------ДПФ и спектры голоса
F_voice=np.fft.fft(yv)
F_voice_new=np.fft.fft(yv_new)
N_v=len(yv)
N_n=len(yv_new)
freqs_voice=np.arange(N_v//2)*Fs/N_v
freqs_new=np.arange(N_n//2)*Fs_new/N_n

mag_voice=np.abs(F_voice[:N_v//2])
mag_new=np.abs(F_voice_new[:N_n //2])


#-------Квантование
signal=yv.astype(float)
def quantize(signal, bits):
    levels=2**bits-1
    signal_min=signal.min()
    signal_max=signal.max()
    signal_norm=(signal-signal_min)/(signal_max-signal_min)
    signal_quant=np.round(signal_norm*levels)
    signal_quant=np.clip(signal_quant, 0, levels)
    return signal_quant


#----------Тестовый сигнал со случайным SR для плавности
f=20 #максимальная частота
SR=300 #число отсчетов

t=[] #t=k*(1/SR) моменты времени
for k in range(SR):
    t.append(k*(1/SR))
t=np.array(t)
y=7*np.cos(2*np.pi*f*t+(np.pi/3))


#------- Оцифровка и восстановление
fs=f*2 #минимальная частота дискретизации
ts=[]
for k in range(fs):
    ts.append(k*(1/fs))
ts = np.array(ts)
ys=7*np.cos(2*np.pi*f*ts+(np.pi/3))

#----ДПФ
N=len(ys)
F=[]
for x in range(N):
    s=0j
    for n in range(N):
        s+=ys[n]*np.exp(-2j*np.pi*x*n/N)
    F.append(s)
F=np.array(F)
print(F)


#---------Всё то же самое для частоты диск. х4
fs4=fs*4
t4=[]
for i in range(fs4):
    t4.append(i*(1/fs4))
t4=np.array(t4)
y4=7*np.cos(2*np.pi*f*t4+(np.pi/3))

#ошибка при квантовании
sig = y4.astype(float)
F_orig = np.fft.fft(sig)
N_sig = len(sig)
freqs = np.arange(N_sig//2) * (fs4) / N_sig
mag_orig = np.abs(F_orig[:N_sig//2])

plt.figure(figsize=(14, 10))
plt.subplot(3, 2, 1)
plt.plot(freqs, mag_orig)
plt.title('Спектр исходного сигнала (без квантования)')
plt.grid(True)

for i, bits in enumerate([3, 4, 5, 6]):
    y_q = quantize(sig, bits)
    F_q = np.fft.fft(y_q)
    mag_q = np.abs(F_q[:N_sig // 2])
    levels=2**bits-1
    signal_norm=((sig-sig.min())/(sig.max()-sig.min())*levels)
    error=np.mean(np.abs(y_q-signal_norm))
    print(f'{bits} бит: 'f'средняя ошибка квантования = {error:.4f}')
    plt.subplot(3, 2, i + 2)
    plt.plot(freqs, mag_q)
    plt.title(f'Спектр после квантования {bits} бит')
    plt.grid(True)

N4=len(y4)
F4=[]
for x in range(N4):
    s=0j
    for n in range(N4):
        s+=y4[n]*np.exp(-2j*np.pi*x*n/N4)
    F4.append(s)
F4=np.array(F4)


print('Объемы памяти')
print(f'Одного элемента массива F {F.itemsize} байт')
print(f'Всего массива F {F.nbytes} байт')

#------Графики сигналов
plt.figure(figsize=(12, 5))
plt.subplot(3,1 ,1)
plt.plot(t, y, 'b-')
plt.grid(True)
plt.title('Оригинальный аналоговый сигнал')
plt.xlabel('t, с')

plt.subplot(3,1 ,2)
plt.plot(ts, ys, 'g-')
plt.grid(True)
plt.title('Восстановленный сигнал (после оцифровки)')
plt.xlabel('t, с')

plt.subplot(3,1 ,3)
plt.plot(t4, y4, 'r-')
plt.grid(True)
plt.title('Оригинальный аналоговый сигнал с частотой дискретизации 160')
plt.xlabel('t, с')

#------Графики голоса
plt.figure(figsize=(14,8))
plt.subplot(2,1,1)
plt.title('Оригинальный звук')
plt.plot(t_voise, yv)
plt.xlabel('Время, с')
plt.ylabel('Амплитуда')
plt.subplot(2,1,2)
plt.title('Прорежженный звук')
plt.plot(t_voise_new, yv_new)
plt.xlabel('Время, с')
plt.ylabel('Амплитуда')

#----Амплитудные спектры
plt.figure(figsize=(14,8))
plt.subplot(2,1,1)
plt.title('Спектр ориг')
plt.plot(freqs_voice, mag_voice)
plt.xlabel('Частота')
plt.subplot(2,1,2)
plt.title('Спектр прореженного')
plt.plot(freqs_new, mag_new)
plt.xlabel('Частота')
plt.ylabel('Амплитуда')


plt.tight_layout()
plt.show()