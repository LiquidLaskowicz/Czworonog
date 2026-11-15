// Biblioteki
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "serwo.h"
#include "serwo_pwm.h"
#include "enkoder.h"
#include "protokol.h"
#include "przekaznik.h"

// Stale
#define ROZMIAR_BUFORA 32
#define OKRES_ODCZYTU_US 100000 // Do testu odpowiedzi skokowej ustawic 5000
#define LICZBA_SERW 3

// Main
int main()
{
    przekaznik_init(PRZEKAZNIK_NOGA_A_PIN);
    przekaznik_init(PRZEKAZNIK_NOGA_B_PIN);
    stdio_init_all();

    serwo_t serwa [LICZBA_SERW] = 
    {
        {   // Kanal 0: bark
            .numer_pinu = 0,
            .kierunek = 1,
            .kat_minimalny_deg = -45,
            .kat_maksymalny_deg = 45,
            .trym_zera_us = 0
        },

        {   // Kanal 1: ramie_R
            .numer_pinu = 1,
            .kierunek = 1,
            .kat_minimalny_deg = -45,
            .kat_maksymalny_deg = 45,
            .trym_zera_us = 0,
            .offset_deg = 90
        },

        {   // Kanal 2: ramie_L
            .numer_pinu = 2,
            .kierunek = 1,
            .kat_minimalny_deg = -45,
            .kat_maksymalny_deg = 45,
            .trym_zera_us = 0,
            .offset_deg = 90
        }
    };

    for(int i = 0; i < LICZBA_SERW; i++)
    {
        serwo_init(&serwa[i]);
        serwo_ustaw_kat(&serwa[i], serwa[i].offset_deg);
    }

    enkoder_init();

    char bufor[ROZMIAR_BUFORA];
    uint8_t pojemnosc_bufora = 0;
    bool bufor_overflow_flag = false;
    uint32_t czas_ostatniego_odczytu = 0;
    float zadany_kat = 0.0f;

    while (1)
    {
        int temp_znak = getchar_timeout_us(0);
        int kanal;
        float kat;

        int noga;
        int stan;

        if (temp_znak != PICO_ERROR_TIMEOUT && bufor_overflow_flag != true)
        {
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

                    if (parsuj_ramke_kat(bufor, &kanal, &kat) && kanal < LICZBA_SERW)
                    {
                        serwo_ustaw_kat(&serwa[kanal], kat);

                        if (kanal == 0) {zadany_kat = kat;}
                    }
                    if (parsuj_ramke_zasilanie(bufor, &noga, &stan))
                    {
                        if (noga == 0) { przekaznik_zasilanie(PRZEKAZNIK_NOGA_A_PIN, stan == 1); }
                        if (noga == 1) { przekaznik_zasilanie(PRZEKAZNIK_NOGA_B_PIN, stan == 1); }

                    }

                    pojemnosc_bufora = 0;
                }

            }
        }
        
        if (temp_znak == '\n') {bufor_overflow_flag = false;}

        uint32_t teraz = time_us_32();

        if (teraz - czas_ostatniego_odczytu >= OKRES_ODCZYTU_US)
        {
            uint16_t dane_z_enkodera = czytaj_enkoder_raw();
            printf("%lu,%.2f,%.2f,%u\n", teraz, zadany_kat, aktualny_kat_serwa_deg(ENKODER_POLOZENIE_ZEROWE, dane_z_enkodera), dane_z_enkodera);
            czas_ostatniego_odczytu = teraz;
        }

    }

    return 0;
}