// Biblioteki
#include "przekaznik.h"
#include "pico/stdlib.h"

void przekaznik_init(uint8_t pin)
{
    gpio_init(pin);
    gpio_put(pin, PRZEKAZNIK_STAN_ODCIECIA);
    gpio_set_dir(pin, GPIO_OUT);
}

void przekaznik_zasilanie(uint8_t pin, bool wlaczone)
{
    if (wlaczone) { gpio_put(pin, !PRZEKAZNIK_STAN_ODCIECIA); }
    else { gpio_put(pin, PRZEKAZNIK_STAN_ODCIECIA); }
}