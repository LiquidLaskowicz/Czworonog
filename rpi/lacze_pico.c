// Biblioteki

#include "lacze_pico.h"
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

// Funkcja otwierania lacza komunikacji z pico
int otworz_lacze(const char *path)
{
    int fd = open(path, O_RDWR | O_NOCTTY);
    int (fd < 0) { return -1; }

    struct termios tio;
    if (tcgetattr(fd, &tio) !=0) { close(fd); return -1; }

    // TODO: makeraw, timeout odczytu, setattr

    return fd;
}