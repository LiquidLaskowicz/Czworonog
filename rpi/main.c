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
#define BARK 0
#define RAMIE_R 1
#define RAMIE_L 2

static const float ZAKRES[3][2] =
{
    {-30, 30},
    {10, 170},
    {10, 170}
};

// Deklaracje funkcji
static int obsluga_klawiatury(int fd, int *tryb);
static int obsluga_telemetrii(int fd, char *bufor, size_t rozmiar, size_t *dlugosc);
static int pin_na_kanal(int pin);
static void wylacz_zasilanie(int fd);

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
    if (pin == 3)
        {
            int stary_tryb = *tryb;
            int nowy_tryb = (int)wartosc;
            if (nowy_tryb < 0 || nowy_tryb > 2) {fprintf(stderr, "Nieprawidlowy numer trybu %d\n", nowy_tryb); return 0;}
            *tryb = nowy_tryb; fprintf(stderr, "Zmiana trybu na: %d\n", *tryb);
            if (stary_tryb == 2 && nowy_tryb != 2)
            {
                buduj_ramke_zasilanie(komenda, sizeof(komenda), 0, 0);
                if (wyslij_linie_lacze(fd, komenda) == -1) {fprintf(stderr, "Blad wysylania linii\n"); return -1;}
                fprintf(stderr, "Wyjscie z trybu 2, zasilanie wylaczone\n");
            }

            return 0;
        }
    if (pin == 9)
    {
        int stan = 0;
        if (wartosc != 0) { stan = 1; }

        if (*tryb != 2 && wartosc != 0) {fprintf(stderr, "Wlaczenie zasilania mozliwe tylko w trybie 2!\n");  return 0; }
        
        buduj_ramke_zasilanie(komenda, sizeof(komenda), 0, stan);
        if (wyslij_linie_lacze(fd, komenda) == -1) {fprintf(stderr, "Blad wysylania linii\n"); return -1;}
        fprintf(stderr, "noga=%d stan=%d\n", 0, stan);

        return 0;

    }

    int kanal = pin_na_kanal(pin);

    if (kanal < 0) {fprintf(stderr, "Brak obslugi pinu V%d\n", pin); return 0;}
    if (*tryb != 2) {fprintf(stderr, "To nie tryb testowy, ignoruje pin V%d\n", pin); return 0;}

    if (wartosc > ZAKRES[kanal][1]) {wartosc = ZAKRES[kanal][1];}
    else if (wartosc < ZAKRES[kanal][0]) { wartosc = ZAKRES[kanal][0];}

    buduj_ramke_kat(komenda, sizeof(komenda), kanal, wartosc);
    if (wyslij_linie_lacze(fd, komenda) == -1) {fprintf(stderr, "Blad wysylania linii\n"); return -1;}
    fprintf(stderr, "pin=%d wartosc=%.2f\n", pin, wartosc);

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

// Definicja funkcji ktora zwraca odpowiedni kanal dla pinow 0 1 2 i -1 dla pozostalych pinow
static int pin_na_kanal(int pin)
{
    if (pin == 0) { return 0; }
    else if (pin == 1) { return 1; }
    else if (pin == 2) { return 2; }
    else { return -1; }
}

static void wylacz_zasilanie(int fd)
{
    char komenda[32];

    buduj_ramke_zasilanie(komenda, sizeof(komenda), 0, 0);
    wyslij_linie_lacze(fd, komenda);
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
        if (ile < 0) {fprintf(stderr, "Blad poll\n"); wylacz_zasilanie(fd); close (fd); return 1; }
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
            if (obsklaw == 1) { wylacz_zasilanie(fd); close(fd); return 0; }
            if (obsklaw == -1) { wylacz_zasilanie(fd); close(fd); return 1; }
        }
    }

    close(fd);
    return 0;
}