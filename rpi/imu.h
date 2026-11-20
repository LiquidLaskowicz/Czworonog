#ifndef IMU_H
#define IMU_H

// Biblioteki
#include <stdint.h>
#include <stddef.h>

typedef struct
{
    double ax;
    double ay;
    double az;
    double gx;
    double gy;
    double gz;
} imu_dane_t;

// Deklaracje funkcji
int bajty_na_dane_imu(const uint8_t *bufor, imu_dane_t *dane);
int otworz_imu(const char *sciezka, int adres);
int imu_czytaj_rejestry(int fd, uint8_t rejestr, uint8_t *bufor, size_t ile);
int imu_zapisz_rejestr(int fd, uint8_t rejestr, uint8_t wartosc);
int imu_init(int fd);
int imu_czytaj_jeden(int fd, imu_dane_t *dane);

#endif