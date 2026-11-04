# Biblioteki
import csv
from pathlib import Path
import sys

def znajdz_skoki(zadany):
    skoki = []
    for i in range(1, len(zadany)):
        if zadany[i] != zadany[i-1]:
            skoki.append(i)
    return skoki

def amplituda_skoku(zadany, zmierzony, indeksy):
    amplitudy = []
    for i in indeksy:
        poziom_przed = sum(zmierzony[i-20:i]) / len(zmierzony[i-20:i])
        cel = zadany[i]
        amplituda = cel - poziom_przed
        amplitudy.append(amplituda)

    return amplitudy

def opoznienie(czas, zmierzony, i, poziom_przed, prog=0.5):
    for j in range(i, len(zmierzony)):
        if abs(zmierzony[j] - poziom_przed) >= prog:
            return czas[j] - czas[i]
    return None

def czas_do_ulamka(czas, zmierzony, zadany, i, poziom_przed, ulamek):
    cel = zadany[i]
    amplituda = abs(cel - poziom_przed)
    for j in range(i, len(zmierzony)):
        if abs(zmierzony[j] - poziom_przed) >= ulamek * amplituda:
            return czas[j] - czas[i]
    return None

def poziom_ustalony(zmierzony, i_koniec, n=100):
    return sum(zmierzony[i_koniec-n:i_koniec]) / n

def przeregulowanie(zmierzony, zadany, i, i_koniec, poziom_przed):
    if (zadany[i] > poziom_przed):
        kierunek = 1
        ekstremum = max(zmierzony[i:i_koniec])
    else:
        kierunek = -1
        ekstremum = min(zmierzony[i:i_koniec])
    przeregulowanie_deg = max(0, kierunek * (ekstremum - zadany[i]))
    return przeregulowanie_deg, (przeregulowanie_deg / abs(zadany[i] - poziom_przed)) * 100

def max_nachylenie(czas, zmierzony, i, i_koniec, k=3):
    najwieksze = 0
    for j in range(i, i_koniec - k):
        nachylenie = abs(zmierzony[j+k] - zmierzony[j]) / (czas[j+k] - czas[j])
        if nachylenie > najwieksze:
            najwieksze = nachylenie
    return najwieksze

folder = Path(__file__).parent.parent / "dane"
if len(sys.argv) > 1:
    plik_csv = folder / sys.argv[1]
else:
    plik_csv = sorted(folder.glob("pomiar_2*.csv"))[-1]

czas = []
zadany = []
zmierzony = []

with open (plik_csv, 'r', newline = '') as plik:
    czytnik = csv.DictReader(plik)
    for wiersz in czytnik:
        czas.append(float(wiersz['czas_us']) / 1e6)
        zadany.append(float(wiersz['zadany']))
        zmierzony.append(float(wiersz['zmierzony_deg']))

skoki = znajdz_skoki(zadany)
granice = skoki + [len(zmierzony)]
wyniki = []

for k in range(len(skoki)):
    i = skoki[k]
    i_koniec = granice[k + 1]
    poziom_przed = sum(zmierzony[i-20:i]) / 20
    amp = zadany[i] - poziom_przed
    d = opoznienie(czas, zmierzony, i, poziom_przed)
    t63 = czas_do_ulamka(czas, zmierzony, zadany, i, poziom_przed, 0.632)
    ust = poziom_ustalony(zmierzony, i_koniec)
    uchyb = zadany[i] - ust
    deg, proc = przeregulowanie(zmierzony, zadany, i, i_koniec, poziom_przed)
    nach = max_nachylenie(czas, zmierzony, i, i_koniec)
    
    print(f"{czas[i]:5.1f}s  amp={amp:7.2f}  opozn={d*1000:4.0f}ms  t63={t63*1000:4.0f}ms  uchyb={uchyb:7.3f}  przereg={deg:.3f} nach={nach:.0f}°/s")
    
    wyniki.append({
        "t_skoku_s": czas[i],
        "amplituda_deg": round(amp, 4),
        "opoznienie_ms": round(d * 1000, 1),
        "t63_ms": round(t63 * 1000, 1),
        "uchyb_deg": round(uchyb, 4),
        "przereg_deg": round(deg, 4),
        "nachylenie_degs": round(nach, 4),
    })

plik_wyniki = Path(__file__).parent.parent / "dane" / "wyniki_identyfikacji.csv"
with open(plik_wyniki, 'w', newline='') as plik:
    pisarz = csv.DictWriter(plik, fieldnames=wyniki[0].keys())
    pisarz.writeheader()
    pisarz.writerows(wyniki)
print("Zapisano:", plik_wyniki)