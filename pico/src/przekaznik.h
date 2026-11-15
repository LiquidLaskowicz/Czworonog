#ifndef PRZEKAZNIK_H
#define PRZEKAZNIK_H

// Biblioteki
#include <stdint.h>
#include <stdbool.h>

// Definicje parametrow
#define PRZEKAZNIK_NOGA_A_PIN 6
#define PRZEKAZNIK_NOGA_B_PIN 8
#define PRZEKAZNIK_STAN_ODCIECIA 1 // wysoki - cewka NC rozlacza prad

// Deklaracje funkcji
void przekaznik_init(uint8_t pin);
void przekaznik_zasilanie(uint8_t pin, bool wlaczone); // true - noga ma zasilanie

#endif