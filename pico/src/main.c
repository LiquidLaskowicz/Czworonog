// Biblioteki
#include <stdio.h>
#include "pico/stdlib.h"
#include "serwo.h"
#include "serwo_pwm.h"
#include "enkoder.h"

// Stale
#define ROZMIAR_BUFORA 32
#define OKRES_ODCZYTU_US 100000

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
    enkoder_init();

    char bufor[ROZMIAR_BUFORA];
    uint8_t pojemnosc_bufora = 0;
    bool bufor_overflow_flag = false;
    uint32_t czas_ostatniego_odczytu = 0;

    while (1)
    {
        // serwo_ustaw_kat(&serwo_test, -30);
        // sleep_ms(2000);
        // serwo_ustaw_kat(&serwo_test, 30);
        // sleep_ms(2000);

        int temp_znak = getchar_timeout_us(0);

        if (temp_znak != PICO_ERROR_TIMEOUT && bufor_overflow_flag != true)
        {
            // printf("Odebrano: %c\n", temp_znak);
            if (pojemnosc_bufora == ROZMIAR_BUFORA - 1 && temp_znak != '\n')
            {
                printf("Bufor przepelniony!, kasuje bufor\n");
                pojemnosc_bufora = 0;
                bufor_overflow_flag = true;

            }
            else
            {
                if (temp_znak != '\n')
                {
                    bufor[pojemnosc_bufora] = temp_znak;
                    pojemnosc_bufora++;
                }
                else
                {
                    if (pojemnosc_bufora > 0 && bufor[pojemnosc_bufora - 1] == '\r')
                    {
                        pojemnosc_bufora--;
                    }
                    bufor[pojemnosc_bufora] = '\0';
                    printf("Odebrano: %s\n", bufor);
                    // printf("Warotsc_bufora: %d\n", pojemnosc_bufora);
                    pojemnosc_bufora = 0;
                }

            }
        }
        
        if (temp_znak == '\n') {bufor_overflow_flag = false;}

        // printf("Pico dziala!\n");
        // sleep_ms(1000);

        uint32_t teraz = time_us_32();

        if (teraz - czas_ostatniego_odczytu >= OKRES_ODCZYTU_US)
        {
            printf("%lu,%u\n",teraz, czytaj_enkoder_raw());
            czas_ostatniego_odczytu = teraz;
        }

    }

    return 0;
}