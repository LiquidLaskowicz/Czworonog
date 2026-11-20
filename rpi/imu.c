// Biblioteki
#include "imu.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Definicje parametrow
#define LSB_PER_G 16384.0
#define LSB_PER_DEG_PER_S 16.4

// Deklaracje funkcji
static int16_t z_bajtow(const uint8_t *p);

// Pomocnicza funkcja sklejajaca dane z dwoch bajtow
static int16_t z_bajtow(const uint8_t *p)
{
    uint16_t surowe = (uint16_t)(p[1] << 8 | p[0]);

    return (int16_t)surowe;
}

// Funkcja zamieniajaca surowe dane z imu na przyspieszenia
int bajty_na_dane_imu(const uint8_t *bufor, imu_dane_t *dane)
{
    if (bufor == NULL || dane == NULL) { return 0; }

    dane->gx = z_bajtow(bufor + 0) / LSB_PER_DEG_PER_S;
    dane->gy = z_bajtow(bufor + 2) / LSB_PER_DEG_PER_S;
    dane->gz = z_bajtow(bufor + 4) / LSB_PER_DEG_PER_S;

    dane->ax = z_bajtow(bufor + 6) / LSB_PER_G;
    dane->ay = z_bajtow(bufor + 8) / LSB_PER_G;
    dane->az = z_bajtow(bufor + 10) / LSB_PER_G;

    return 1;
}

// Funkcja sluzaca do otwarcia komunikacji z imu
int otworz_imu(const char *sciezka, int adres)
{
    int fd = open(sciezka, O_RDWR);
    if (fd < 0) { return -1; }

    if (ioctl(fd, I2C_SLAVE, adres) < 0) { close(fd); return -1; }

    return fd;
}

// Funkcja czytajaca dane z imu
int imu_czytaj_rejestry(int fd, uint8_t rejestr, uint8_t *bufor, size_t ile)
{
    if(write(fd, &rejestr, 1) != 1) { return -1; }

    if(read(fd, bufor, ile) != (ssize_t)ile) { return -1; }

    return 0;
}