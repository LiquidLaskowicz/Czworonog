// Biblioteki
#include "pico/stdlib.h"
#include "serwo.h"
#include "serwo_pwm.h"


// Main
int main()
{
    stdio_init_all();

    serwo_t serwo_test = 
    {
        .numer_pinu = 0,
        .kierunek = 1,
        .kat_minimalny_deg = -45,
        .kat_maksymalny_deg = 45,
        .trym_zera_us = 0
    };

    serwo_init(&serwo_test);

    while (1)
    {
        serwo_ustaw_kat(&serwo_test, -30);
        sleep_ms(2000);
        serwo_ustaw_kat(&serwo_test, 30);
        sleep_ms(2000);

    }

    return 0;
}