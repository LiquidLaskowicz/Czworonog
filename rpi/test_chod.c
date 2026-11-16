// Biblioteki
#include "chod.h"
#include <assert.h>
#include <stdio.h>
#include "kinematyka.h"
#include <math.h>

int main(void)
{
    int przen;
    przen = chod_noga_przenoszona(-1);
    assert(przen == -1);

    przen = chod_noga_przenoszona(2);
    assert(przen == 3);

    przen = chod_noga_przenoszona(5);
    assert(przen == 0);

    przen = chod_noga_przenoszona(7);
    assert(przen == 1);

    printf("Test zapetlania kolejnosci nog OK\n");

    assert(chod_przechyl(-1) == 0.0);
    assert(fabs(chod_przechyl(2) - PRZECHYL) < 1e-9);
    assert(fabs(chod_przechyl(3) + PRZECHYL) < 1e-9);

    printf("Test poprawnosci przechylu dla nog OK\n");

    assert(chod_faza(-1) == NULL);
    assert(chod_faza(5) == NULL);
    assert(chod_faza(2)->przod == 1);
    assert(chod_faza(0)->uniesienie == 0);
    assert(chod_faza(4)->przechyl == 0);

    printf("Test poprawnosci fazy chodu OK\n");

    return 0;
}