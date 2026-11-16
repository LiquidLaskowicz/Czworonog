// Biblioteki
#include "kinematyka.h"
#include <math.h>

// Zmienne
static double kat_dom_F;
static double kat_dom_B;
static double kat_dom_lydka;
static const double PRZESUNIECIE_PLASZCZYZNY[4] = 
{
    -0.0397,
    -0.0397,
     0.0397,
     0.0397
};
static const int ZNAK[4] =
{
     1,
    -1,
    -1,
     1
};

// Deklaracje funkcji
static double zawin_kat(double a);
static int punkt_kolana(double hu, double ju, double jw, double na_zewnatrz, double *ku, double *kw);
void kinematyka_init(void);
static int kin_pozycja_nogi(int noga, double przechyl, double podniesienie, double *bark_rad, double *fh);
static double rad_na_stopnie(double rad);

// Funkcja zawijajaca kat do danego zakresu <-PI; PI>
static double zawin_kat(double a)
{
    while (a > PI) { a -= 2*PI; }
    while (a < -PI) { a += 2*PI; }

    return a;
}

// Funkcja wyznaczajaca punkt na przecieciu okregow zbudowanych na udzie i lydce
static int punkt_kolana(double hu, double ju, double jw, double na_zewnatrz, double *ku, double *kw)
{
    double du = ju - hu;
    double dw = jw;
    double d = sqrt(du*du + dw*dw);

    if (d > DLUGOSC_UDO + DLUGOSC_LYDKA - 1e-4 || d < fabs(DLUGOSC_LYDKA - DLUGOSC_UDO)+ 1e-4) { return 0; }

    double l = (DLUGOSC_UDO * DLUGOSC_UDO - DLUGOSC_LYDKA * DLUGOSC_LYDKA + d * d) / (2*d);
    double h = sqrt(fmax(0, DLUGOSC_UDO * DLUGOSC_UDO - l * l));
    double eu = du / d;
    double ew = dw / d;

    double P_u = hu + l * eu;
    double P_w = l * ew;

    double c1u = P_u - h * ew;
    double c1w = P_w + h * eu;
    double c2u = P_u + h * ew;
    double c2w = P_w - h * eu;

    if ((c1u - c2u) * na_zewnatrz > 0) {*ku = c1u; *kw = c1w; }
    else {*ku = c2u; *kw = c2w; }

    return 1;
}

// Funkcja inizjalizujaca polozenie konczyn
void kinematyka_init(void)
{
    double huf = POLOWA_ROZSTAWU_SERW_UDO;
    double zluf = ZLACZE_LYDKI_U;
    double zlwf = ZLACZE_LYDKI_W;
    double na_zewnatrzf = 1.0;
    double kFu;
    double kFw;

    punkt_kolana(huf, zluf, zlwf, na_zewnatrzf, &kFu, &kFw);

    double hub = -POLOWA_ROZSTAWU_SERW_UDO;
    double zlub = ZLACZE_LYDKI_U;
    double zlwb = ZLACZE_LYDKI_W;
    double na_zewnatrzb = -1.0;
    double kBu;
    double kBw;

    punkt_kolana(hub, zlub, zlwb, na_zewnatrzb, &kBu, &kBw);


    kat_dom_F = atan2(kFw, kFu - POLOWA_ROZSTAWU_SERW_UDO);
    kat_dom_B = atan2(kBw, kBu + POLOWA_ROZSTAWU_SERW_UDO);
    kat_dom_lydka = atan2(ZLACZE_LYDKI_W - kFw, ZLACZE_LYDKI_U - kFu);
}

// Funkcja liczaca kinematyke odwrotna
int kin_odw_noga(double fu, double fw, double *thF, double *thB)
{
    double Ju = fu - STOPA_PRZES_U;
    double Jw = fw - STOPA_PRZES_W;
    double kFu;
    double kFw;
    double kBu;
    double kBw;

    int pk;

    for (int it = 0; it < 3; it++)
    {
        pk = punkt_kolana(POLOWA_ROZSTAWU_SERW_UDO, Ju, Jw, 1.0, &kFu, &kFw);
        if (!pk) { return 0; }

        double d = atan2(Jw - kFw, Ju - kFu) - kat_dom_lydka;

        Ju = fu - (STOPA_PRZES_U * cos(d) - STOPA_PRZES_W * sin(d));
        Jw = fw - (STOPA_PRZES_U * sin(d) + STOPA_PRZES_W * cos(d));
    }

    pk = punkt_kolana(POLOWA_ROZSTAWU_SERW_UDO, Ju, Jw, 1, &kFu, &kFw);
    if (!pk) { return 0; }
    pk = punkt_kolana(-POLOWA_ROZSTAWU_SERW_UDO, Ju, Jw, -1, &kBu, &kBw);
    if (!pk) { return 0; }

    *thF = zawin_kat(atan2(kFw, kFu - POLOWA_ROZSTAWU_SERW_UDO) - kat_dom_F);
    *thB = zawin_kat(atan2(kBw, kBu + POLOWA_ROZSTAWU_SERW_UDO) - kat_dom_B);

    return 1;
}

// Funkcja liczy kat barku i wysokosc stopy dla zadanego przechylu ciala i podniesienia nogi
static int kin_pozycja_nogi(int noga, double przechyl, double podniesienie, double *bark_rad, double *fh)
{
    if (noga < 0 || noga > 3 ) { return 0; }

    double przesuniecie;
    if (noga == 0 || noga == 2) { przesuniecie = przechyl; }
    else { przesuniecie = -przechyl; }
    *bark_rad = atan(przesuniecie / WYSOKOSC_BARK_STOPA);

    double zp = ((-WYSOKOSC_BARK_STOPA - PRZESUNIECIE_PLASZCZYZNY[noga] * sin(*bark_rad)) / cos(*bark_rad));
    *fh = zp + LINIA_BIODER_NAD_BARKIEM + podniesienie + ZAPAS_WYSOKOSCI;

    return 1;
}

static double rad_na_stopnie(double rad)
{
    return rad * (180 / PI);
}

// Funkcja liczy katy serw jednej nogi dla zadanego przechylu i podniesienia
int kin_nogi_serwa(int noga, double przechyl, double podniesienie, double *bark_deg, double *udo_F_deg, double *udo_B_deg, double przod)
{
    double bark_rad;
    double fh;
    double thF;
    double thB;

    int pk = kin_pozycja_nogi(noga, przechyl, podniesienie, &bark_rad, &fh);
    if (!pk) { return 0; }

    int ok = kin_odw_noga(przod, fh, &thF, &thB);
    if (!ok) { return 0; }

    *bark_deg = ZNAK[noga] * rad_na_stopnie(bark_rad);
    *udo_F_deg = OFFSET_UDA_DEG + ZNAK[noga] * rad_na_stopnie(thF);
    *udo_B_deg = OFFSET_UDA_DEG + ZNAK[noga] * rad_na_stopnie(thB);

    return 1;
}