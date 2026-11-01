// Biblioteki
#include "enkoder.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Inicjalizacja enkodera
void enkoder_init(void)
{
    i2c_init(ENKODER_I2C1, ENKODER_I2C1_BAUDRATE);
    gpio_set_function(ENKODER_SCL1, GPIO_FUNC_I2C);
    gpio_set_function(ENKODER_SDA1, GPIO_FUNC_I2C);
}

// Odczyt surowych danych z enkodera
uint16_t czytaj_enkoder_raw(void)
{
    uint8_t adres_rejestru_kata = ENKODER_ADRES_REJESTRU_KATA;
    i2c_write_blocking(ENKODER_I2C1, ENKODER_ADRES_UKLADU, &adres_rejestru_kata, 1, true);
    uint8_t dane[2];
    i2c_read_blocking(ENKODER_I2C1, ENKODER_ADRES_UKLADU, dane, 2, false);

    uint16_t wynik = dane[0] * 256 + dane[1];

    wynik = wynik & 0xFFF;

    return wynik;
}

// Zwracanie pozycji serwa w stopniach wzgledem polozenia zerowego
float aktualny_kat_serwa_deg(uint16_t zero, uint16_t teraz)
{
    float przelicznik = 360/4096.0f;
    float zero_deg = zero * przelicznik;
    float teraz_deg = teraz * przelicznik;
    float wynik = teraz_deg - zero_deg;

    if(zero_deg < 90 && wynik > 180)
    {
        wynik = wynik - 360;
    }
    else if (zero_deg > 270 && wynik < -180)
    {
        wynik = wynik + 360;
    }

    return wynik;
}