#ifndef SERWO_H
#define SERWO_H

// Biblioteki
#include <stdint.h>

// Definicje parametrow
#define SERWO_IMPULS_MIN_US 500
#define SERWO_IMPULS_MAX_US 2500
#define SERWO_IMPULS_MID_US 1500
#define SERWO_KATY_ZAKRES_DEG 180
#define SERWO_OKRES_US 20000

// Struktura od serwa
typedef struct
{
    uint8_t numer_pinu;
    int8_t kierunek;
    float kat_minimalny_deg;
    float kat_maksymalny_deg;
    float trym_zera_us; // korekta zera serwa

}serwo_t;

// Deklaracje funkcji
uint16_t kat_na_impuls(const serwo_t *s, float kat_deg);

#endif