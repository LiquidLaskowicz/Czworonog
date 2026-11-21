#ifndef ORIENT_H
#define ORIENT_H

// Biblioteki
#include "imu.h"

typedef struct
{
    double roll;
    double pitch;
} orient_t; // stopnie

// Deklaracje funkcji
void orient_init(orient_t *o, const imu_dane_t *dane);
void orient_aktualizuj(orient_t *o, const imu_dane_t *dane, double dt);
int imu_kalibruj_zyro(int fd, imu_dane_t *offset, int liczba_probek);

#endif