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

    char *koniec;
    long k = strtol(linia + 2, &koniec, 10);

    if (k < 0 || k > 255) { return 0; }
    if (koniec == linia + 2 || *koniec != ' ') { return 0; }

    char *koniec2;
    float a = strtof(koniec, &koniec2);

    if (!isfinite(a)) { return 0; }
    if (koniec == koniec2) { return 0; }

    *kanal = (int)k;
    *kat = a;

    return 1;
}