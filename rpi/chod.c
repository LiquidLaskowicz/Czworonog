// Biblioteki
#include "chod.h"
#include "kinematyka.h"
#include <stddef.h>

static const int KOLEJNOSC_NOG[LICZBA_NOG]=
{
    2,
    0,
    3,
    1
};

static const faza_t FAZY[LICZBA_FAZ]=
{
    {
        .przechyl = 1,
        .uniesienie = 0,
        .przod = 0
    },
    
    {
        .przechyl = 1,
        .uniesienie = 1,
        .przod = 0
    },

    {
        .przechyl = 1,
        .uniesienie = 1,
        .przod = 1
    },

    {
        .przechyl = 1,
        .uniesienie = 0,
        .przod = 1
    },

    {
        .przechyl = 0,
        .uniesienie = 0,
        .przod = 1
    }
};

// Funkcja zwracajaca numer przenoszonej nogi
int chod_noga_przenoszona(int krok)
{
    if (krok < 0) { return -1; }
    else { return KOLEJNOSC_NOG[krok % LICZBA_NOG]; }
}

// Funkcja zwracajaca przechyl o odpowiednim dla danej nogi znaku
double chod_przechyl(int noga)
{
    if (noga == 0 || noga == 2) { return PRZECHYL; }
    else if (noga == 1 || noga == 3) { return -PRZECHYL; }
    else { return 0.0; }

}

const faza_t *chod_faza(int nr)
{
    if (nr < 0 || nr >= LICZBA_FAZ) { return NULL; }
    else { return &FAZY[nr]; }
}

int chod_poza_nogi(int noga, int krok, int faza, double *bark_deg, double *udo_F_deg, double *udo_B_deg)
{
    int przen = chod_noga_przenoszona(krok);
    if (przen == -1) { return 0; }
    
    const faza_t *f;
    f = chod_faza(faza);
    if (f == NULL) { return 0; }
    
    double przech = f->przechyl * chod_przechyl(przen);
    double uniesienie = 0.0;
    double przod = 0.0;

    if (noga == przen) { uniesienie = f->uniesienie * PODNIESIENIE; przod = f->przod * DLUGOSC_KROKU; }

    return kin_nogi_serwa(noga, przech, uniesienie, bark_deg, udo_F_deg, udo_B_deg, przod);
}