// Biblioteki
#include "chod.h"
#include "kinematyka.h"

static const int KOLEJNOSC_NOG[LICZBA_NOG]=
{
    2,
    0,
    3,
    1
};

// Funkcja zwracajaca numer przenoszonej nogi
int chod_noga_przenoszona(int krok)
{
    if (krok < 0) { return -1; }
    else { return KOLEJNOSC_NOG[krok % LICZBA_NOG]; }
}

double chod_przechyl(int noga)
{
    if (noga == 0 || noga == 2) { return PRZECHYL; }
    else if (noga == 1 || noga == 3) { return -PRZECHYL; }
    else { return 0.0; }

}