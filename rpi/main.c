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
static int obsluga_klawiatury (int fd, int *tryb);
static int obsluga_telemetrii (int fd, char *bufor, size_t rozmiar, size_t *dlugosc);

// Definicja funkcji do obslugi klawaitury
static int obsluga_klawiatury(int fd, int *tryb)
{
    char linia[32];
    char komenda[32];

    if(fgets(linia, sizeof(linia), stdin) == NULL)
    {
        return 1;
    }

    int pin;
    float wartosc;
    int n = sscanf(linia, "V%d %f", &pin, &wartosc);

    if (n != 2) {fprintf(stderr, "Niepoprawna komenda\n"); return 0;}
    else if (pin == 3) {*tryb = (int)wartosc; fprintf(stderr, "Zmiana trybu na: %d\n", *tryb); return 0;}
    else if (pin == 0 && *tryb != 2) {fprintf(stderr, "Ignoruje V0: to nie tryb testowy\n"); return 0;}
    else if (pin == 0)
    {
        if (wartosc > 30) {wartosc = 30;}
        else if (wartosc < -30) { wartosc = -30;}

        buduj_ramke_kat(komenda, sizeof(komenda), wartosc);
        if (wyslij_linie_lacze(fd, komenda) == -1) {fprintf(stderr, "Blad wysylania linii\n"); return -1;}
        fprintf(stderr, "pin=%d wartosc=%.2f\n", pin, wartosc);
    }
    else {fprintf(stderr, "Tymczasowy brak obslugi pinu\n");}
    return 0;
}

// Definicja funkcji do obslugi telemetrii
static int obsluga_telemetrii(int fd, char *bufor, size_t rozmiar, size_t *dlugosc)
{
    uint32_t czas = 0;
    float zadany = 0;
    float zmierzony = 0;
    uint16_t surowy = 0;

    int czyt = czytaj_linie_lacze(fd, bufor, rozmiar, dlugosc);
    
    if (czyt == -1) {fprintf(stderr, "Blad odczytu danych z lacza z pico\n"); return -1; }
    if (czyt == 1)
    {
        // printf("Linia: %s\n", bufor);
        if (parsuj_telemetrie(bufor, &czas, &zadany, &zmierzony, &surowy))
        {
            printf("%" PRIu32 ",%.2f,%.2f,%u\n", czas, zadany, zmierzony, surowy);
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    setvbuf(stdout, NULL, _IOLBF, 0);

    const char *port = PORT;

    if (argc > 1) {port = argv[1];}

    fprintf(stderr, "Port komunikacyjny: %s\n", port);
    int fd = otworz_lacze(port);
    if (fd == -1) {fprintf(stderr, "Nie otwarto lacza komunikacji z pico\n"); return 1; }
    fprintf(stderr, "Otwarto lacze komunikacji z pico\n");

    char bufor[128];
    size_t dlugosc = 0;

    struct pollfd fds[2];
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    fds[1].fd = STDIN_FILENO;
    fds[1].events = POLLIN;

    printf("czas_us,zadany,zmierzony_deg,surowy\n");

    int tryb = 0;

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
            if (obsluga_telemetrii(fd, bufor, sizeof(bufor), &dlugosc) == -1) { close(fd); return 1; }
        }
        if (fds[1].revents & (POLLIN | POLLHUP))
        {
            int obsklaw = obsluga_klawiatury(fd, &tryb);
            if (obsklaw == 1) { close(fd); return 0; }
            if (obsklaw == -1) { close(fd); return 1; }
        }
    }

    close(fd);
    return 0;
}