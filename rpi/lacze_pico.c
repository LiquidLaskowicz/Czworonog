// Biblioteki

#include "lacze_pico.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

// Funkcja otwierania lacza komunikacji z pico
int otworz_lacze(const char *path)
{
    int fd = open(path, O_RDWR | O_NOCTTY);
    if (fd < 0) { return -1; }

    struct termios tio;
    if (tcgetattr(fd, &tio) !=0) { close(fd); return -1; }

    cfmakeraw(&tio);
    tio.c_cc[VMIN] = 0;
    tio.c_cc[VTIME] = 1;
    if(tcsetattr(fd, TCSANOW, &tio) != 0) { close(fd); return -1; }

    return fd;
}

// Funkcja czytajaca dane z pico
int czytaj_linie_lacze(int fd, char *bufor, size_t rozmiar, size_t *dlugosc)
{
    char znak;

    while(1)
    {
        ssize_t n = read(fd, &znak, 1);
        if (n < 0) { return -1; }
        if (n == 0) { return 0; }

        if (znak == '\n')
        {
            if (*dlugosc > 0 && bufor[*dlugosc - 1] == '\r')
            {
                (*dlugosc)--;
            }

            bufor[*dlugosc] = '\0';
            *dlugosc = 0;
            return 1;
        }
        else if (*dlugosc < rozmiar - 1)
        {
            bufor[*dlugosc] = znak;
            (*dlugosc)++;
        }

    }
}

// Funkcja wysylajaca dane do pico
int wyslij_linie_lacze(int fd, const char *tekst)
{
    size_t dlugosc = strlen(tekst);
    size_t wyslano = 0;

    while(wyslano < dlugosc)
    {
        ssize_t n = write(fd, tekst + wyslano, dlugosc - wyslano);
        if (n < 0) { return -1;}
        wyslano += n;
    }

    return 0;

}