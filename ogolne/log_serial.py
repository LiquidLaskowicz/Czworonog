# Biblioteki
import serial

ser = serial.Serial('COM4', baudrate=115200, timeout=1)

with open('pomiar_kata.csv', 'w') as plik:

    plik.write("czas_us,zadany,zmierzony_deg,surowy" + '\n')

    while (1):
        surowe_dane = ser.readline()
        if not surowe_dane:
            continue
        dane = surowe_dane.decode('utf-8').strip()
        print(dane)

        if len(dane.split(',')) == 4:
            plik.write(dane + '\n')
