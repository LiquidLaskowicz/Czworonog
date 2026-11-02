import time
from datetime import datetime
from pathlib import Path

import serial

PORT = 'COM4'
BAUD = 115200

KATY = [0, 10, 0, -10, 0, 20, 0, -20, 0]   # kolejne kąty zadane [°]
CZAS_NA_KAT_S = 2.0                         # ile sekund trzymać każdy kąt

folder = Path(__file__).parent.parent / "dane"
folder.mkdir(exist_ok=True)
plik_wyjsciowy = folder / f"pomiar_{datetime.now():%Y%m%d_%H%M%S}.csv"

with serial.Serial(PORT, baudrate=BAUD, timeout=0.05) as ser, \
        open(plik_wyjsciowy, 'w', newline='') as plik:

    time.sleep(2)               # chwila na ustabilizowanie połączenia
    ser.reset_input_buffer()    # wyrzuć to, co przyszło wcześniej

    plik.write("czas_us,zadany,zmierzony_deg,surowy\n")

    for kat in KATY:
        ser.write(f"S {kat}\n".encode())
        koniec = time.monotonic() + CZAS_NA_KAT_S

        while time.monotonic() < koniec:
            surowe_dane = ser.readline()
            if not surowe_dane:
                continue
            dane = surowe_dane.decode('utf-8', errors='ignore').strip()
            print(dane)
            if len(dane.split(',')) == 4:
                plik.write(dane + '\n')

print("Zapisano:", plik_wyjsciowy)