// Biblioteki
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "protokol.h"

int main (void)
{
    // Test budowy ramki
    char bufor[32];
    int n = buduj_ramke_kat(bufor, sizeof(bufor), 0, 10.0f);
    assert(n==10);
    assert(strcmp(bufor, "S 0 10.00\n") == 0);

    printf("Test ramki ok\n");

    // Test ramki dla innego kanalu i wartosci kata
    n = buduj_ramke_kat(bufor, sizeof(bufor), 2, -5.50f);
    assert(n==10);
    assert(strcmp(bufor, "S 2 -5.50\n") == 0);

    // Test parsowania telemetrii
    uint32_t czas = 0;
    float zadany = 0;
    float zmierzony = 0;
    uint16_t surowy = 0;

    int pars = parsuj_telemetrie("123456,10.00,9.84,2137", &czas, &zadany, &zmierzony, &surowy);
    assert(pars == 1);
    assert(czas == 123456);
    assert(surowy == 2137);

    printf("Test parsu telemetrii ok\n");

    int kanal = -1;
    float kat = 0;

    assert(parsuj_ramke_kat("S 2 -5.50", &kanal, &kat) == 1);
    assert(kanal == 2);
    assert(kat == -5.50f);
    assert(parsuj_ramke_kat("S 30.00", &kanal, &kat) == 0);
    assert(parsuj_ramke_kat("S 2.00", &kanal, &kat) == 0);
    assert(parsuj_ramke_kat("S 2 nan", &kanal, &kat) == 0);
    assert(parsuj_ramke_kat("S 300 5", &kanal, &kat) == 0);
    assert(parsuj_ramke_kat("S 2", &kanal, &kat) == 0);

    printf("Test parsowania ramki OK\n");

    return 0;
}