// Biblioteki
#include "protokol.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>

// Funkcja formujaca ramke do przekaznywania zadanego kata
int buduj_ramke_kat(char *bufor, size_t rozmiar, int kanal, float kat)
{
    return snprintf(bufor, rozmiar, "S %d %.2f\n", kanal, kat);
}

// Funkcja parsujaca dane z pico
int parsuj_telemetrie (const char *linia, uint32_t *czas_us, float *zadany, float *zmierzony, uint16_t *surowy)
{
    int wczytane = sscanf(linia, "%" SCNu32 ",%f,%f,%" SCNu16, czas_us, zadany, zmierzony, surowy);
    return wczytane == 4;
}

// Funkcja odczytuje z ramki kanal i wartosc kata
int parsuj_ramke_kat(const char *linia, int *kanal, float *kat)
{
    if (linia[0] != 'S' || linia[1] != ' ') { return 0; }

    char *koniec_kanalu;
    long k = strtol(linia + 2, &koniec_kanalu, 10);

    if (k < 0 || k > 255) { return 0; }
    if (koniec_kanalu == linia + 2 || *koniec_kanalu != ' ') { return 0; }

    char *koniec_kata;
    float a = strtof(koniec_kanalu, &koniec_kata);

    if (!isfinite(a)) { return 0; }
    if (koniec_kanalu == koniec_kata) { return 0; }

    *kanal = (int)k;
    *kat = a;

    return 1;
}

// Funkcja parsujaca ramke sygnalu sterowania nogi
int parsuj_ramke_zasilanie(const char *linia, int *noga, int *stan)
{
    int n;
    int s;
    int pozycja;

    int wczytane = sscanf(linia, "Z %d %d%n", &n, &s, &pozycja);

    if (wczytane != 2) { return 0; }
    while (linia[pozycja] == ' ' || linia[pozycja] == '\n' || linia[pozycja] == '\r') { pozycja++; }
    if(linia[pozycja] != '\0') { return 0; }

    if (n < 0 || n > 1 || s < 0 || s > 1) { return 0; }

    *noga = n;
    *stan = s;

    return 1;
}

// Funkcja budujaca ramke danych do sterowania przekaznikiem zasilania
int buduj_ramke_zasilanie(char *bufor, size_t rozmiar, int noga, int stan)
{
    return snprintf(bufor, rozmiar, "Z %d %d\n", noga, stan);
}