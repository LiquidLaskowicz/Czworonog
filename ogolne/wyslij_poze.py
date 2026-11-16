import argparse
import sys
import time

import serial


def wczytaj_ramki(sciezka):
    ramki = []
    with open(sciezka, encoding="utf-8-sig") as plik:
        for nr, linia in enumerate(plik, 1):
            linia = linia.strip()
            if not linia or linia.startswith("#"):
                continue
            if not linia.startswith("S "):
                raise ValueError(f"{sciezka}:{nr}: to nie jest ramka S: {linia!r}")
            ramki.append(linia)
    if not ramki:
        raise ValueError(f"{sciezka}: pusty plik")
    return ramki


def wyslij(lacze, linia):
    lacze.write((linia + "\n").encode("ascii"))


def odczytaj(lacze, czas):
    """Wypisuje to, co Pico odpowiedzialo (np. 'Odebrano: ...')."""
    koniec = time.monotonic() + czas
    while time.monotonic() < koniec:
        surowe = lacze.readline()
        if surowe:
            print("   Pico:", surowe.decode(errors="replace").strip())


def odetnij(lacze, nogi):
    for noga in nogi:
        try:
            wyslij(lacze, f"Z {noga} 0")
            print(f"[zasilanie] noga {noga}: ODCIETE")
        except Exception as blad:
            print(f"[zasilanie] noga {noga}: NIE UDALO SIE ODCIAC ({blad})", file=sys.stderr)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("port")
    parser.add_argument("pliki", nargs="+")
    parser.add_argument("--nogi", type=int, nargs="+", default=[0, 1], choices=[0, 1])
    parser.add_argument("--pauza", type=float, default=0.05, help="przerwa miedzy ramkami [s]")
    arg = parser.parse_args()

    # bledy w plikach wychodza zanim zostanie wlaczone jakiekolwiek zasilanie
    pozy = [(sciezka, wczytaj_ramki(sciezka)) for sciezka in arg.pliki]

    lacze = serial.Serial(arg.port, 115200, timeout=0.05)
    try:
        time.sleep(1.5)    # po otwarciu portu Pico moze sie zrestartowac
        odczytaj(lacze, 0.3)

        # bezpieczny start: zasilanie odciete
        odetnij(lacze, [0, 1])
        odczytaj(lacze, 0.3)

        # zasilanie wlaczamy PRZED wyslaniem pozycji: Pico dochodzi do celu powoli
        for noga in arg.nogi:
            odp = input(f"Wlaczyc zasilanie nogi {noga}? [Enter = tak, q = koniec] ")
            if odp.strip().lower() == "q":
                return
            wyslij(lacze, f"Z {noga} 1")
            print(f"[zasilanie] noga {noga}: WLACZONE")
            odczytaj(lacze, 1.0)

        for sciezka, ramki in pozy:
            odp = input(f"Wyslac pozycje z '{sciezka}' ({len(ramki)} ramek)? [Enter = tak, q = koniec] ")
            if odp.strip().lower() == "q":
                break
            for ramka in ramki:
                wyslij(lacze, ramka)
                print("->", ramka)
                odczytaj(lacze, arg.pauza)
            print("   (Pico dochodzi do pozycji powoli, poczekaj)")
            odczytaj(lacze, 1.0)
    except KeyboardInterrupt:
        print("\nPrzerwano (Ctrl+C).")
    finally:
        odetnij(lacze, [0, 1])
        odczytaj(lacze, 0.5)
        lacze.close()


if __name__ == "__main__":
    main()