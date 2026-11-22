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

static const int KIERUNEK_PRZOD[LICZBA_NOG]=
{
    1,
    1,
    -1,
    -1
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

static const int STRONA_NOGI[LICZBA_NOG] =
{
     1,
    -1,
     1,
    -1
};

static double wspolczynnik_kierunku(int noga, kierunek_t kierunek)
{
    int k = KIERUNEK_PRZOD[noga];

    if(kierunek == KIER_PRAWO) {return k * STRONA_NOGI[noga]; }
    if(kierunek == KIER_LEWO) {return -k * STRONA_NOGI[noga]; }

    return k;
}

static double chod_x_stopy(int noga, int krok, int faza)
{
    double x = 0.0;

    for (int i = 0; i < krok; i++)
    {
        if (KOLEJNOSC_NOG[i % LICZBA_NOG] == noga) { x = DLUGOSC_KROKU / 2; }
        x -= DLUGOSC_KROKU / 4;
    }

    if (KOLEJNOSC_NOG[krok % LICZBA_NOG] == noga && faza >= 2) { x = DLUGOSC_KROKU / 2; }
    if (faza >= 4) { x -= DLUGOSC_KROKU / 4; }

    return x;
}

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

int chod_poza_nogi_kierunek(int noga, int krok, int faza, kierunek_t kierunek, double *bark_deg, double *udo_F_deg, double *udo_B_deg)
{
    if (noga < 0 || noga >= LICZBA_NOG) { return 0; }

    int przen = chod_noga_przenoszona(krok);
    if (przen == -1) { return 0; }

    const faza_t *f = chod_faza(faza);
    if (f == NULL) {return 0; }

    double przech = f->przechyl * chod_przechyl(przen);
    double uniesienie = 0.0;
    if (noga == przen) { uniesienie = f->uniesienie * PODNIESIENIE; }

    double przod = chod_x_stopy(noga, krok, faza) * wspolczynnik_kierunku(noga, kierunek);

    return kin_nogi_serwa(noga, przech, uniesienie, bark_deg, udo_F_deg, udo_B_deg, przod);
}

int chod_poza_nogi(int noga, int krok, int faza, double *bark_deg, double *udo_F_deg, double *udo_B_deg)
{
    return chod_poza_nogi_kierunek(noga, krok, faza, KIER_PRZOD, bark_deg, udo_F_deg, udo_B_deg);
}