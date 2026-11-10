#ifndef LACZE_PICO_H
#define LACZE_PICO_H

// Biblioteki
#include <stddef.h>

// Deklaracje funkcji
int otworz_lacze(const char *path);
int czytaj_linie_lacze(int fd, char *bufor, size_t rozmiar, size_t dlugosc);
int wyslij_linie_lacze(int fd, const char *tekst);

#endif