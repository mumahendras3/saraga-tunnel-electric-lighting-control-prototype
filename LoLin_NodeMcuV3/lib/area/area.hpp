#ifndef AREA_H
#define AREA_H

#include <hp_BH1750.h>
#include <Adafruit_NeoPixel.h>
#include <i2c_mux_helper_func.hpp>

// I2C device address definitions
#define TCA_ADDR_DEF 0x70
// Total number of I2C mux used
#define NUM_TCA 2
// Total number of I2C channels per I2C mux
#define NUM_TCA_CH 8
// Maximum number of light sensors that can be used
#define MAX_NUM_BH1750 NUM_TCA*NUM_TCA_CH*2

// Luminous intensity of the red, green, and blue LED for each pixel in the LED strip
// in mcd (milicandela) (taken from datasheet)
#define LUM_RED 310
#define LUM_GREEN 780
#define LUM_BLUE 215

class area {
    private:
    // hp_BH1750 object array to interact with the sensors
    hp_BH1750 sensor[];
    // Total number of sensors in the area
    uint8_t num_sensors;
    // Total number of LEDs and the index of first LED in the area
    uint8_t num_leds, led_index;
    // LED group distance from measurement point in the area
    float distance;

    public:
    // The address for each sensor in the area
    // The address format is {I2C_Mux_Number, I2C_Mux_Channel, Low/High_Addr_Pin}
    uint8_t addr[][3];
    // Average illuminance for the area
    float avg_lux;
    // Average illuminance target for the area
    float avg_lux_target;
    // The current RGB value of all LEDs in the area
    uint8_t red = 255, green = 255, blue = 255;
    // The new RGB value of all LEDs in the area (received from the server)
    uint8_t new_red = 255, new_green = 255, new_blue = 255;
    // The brightness of all LEDs in the area (range: 0%-100%)
    uint8_t brightness = 0;
    // Class constructor
    area(uint8_t num_sensors, uint8_t num_leds, uint8_t led_index, float distance);
    // Method to set sensor addresses for the area
    void set_address(uint8_t arr[][3]);
    // Method to initialize all sensors in the area
    void sensors_init();
    // Method to start measurement for all sensors in the area
    void sensors_start(bool forcePreShot);
    // Method to check if all sensors in the area have finished measuring
    bool sensors_finished();
    // Method to calculate the average illuminance of all sensors in the area
    void calc_lux();
    // Method to apply the brightness and RGB value of this LED group to the LED strip
    void leds_apply(Adafruit_NeoPixel * strip);
    // Method to adapt the brightness value so that the average illuminance target can be achieved
    void brightness_adapt();
};

#endif