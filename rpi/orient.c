// Biblioteki
#include "orient.h"
#include <math.h>
#include <unistd.h>

// Definicje parametrow
#define PI 3.14159265359
#define ALFA 0.98
#define RAD_NA_DEG (180.0 / PI)

// Deklaracje funkcji
static void katy_z_akcelerometru(const imu_dane_t *dane, double *roll, double *pitch);

// Funkcja liczaca roll i pitch tylko z kierunku grawitacji
static void katy_z_akcelerometru(const imu_dane_t *dane, double *roll, double *pitch)
{
    *roll = atan2(dane->ay, dane->az) * RAD_NA_DEG; 
    *pitch = atan2(-dane->ax, sqrt(dane->ay * dane->ay + dane->az * dane->az)) * RAD_NA_DEG;
}

// Funkcja ustawiajaca katy startowe z akcelerometru
void orient_init(orient_t *o, const imu_dane_t *dane)
{  
    katy_z_akcelerometru(dane, &o->roll, &o->pitch);
}

// Funkcja wyznaczajaca wartosci katowe pitch i roll na podstawie danych z imu, filtr komplementarny
void orient_aktualizuj(orient_t *o, const imu_dane_t *dane, double dt)
{
    double obr;
    double pochyl;
    katy_z_akcelerometru(dane, &obr, &pochyl);

    o->roll = ALFA * (o->roll + dane->gx * dt) + (1.0 - ALFA) * obr;
    o->pitch = ALFA * (o->pitch + dane->gy * dt) + (1.0 - ALFA) * pochyl;
}

// Funkcja kalibrujaca zyroskop
int imu_kalibruj_zyro(int fd, imu_dane_t *offset, int liczba_probek)
{
    if (liczba_probek <= 0) { return -1; }

    double suma_x = 0.0;
    double suma_y = 0.0;
    double suma_z = 0.0;
    imu_dane_t dane;

    for (int i = 0; i < liczba_probek; i++)
    {
        if (imu_czytaj_jeden(fd, &dane) == -1) { return -1; }
        suma_x += dane.gx;
        suma_y += dane.gy;
        suma_z += dane.gz;

        usleep(5000);
    }

    offset->gx = suma_x / liczba_probek;
    offset->gy = suma_y / liczba_probek;
    offset->gz = suma_z / liczba_probek;

    offset->ax = 0.0;
    offset->ay = 0.0;
    offset->az = 0.0;

    return 0;
}