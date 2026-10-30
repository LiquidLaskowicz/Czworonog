// Biblioteki
#include "serwo.h"
#include <math.h>

// Zamiana pozadanego kata na impuls dla sterownika serwa
uint16_t kat_na_impuls(const serwo_t *s, float kat_deg)
{

    if (kat_deg < s->kat_minimalny_deg) {kat_deg = s->kat_minimalny_deg;}
    else if (kat_deg > s->kat_maksymalny_deg) {kat_deg = s->kat_maksymalny_deg;}

    float us_na_stopien = (SERWO_IMPULS_MAX_US - SERWO_IMPULS_MIN_US) / (float)SERWO_KATY_ZAKRES_DEG;
    float impuls_us = SERWO_IMPULS_MID_US + kat_deg * us_na_stopien * s->kierunek + s->trym_zera_us;

    if (impuls_us < SERWO_IMPULS_MIN_US) {impuls_us = SERWO_IMPULS_MIN_US;}
    else if (impuls_us > SERWO_IMPULS_MAX_US) {impuls_us = SERWO_IMPULS_MAX_US;}

    return (uint16_t)roundf(impuls_us);
}