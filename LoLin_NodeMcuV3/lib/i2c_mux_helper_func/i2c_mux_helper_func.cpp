#include "i2c_mux_helper_func.hpp"

void tca_select(uint8_t address, uint8_t channel) {
    Wire.beginTransmission(address);
    Wire.write(1 << channel);
    Wire.endTransmission();  
}

void tca_disable(uint8_t address) {
    Wire.beginTransmission(address);
    Wire.write(0);  // no channel selected
    Wire.endTransmission();
}