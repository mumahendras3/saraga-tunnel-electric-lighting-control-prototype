#ifndef I2C_MUX_HELPER_FUNC_H
#define I2C_MUX_HELPER_FUNC_H

#include <Wire.h>

void tca_select(uint8_t address, uint8_t channel);
void tca_disable(uint8_t address);

#endif