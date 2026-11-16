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

    double bark;
    double udo_F;
    double udo_B;

    ko = kin_nogi_serwa(0, 0.0, 0.0, &bark, &udo_F, &udo_B, 0.0);
    assert(ko == 1);
    assert(fabs(bark) < 1e-2);
    assert(fabs(udo_F - 95.077) < 1e-2);
    assert(fabs(udo_B - 85.273) < 1e-2);

    printf("Test nogi 0 (1) OK\n");

    ko = kin_nogi_serwa(0, 0.07, 0.08, &bark, &udo_F, &udo_B, 0.0);
    assert(ko == 1);
    assert(fabs(bark - 9.778) < 1e-2);
    assert(fabs(udo_F - 115.478) < 1e-2);
    assert(fabs(udo_B - 67.715) < 1e-2);

    printf("Test nogi 0 (2) OK\n");

    ko = kin_nogi_serwa(1, 0.07, 0.0, &bark, &udo_F, &udo_B, 0.0);
    assert(ko == 1);
    assert(fabs(bark - 9.778) < 1e-2);
    assert(fabs(udo_F - 88.256) < 1e-2);
    assert(fabs(udo_B - 91.822) < 1e-2);

    printf("Test nogi 1 OK\n");

    ko = kin_nogi_serwa(4, 0.0, 0.0, &bark, &udo_F, &udo_B, 0.0);
    assert(ko == 0);

    printf("Test blednej nogi OK\n");

    ko = kin_nogi_serwa(0, 0.0, 0.0, &bark, &udo_F, &udo_B, 0.05);
    assert(ko == 1);
    assert(bark == 0);
    assert(fabs(udo_F - 103.39) < 1e-2);
    assert(fabs(udo_B - 95.21) < 1e-2);

    printf("Test niezerowego ruchu OK\n");

    ko = kin_nogi_serwa(1, 0.0, 0.0, &bark, &udo_F, &udo_B, 0.05);
    assert(ko == 1);
    assert(fabs(bark) < 1e-2);
    assert(fabs(udo_F - 76.61) < 1e-2);
    assert(fabs(udo_B - 84.79) < 1e-2);

    printf("Test niezerowego ruchu(2) OK\n");

    return 0;
}