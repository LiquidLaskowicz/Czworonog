#ifndef SERWO_PWM_H
#define SERWO_PWM_H

// Biblioteki
#include "serwo.h"

// Deklaracje funkcji
void serwo_init (const serwo_t *s);
void serwo_ustaw_kat (const serwo_t *s, float kat_deg);

#endif