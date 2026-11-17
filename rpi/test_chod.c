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

    double bark;
    double udo_F;
    double udo_B;
    double bark2;
    double udo_F2;
    double udo_B2;

    kinematyka_init();

    assert(chod_poza_nogi(0, 0, 1, &bark, &udo_F, &udo_B) == 1);
    assert(kin_nogi_serwa(0, PRZECHYL, 0.0, &bark2, &udo_F2, &udo_B2, 0.0) == 1);
    assert(fabs(bark - bark2) < 1e-9);
    assert(fabs(udo_F - udo_F2) < 1e-9);
    assert(fabs(udo_B - udo_B2) < 1e-9);

    printf("Test poprawnego skladania ruchu dla nogi 0 OK\n");

    assert(chod_poza_nogi(1, 0, 1, &bark, &udo_F, &udo_B) == 1);
    assert(kin_nogi_serwa(1, PRZECHYL, 0.0, &bark2, &udo_F2, &udo_B2, 0.0) == 1);
    assert(fabs(bark - bark2) < 1e-9);
    assert(fabs(udo_F - udo_F2) < 1e-9);
    assert(fabs(udo_B - udo_B2) < 1e-9);

    printf("Test poprawnego skladania ruchu dla nogi 1 OK\n");

    assert(chod_poza_nogi(2, 0, 2, &bark, &udo_F, &udo_B) == 1);
    assert(kin_nogi_serwa(2, PRZECHYL, PODNIESIENIE, &bark2, &udo_F2, &udo_B2, -DLUGOSC_KROKU) == 1);
    assert(fabs(bark - bark2) < 1e-9);
    assert(fabs(udo_F - udo_F2) < 1e-9);
    assert(fabs(udo_B - udo_B2) < 1e-9);

    printf("Test poprawnego skladania ruchu dla nogi 2 z ruchem w przod OK\n");

    assert(chod_poza_nogi(3, 0, 1, &bark, &udo_F, &udo_B) == 1);
    assert(kin_nogi_serwa(3, PRZECHYL, 0.0, &bark2, &udo_F2, &udo_B2, 0.0) == 1);
    assert(fabs(bark - bark2) < 1e-9);
    assert(fabs(udo_F - udo_F2) < 1e-9);
    assert(fabs(udo_B - udo_B2) < 1e-9);

    printf("Test popranego skladania ruchu dla nogi 3 OK\n");

    assert(chod_poza_nogi(0, -1, 1, &bark, &udo_F, &udo_B) == 0);

    printf("Test komendy z blednym krokiem OK\n");

    assert(chod_poza_nogi(0, 0, 5, &bark, &udo_F, &udo_B) == 0);

    printf("Test komendy z bledna faza OK\n");

    return 0;
}