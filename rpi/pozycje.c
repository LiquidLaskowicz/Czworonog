// Biblioteki
#include "kinematyka.h"
#include <stdio.h>
#include "protokol.h"
#include <string.h>
#include "chod.h"

// Definicje parametrow
#define KANAL_BARK 0
#define KANAL_UDO_F 1
#define KANAL_UDO_B 2

// Deklaracje funkcji
static void wypisz_noge(int noga, double przechyl, double podniesienie);
static void wypisz_kanaly(int noga, double bark, double udo_F, double udo_B);

//Funkcja liczy katy serw nogi i wypisuje je jako ramki
static void wypisz_noge(int noga, double przechyl, double podniesienie)
{
    double bark;
    double udo_F;
    double udo_B;

    int ko = kin_nogi_serwa(noga, przechyl, podniesienie, &bark, &udo_F, &udo_B, 0.0);
    if (ko == 0) { fprintf(stderr, "Blad wyznaczania kinematyki, zla noga lub blad zakresu\n"); return;}

    wypisz_kanaly(noga, bark, udo_F, udo_B);

}

// Funkcja wypisujaca ramki do kanalow
static void wypisz_kanaly(int noga, double bark, double udo_F, double udo_B)
{
    char bufor[32];
    int kk;
    int kanal_bark = 3 * (noga % 2) + KANAL_BARK;
    int kanal_udo_F = 3 * (noga % 2) + KANAL_UDO_F;
    int kanal_udo_B = 3 * (noga % 2) + KANAL_UDO_B;

    kk = buduj_ramke_kat(bufor, sizeof(bufor), kanal_bark, bark);
    if (kk < 0 || kk >= (int)sizeof(bufor)) {fprintf(stderr, "Nieprawidlowa liczba znakow przy konstrukcji ramki (bark)\n"); return;}
    fputs(bufor, stdout);
    kk = buduj_ramke_kat(bufor, sizeof(bufor), kanal_udo_F, udo_F);
    if (kk < 0 || kk >= (int)sizeof(bufor)) {fprintf(stderr, "Nieprawidlowa liczba znakow przy konstrukcji ramki (udo_F)\n"); return;}
    fputs(bufor, stdout);
    kk = buduj_ramke_kat(bufor, sizeof(bufor), kanal_udo_B, udo_B);
    if (kk < 0 || kk >= (int)sizeof(bufor)) {fprintf(stderr, "Nieprawidlowa liczba znakow przy konstrukcji ramki (udo_B)\n"); return;}
    fputs(bufor, stdout);
}

int main(int argc, char *argv[])
{
    if (argc > 5 || argc < 3) { fprintf(stderr, "Pozycje <pico 0|1> <poza>\n"); return 1;}

    int pico;
    int wczytane = sscanf(argv[1], "%d", &pico);
    if (wczytane != 1 || pico < 0 || pico > 1) {fprintf(stderr, "Nieprawidlowy numer pico\n"); return 1;}
    double przechyl;

    kinematyka_init();

    if (strcmp(argv[2], "stoj") == 0)
    {
        if (argc != 3) { fprintf(stderr, "Pozycje <pico 0|1> <poza>\n"); return 1;}
        for (int i = 0; i < 2; i++) { wypisz_noge(2*pico + i, 0.0, 0.0); }
    }
    else if (strcmp(argv[2], "podnies") == 0)
    {
        if (argc != 4) { fprintf(stderr, "Pozycje <pico 0|1> <poza>\n"); return 1;}
        int noga;
        wczytane = sscanf(argv[3], "%d", &noga);
        if (wczytane != 1 || noga < 0 || noga > 3) {fprintf(stderr, "Nieprawidlowy numer nogi\n"); return 1;}

        if (noga == 0 || noga == 2) { przechyl = PRZECHYL; }
        else { przechyl = -PRZECHYL; }

        for (int i = 0; i < 2; i++)
        {
            if (2*pico + i == noga) { wypisz_noge(2*pico + i, przechyl, PODNIESIENIE); }
            else { wypisz_noge(2*pico + i, przechyl, 0.0); }
        }

    }
    else if (strcmp(argv[2], "krok") == 0)
    {
        if (argc != 5) { fprintf(stderr, "Pozycje <pico 0|1> <poza> <krok> <faza>\n"); return 1;}
        int krok;
        wczytane = sscanf(argv[3], "%d", &krok);
        if (wczytane != 1 || krok < 0) {fprintf(stderr, "Nieprawidlowy krok\n"); return 1;}
        int faza;
        wczytane = sscanf(argv[4], "%d", &faza);
        if (wczytane != 1 || faza < 0 || faza > (LICZBA_FAZ-1)) {fprintf(stderr, "Nieprawidlowa faza\n"); return 1;}

        int noga;
        double bark;
        double udo_F;
        double udo_B;

        for (int i = 0; i < 2; i++)
        {
            noga = 2*pico + i;
            int chp;
            chp = chod_poza_nogi(noga, krok, faza, &bark, &udo_F, &udo_B);
            if (chp == 0) { fprintf(stderr, "Blad przy wyznaczaniu pozy kroku\n"); return 1; }
            wypisz_kanaly(noga, bark, udo_F, udo_B);
        }


    }
    else { fprintf(stderr, "Nieznana poza\n"); return 1; }

    return 0;

}