#ifndef PROTOKOL_H
#define PROTOKOL_H

// Biblioteki
#include <stddef.h>
#include <stdint.h>

// Deklaracje funkcji
int buduj_ramke_kat(char *bufor, size_t rozmiar, int kanal, float kat);
int parsuj_telemetrie(const char *linia, uint32_t *czas_us, float *zadany, float *zmierzony, uint16_t *surowy);
int parsuj_ramke_kat(const char *linia, int *kanal, float *kat);
int parsuj_ramke_zasilanie(const char *linia, int *noga, int *stan);
int buduj_ramke_zasilanie(char *bufor, size_t rozmiar, int noga, int stan);

#endif