# Biblioteki
import csv
from matplotlib import pyplot

with open ("pomiar_kata.csv", 'r', newline = '') as plik:
    czytnik = csv.DictReader(plik)
    czas = []
    zadany = []
    zmierzony = []

    for wiersz in czytnik:
        czas.append(float(wiersz["czas_us"]))
        zadany.append(float(wiersz["zadany"]))
        zmierzony.append(float(wiersz["zmierzony_deg"]))


czas_ms = [(t - czas[0]) / 1000 for t in czas]

pyplot.plot(czas_ms, zadany, label="zadany")
pyplot.plot(czas_ms, zmierzony, label="zmierzony")
pyplot.xlabel("czas [ms]")
pyplot.ylabel("kąt [°]")
pyplot.title("Odpowiedź skokowa serwa")
pyplot.legend()
pyplot.grid(True)
pyplot.show()