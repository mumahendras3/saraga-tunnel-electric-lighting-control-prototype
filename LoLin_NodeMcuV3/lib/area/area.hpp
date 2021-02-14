#ifndef AREA_H
#define AREA_H

#include <hp_BH1750.h>
#include <Adafruit_NeoPixel.h>
#include <i2c_mux_helper_func.hpp>
#include <constants.hpp>

// 'area' class definition
class area {
    private:
    // hp_BH1750 object array to interact with the sensors
    hp_BH1750 sensor[MAX_NUM_BH1750];
    // Total number of sensors in the area
    uint8_t num_sensors;
    // Total number of LEDs and the index of first LED in the area
    uint8_t num_leds, led_index;
    // LED group distance from measurement point in the area
    float distance;
    // Lux calculation status
    bool calculated;

    public:
    // The address for each sensor in the area
    // The address format is {I2C_Mux_Number, I2C_Mux_Channel, Low/High_Addr_Pin}
    uint8_t addr[MAX_NUM_BH1750][3];
    // Average illuminance for the area
    float avg_lux = -1;
    // Average illuminance target for the area
    float avg_lux_target = 43;
    // // Smallest measured illuminance of the area
    // float lux_min;
    // // The overall uniformity of the area
    // float uniformity;
    // The current RGB value of all LEDs in the area
    uint8_t red = 255, green = 255, blue = 255;
    // The new RGB value of all LEDs in the area (received from the server)
    uint8_t new_red = 255, new_green = 255, new_blue = 255;
    // The brightness of all LEDs in the area (range: 0-1)
    float brightness = 0;
    // The new brightness of all LEDs in the area (received from the server) (range: 0-1)
    float new_brightness = 0;
    // The flag to check for operating mode
    bool automatic = true;
    // Class constructor
    area(uint8_t num_sensors, uint8_t num_leds, uint8_t led_index, float distance);
    // Method to set sensor addresses for the area
    void set_address(uint8_t arr[][3]);
    // Method to set luxFactor of n'th sensor in the area to 'factor'
    void set_luxFactor(uint8_t n, float factor);
    // Method to initialize all sensors in the area
    void sensors_init();
    // Method to start measurement for all sensors in the area
    void sensors_start(bool forcePreShot);
    // Method to check if all sensors in the area have finished measuring
    bool sensors_finished();
    // Method to calculate the average illuminance of all sensors in the area
    void calc_lux();
    // // Method to calculate the overall uniformity of the area
    // void calc_uniformity();
    // Method to check the average lux calculation status in the area
    bool lux_calculated();
    // Method to update the brightness and RGB value of the LED group
    void leds_update();
    // Method to apply the brightness and RGB value of this LED group to the LED strip
    void leds_apply(Adafruit_NeoPixel *strip);
};

#endif