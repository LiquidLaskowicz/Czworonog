#ifndef ENKODER_H
#define ENKODER_H

// Biblioteki
#include <stdint.h>

// Definicje parametrow
#define ENKODER_I2C1_BAUDRATE 100000
#define ENKODER_I2C1 i2c1
#define ENKODER_SDA1 14
#define ENKODER_SCL1 15
#define ENKODER_ADRES_UKLADU 0x36
#define ENKODER_ADRES_REJESTRU_KATA 0x0C
#define ENKODERY_PODLACZONE 0

#define ENKODER_POLOZENIE_ZEROWE 2048

#define ENKODERY_MUX 1
#define ENKODER_MUX_ADRES 0x70
#define ENKODER_LICZBA 6
#define ENKODER_ADRES_STATUSU 0x0B
#define ENKODER_MAGNES_WYKRYTY 0x20
#define ENKODER_TIMEOUT_US 3000
#define ENKODER_BLAD_I2C (-1)
#define ENKODER_BRAK_MAGNESU (-2)

// Deklaracje funkcji
void enkoder_init(void);
uint16_t czytaj_enkoder_raw(void);
float aktualny_kat_serwa_deg(uint16_t zero, uint16_t teraz);
void enkoder_mux_init(void);
int enkoder_mux_czytaj(int kanal);

#endif