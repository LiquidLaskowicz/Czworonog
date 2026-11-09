// Biblioteki
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "protokol.h"

int main (void)
{
    // Test budowy ramki
    char bufor[32];
    int n = buduj_ramke_kat(bufor, sizeof(bufor), 10.0f);
    assert(n==8);
    assert(strcmp(bufor, "S 10.00\n") == 0);

    printf("Test ramki ok\n");

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

    return 0;
}