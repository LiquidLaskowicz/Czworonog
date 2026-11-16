#ifndef CHOD_H
#define CHOD_H

// Definicje parametrow
#define LICZBA_NOG 4
#define LICZBA_FAZ 5
#define DLUGOSC_KROKU 0.04

typedef struct // to sa ulamki wartosci z define
{
    double przechyl;
    double uniesienie;
    double przod;
} faza_t;

// Deklaracje funkcji
int chod_noga_przenoszona(int krok);
double chod_przechyl(int noga);
const faza_t *chod_faza(int nr);

#endif