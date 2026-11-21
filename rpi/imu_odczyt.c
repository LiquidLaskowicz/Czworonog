// Biblioteki
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "imu.h"
#include "orient.h"
#include <time.h>

// Definicje parametrow
#define DOMYSLNA_SCIEZKA "/dev/i2c-1"
#define DOMYSLNY_ADRES 0x68

int main(int argc, char *argv[])
{
    const char *sciezka = DOMYSLNA_SCIEZKA;
    int adres = DOMYSLNY_ADRES;
    imu_dane_t dane;
    orient_t o;
    imu_dane_t offset;

    if (argc > 1) { sciezka = argv[1]; }
    if (argc > 2) { adres = (int)strtol(argv[2], NULL, 0); }

    int fd = otworz_imu(sciezka, adres);
    if (fd == -1) { fprintf(stderr, "Nie znaleziono urzadzenia\n"); return 1; }
    if (imu_init(fd) == -1) { fprintf(stderr, "Nieprawidlowy adres lub podlaczenie\n"); close(fd); return 1; }

    printf("Kalibracja zyroskopu, nie ruszac robota...\n");
    if (imu_kalibruj_zyro(fd, &offset, 200) == -1) {fprintf(stderr, "Kalibracja nieudana\n"); close(fd); return 1; }
    printf("Offset: gx %.3f gy %.3f gz %.3f\n", offset.gx, offset.gy, offset.gz);

    if (imu_czytaj_jeden(fd, &dane) == -1) { fprintf(stderr, "Blad odczytu danych z imu\n"); close(fd); return 1; }
    orient_init(&o, &dane);

    struct timespec poprz;
    clock_gettime(CLOCK_MONOTONIC, &poprz);

    while(1)
    {
        if (imu_czytaj_jeden(fd, &dane) == -1) { fprintf(stderr, "Blad odczytu danych z imu(1)\n"); break; }
        dane.gx -= offset.gx;
        dane.gy -= offset.gy;
        dane.gz -= offset.gz;
        struct timespec teraz;
        clock_gettime(CLOCK_MONOTONIC, &teraz);
        double dt = (teraz.tv_sec - poprz.tv_sec) + (teraz.tv_nsec - poprz.tv_nsec) / 1e9;
        orient_aktualizuj(&o, &dane, dt);
        printf("ax %.3f ay %.3f az %.3f | gx %.2f gy %.2f gz %.2f | roll %.2f | pitch %.2f\n", dane.ax, dane.ay, dane.az, dane.gx, dane.gy, dane.gz, o.roll, o.pitch);
        poprz = teraz;
        usleep(50000);
    }

    close(fd);
    return 0;
}