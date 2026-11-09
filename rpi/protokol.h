#ifndef PROTOKOL_H
#define PROTOKOL_H

// Biblioteki
#include <stddef.h>
#include <stdint.h>

// Deklaracje funkcji
int buduj_ramke_kat(char *bufor, size_t rozmiar, float kat);
int parsuj_telemetrie(const char *linia, uint32_t *czas_us, float *zadany, float *zmierzony, uint16_t *surowy);

#endif