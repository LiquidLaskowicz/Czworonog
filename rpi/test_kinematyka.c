// Biblioteki
#include "kinematyka.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>

int main(void)
{
    double a;
    double b;
    int ko;

    // Test pozycji domowej
    kinematyka_init();
    ko = kin_odw_noga(ZLACZE_LYDKI_U + STOPA_PRZES_U, ZLACZE_LYDKI_W + STOPA_PRZES_W, &a, &b);
    assert(ko == 1);
    assert(fabs(a) < 1e-6);
    assert(fabs(b) < 1e-6);

    printf("Test pozycji domowej OK\n");

    // Test punktu refencyjnego z Weebots
    ko = kin_odw_noga(-0.12, -0.44, &a, &b);
    assert(ko == 1);
    assert(fabs(a + 0.891424) < 1e-5);
    assert(fabs(b - 0.152987) < 1e-5);

    printf("Test punktu referencyjnego OK\n");

    // Test pozycji nieosiagalnej
    ko = kin_odw_noga(0.0, -0.8, &a, &b);
    assert(ko == 0);

    printf("Test pozycji nieosiagalnej OK\n");

    return 0;
}