#ifndef CHOD_H
#define CHOD_H

// Definicje parametrow
#define LICZBA_NOG 4
#define LICZBA_FAZ 5
#define DLUGOSC_KROKU 0.04

typedef struct // to sa ulamki wartosci z define
{
    double przechyl;
    double uniesienie;
    double przod;
} faza_t;

typedef enum
{  
    KIER_PRZOD = 0,
    KIER_LEWO = 1,
    KIER_PRAWO = 2
} kierunek_t;

// Deklaracje funkcji
int chod_noga_przenoszona(int krok);
double chod_przechyl(int noga);
const faza_t *chod_faza(int nr);
int chod_poza_nogi(int noga, int krok, int faza, double *bark_deg, double *udo_F_deg, double *udo_B_deg);
int chod_poza_nogi_kierunek(int noga, int krok, int faza, kierunek_t kierunek, double *bark_deg, double *udo_F_deg, double *udo_B_deg);

#endif