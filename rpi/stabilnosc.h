#ifndef STABILNOSC_H
#define STABILNOSC_H

static double strona(const double a[2], const double b[2], const double p[2]);
double margines_stabilnosci(const double stopy[4][2], const int w_podparciu[4], const double cog[2]);
int mozna_podniesc_noge(const double stopy[4][2], const int w_podparciu[4], const double cog[2], int noga);

#endif