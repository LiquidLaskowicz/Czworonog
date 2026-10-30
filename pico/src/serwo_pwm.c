// Biblioteki
#include "serwo_pwm.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Inicjalizacja serwa
void serwo_init (const serwo_t *s)
{
    gpio_set_function(s->numer_pinu, GPIO_FUNC_PWM);
    uint numer_bloku_pwm = pwm_gpio_to_slice_num(s->numer_pinu);
    pwm_set_clkdiv(numer_bloku_pwm, 125.0f); // 1 MHz
    pwm_set_wrap(numer_bloku_pwm, SERWO_OKRES_US - 1);
    pwm_set_enabled(numer_bloku_pwm, true);
}

// Ustawienie polozenia serwa
void serwo_ustaw_kat (const serwo_t *s, float kat_deg)
{
    uint16_t impuls = kat_na_impuls(s, kat_deg);
    pwm_set_gpio_level(s->numer_pinu, impuls);
}