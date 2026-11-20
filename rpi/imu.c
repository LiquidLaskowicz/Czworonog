// Biblioteki
#include "imu.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Definicje parametrow
#define LSB_PER_G 16384.0
#define LSB_PER_DEG_PER_S 16.4
#define REJ_CHIP_ID 0x00
#define REJ_CMD 0x7E
#define CHIP_ID_BMI160 0xD1
#define CMD_RESET 0xB6
#define CMD_ACC_NORMAL 0x11
#define CMD_GYR_NORMAL 0x15
#define REJ_DANE 0x0C

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

// Funkcja zapisujaca zadana wartosc do rejestru imu
int imu_zapisz_rejestr(int fd, uint8_t rejestr, uint8_t wartosc)
{
    uint8_t ramka[2] = {rejestr, wartosc};
    if (write(fd, ramka, 2) != 2) { return -1; }

    return 0;
}

// Funkcja inicjalizujaca imu
int imu_init(int fd)
{
    uint8_t id = 0;

    if (imu_czytaj_rejestry(fd, REJ_CHIP_ID, &id, 1) == -1 || id != CHIP_ID_BMI160) { return -1; }
    if (imu_zapisz_rejestr(fd, REJ_CMD, CMD_RESET) == -1) { return -1; }
    usleep(100000);
    if (imu_zapisz_rejestr(fd, REJ_CMD, CMD_ACC_NORMAL) == -1) { return -1; }
    usleep(100000);
    if (imu_zapisz_rejestr(fd, REJ_CMD, CMD_GYR_NORMAL) == -1) { return -1; }
    usleep(100000);

    return 0;
}

// Funkcja czytajaca jeden rejestr imu
int imu_czytaj_jeden(int fd, imu_dane_t *dane)
{
    uint8_t bufor[12];
    
    if (imu_czytaj_rejestry(fd, REJ_DANE, bufor, sizeof(bufor)) == -1) { return -1; }

    if (bajty_na_dane_imu(bufor, dane) == 0) { return -1; }

    return 0;
}