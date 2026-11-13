// Biblioteki
#include "protokol.h"
#include <stdio.h>
#include <inttypes.h>

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