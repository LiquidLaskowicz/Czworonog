#ifndef KINEMATYKA_H
#define KINEMATYKA_H

// Definicje parametrow
#define PI 3.14159265358979323846
#define POLOWA_ROZSTAWU_SERW_UDO 0.0265
#define DLUGOSC_UDO 0.181
#define DLUGOSC_LYDKA 0.263
#define ZLACZE_LYDKI_U 0.0005
#define ZLACZE_LYDKI_W (-0.3286)
#define STOPA_PRZES_U 0.0004
#define STOPA_PRZES_W (-0.0376)
#define LINIA_BIODER_NAD_BARKIEM 0.04
#define ZAPAS_WYSOKOSCI 0.02
#define WYSOKOSC_BARK_STOPA (LINIA_BIODER_NAD_BARKIEM - (ZLACZE_LYDKI_W + STOPA_PRZES_W))

// Deklaracje funkcji
void kinematyka_init(void);
int kin_odw_noga(double fu, double fw, double *thF, double *thB);

#endif