// Biblioteki
#include <stdio.h>
#include "lacze_pico.h"
#include <unistd.h>
#include <inttypes.h>
#include <stdint.h>
#include "protokol.h"
#include <poll.h>
#include <stdlib.h>

// Definicje parametrow
#define PORT "/dev/ttyACM0"

// Deklaracje funkcji
static int obsluga_klawiatury (int fd);
static int obsluga_telemetrii (int fd, char *bufor, size_t rozmiar, size_t *dlugosc);

// Definicja funkcji do obslugi klawaitury
static int obsluga_klawiatury(int fd)
{
    char komenda[32];
    char linia[32];

    if(fgets(linia, sizeof(linia), stdin) == NULL)
    {
        return 1;
    }

    char *koniec;
    float kat = strtof(linia, &koniec);
    if (koniec == linia)
    {
        printf("Niepoprawna wartosc kata \n");
    }
    else
    {
        buduj_ramke_kat(komenda, sizeof(komenda), kat);
        if(wyslij_linie_lacze(fd, komenda) == -1)
        {
            fprintf(stderr, "Blad wysylania danych do pico\n");
            return -1;
        }
    }

    return 0;
}

// Definicja funkcji do obslugi telemetrii
static int obsluga_telemetrii(int fd, char *bufor, size_t rozmiar, size_t *dlugosc)
{
    uint32_t czas = 0;
    float zadany = 0;
    float zmierzony = 0;
    uint16_t surowy = 0;

    int read = czytaj_linie_lacze(fd, bufor, sizeof(bufor), &dlugosc);
    
    if (read == -1) {fprintf(stderr, "Blad odczytu danych z lacza z pico\n"); close(fd); return 1; }
    if (read == 1)
    {
        // printf("Linia: %s\n", bufor);
        if (parsuj_telemetrie(bufor, &czas, &zadany, &zmierzony, &surowy))
        {
            printf("t=%" PRIu32 " us, zadany=%.2f, zmierzony=%.2f, surowy=%u\n", czas, zadany, zmierzony, surowy);
        }
    }

    return 0;
}

int main(void)
{

    int fd = otworz_lacze(PORT);
    if (fd == -1) {fprintf(stderr, "Nie otwarto lacza komunikacji z pico\n"); return 1; }
    printf("Otwarto lacze komunikacji z pico\n");

    char bufor[128];
    size_t dlugosc = 0;

    struct pollfd fds[2];
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    fds[1].fd = STDIN_FILENO;
    fds[1].events = POLLIN;

    while(1)
    {
        int ile = poll(fds, 2, 100);
        if (ile < 0) {fprintf(stderr, "Blad poll\n"); close (fd); return 1; }
        if (fds[0].revents & ( POLLHUP | POLLERR))
        {
            fprintf(stderr, "Lacze z pico zamkniete\n");
            close(fd);
            return 1;
        }
        if (fds[0].revents & (POLLIN))
        {
            if (obsluga_telemetrii(fd, bufor, rozmiar, dlugosc) == -1) { return 1; }
        }
        if (fds[1].revents & (POLLIN | POLLHUP))
        {
            int obsklaw = obsluga_klawiatury(fd);
            if (obsklaw == 1) { close(fd); return 0; }
            if (obsklaw == -1) { close(fd); return 1; }
        }
    }

    close(fd);
    return 0;
}