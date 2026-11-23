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

void enkoder_mux_init(void)
{
    i2c_init(ENKODER_I2C1, 400000);
    gpio_set_function(ENKODER_SCL1, GPIO_FUNC_I2C);
    gpio_set_function(ENKODER_SDA1, GPIO_FUNC_I2C);
    gpio_pull_up(ENKODER_SDA1);
    gpio_pull_up(ENKODER_SCL1);
}

int enkoder_mux_czytaj(int kanal)
{
    if (kanal < 0 || kanal >= ENKODER_LICZBA) { return ENKODER_BLAD_I2C; }

    uint8_t wybor = (uint8_t)(1u << kanal);
    if (i2c_write_timeout_us(ENKODER_I2C1, ENKODER_MUX_ADRES, &wybor, 1, false, ENKODER_TIMEOUT_US) != 1)
    {
        return ENKODER_BLAD_I2C;
    }

    uint8_t rejestr = ENKODER_ADRES_STATUSU;
    uint8_t status;
    if (i2c_write_timeout_us(ENKODER_I2C1, ENKODER_ADRES_UKLADU, &rejestr, 1, true, ENKODER_TIMEOUT_US) != 1 ||
        i2c_read_timeout_us(ENKODER_I2C1, ENKODER_ADRES_UKLADU, &status, 1, false, ENKODER_TIMEOUT_US) != 1)
    {
        return ENKODER_BLAD_I2C;
    }
    if (!(status & ENKODER_MAGNES_WYKRYTY)) { return ENKODER_BRAK_MAGNESU; }

    rejestr = ENKODER_ADRES_REJESTRU_KATA;
    uint8_t dane[2];
    if (i2c_write_timeout_us(ENKODER_I2C1, ENKODER_ADRES_UKLADU, &rejestr, 1, true, ENKODER_TIMEOUT_US) != 1 ||
        i2c_read_timeout_us(ENKODER_I2C1, ENKODER_ADRES_UKLADU, dane, 2, false, ENKODER_TIMEOUT_US) != 2)
    {
        return ENKODER_BLAD_I2C;
    }

    return ((dane[0] << 8) | dane[1]) & 0x0FFF;
}

