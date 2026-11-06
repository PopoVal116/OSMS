import math
import numpy as np
import matplotlib.pyplot as plt

#Исходные данные
TxPowerBS=46
sectors=3
TxPowerUE=24
AntGainBS=21
PenetrationM=15
InterfM=1
f_GHz=1.8
f_MHz = 1800
BW_UL=10e6
BW_DL=20e6
NoiseFigureBS=2.4
NoiseFigureUE=6
SINR_DL=2
SINR_UL=4
MIMO=2
S_total=100
S_business=4

#1
FeederLoss=2.9
MIMOGain=3
IM=InterfM
RequiredSINR_UL=SINR_UL
ThermalNoiseUL=-174+10*math.log10(BW_UL)
RxSensBS=NoiseFigureBS+ThermalNoiseUL+RequiredSINR_UL
MAPL_UL=TxPowerUE-FeederLoss+AntGainBS+MIMOGain-IM-PenetrationM-RxSensBS

print("Тепловой шум UL =", ThermalNoiseUL, "дБм")
print("Rx чувствительность BS =", RxSensBS, "дБм")
print("MAPL UL =", MAPL_UL, "дБ")

#2
ThermalNoiseDL=-174+10*math.log10(BW_DL)
RxSensUE=NoiseFigureUE+ThermalNoiseDL+SINR_DL
MAPL_DL=TxPowerBS-FeederLoss+AntGainBS+MIMOGain-IM-PenetrationM-RxSensUE
print("Тепловой шум DL =", ThermalNoiseDL, "дБм")
print("Чувствительность UE =", RxSensUE, "дБм")
print("MAPL DL =", MAPL_DL, "дБ")

#3
d=np.linspace(10, 5000, 500)
c=3e8
f_Hz=f_GHz*1e9
PL_FS=20*np.log10((4*np.pi*d*f_Hz)/c)
print("Потери FS для первого расстояния:", PL_FS[0], "дБ")
print("Потери FS для последнего расстояния:", PL_FS[-1], "дБ")

#COST-231 Hata
hBS=30    #высотаБС,м
hms=1.5   #высотаUE,м
A=46.3
B=33.9
a=3.2*(np.log10(11.75*hms))**2-4.97
Lclutter=0
d_km=d/1000
s = []

for distance in d_km:
    if distance>=1:
        s_value=44.9-6.55*math.log10(f_MHz)
    else:
        s_value=(47.88+13.9*math.log10(f_MHz)-13.9*math.log10(hBS))/math.log10(50)

    s.append(s_value)
PL_COST231 = []
for i in range(len(d_km)):
    PL=A+B*math.log10(f_MHz)-13.82*math.log10(hBS)-a+s[i]*math.log10(d_km[i])+Lclutter

    PL_COST231.append(PL)

print("Потери COST231 для первого расстояния:", PL_COST231[0], "дБ")
print("Потери COST231 для последнего расстояния:", PL_COST231[-1], "дБ")

#UMiNLOS
PL_UMiNLOS=26*np.log10(f_GHz)+22.7+36.7*np.log10(d)

print("Потери UMiNLOS для первого расстояния:", PL_UMiNLOS[0], "дБ")
print("Потери UMiNLOS для последнего расстояния:", PL_UMiNLOS[-1], "дБ")



#4
R_UL=0

for dist in np.arange(0.01, 20.01, 0.01):
    if dist>=1:
        s_value=44.9-6.55*math.log10(f_MHz)
    else:
        s_value=(47.88+13.9*math.log10(f_MHz)-13.9*math.log10(hBS))/math.log10(50)
    PL=A+B*math.log10(f_MHz)-13.82*math.log10(hBS)-a+s_value*math.log10(dist)+Lclutter

    if PL>=MAPL_UL:
        R_UL=dist
        break
print("Радиус БС в UL =", R_UL, "км")
R_DL=0
for dist in np.arange(0.01, 20.01, 0.01):
    if dist>=1:
        s_value=44.9-6.55*math.log10(f_MHz)
    else:
        s_value=(47.88+13.9*math.log10(f_MHz)-13.9*math.log10(hBS))/math.log10(50)
    PL=A+B*math.log10(f_MHz)-13.82*math.log10(hBS)-a+s_value*math.log10(dist)+Lclutter

    if PL>=MAPL_DL:
        R_DL=dist
        break
print("Радиус БС в DL =", R_DL, "км")

R = min(R_UL, R_DL)
print("Радиус БС =", R, "км")
S_BS=1.95*R**2
print("Площадь покрытия одной БС =", S_BS, "км^2")
N=S_total/S_BS
print("Расчётное количество БС =", N)
N_sites=math.ceil(N)
print("Требуемое количество БС =", N_sites)


plt.plot(d, PL_FS, label="Свободное пространство")
plt.plot(d, PL_COST231, label="COST-231 Hata")
plt.plot(d, PL_UMiNLOS, label="UMiNLOS")

plt.xlabel("Расстояние, м")
plt.ylabel("Потери PL(d), дБ")
plt.title("Зависимость потерь радиосигнала от расстояния")
plt.legend()
plt.grid()
plt.show()