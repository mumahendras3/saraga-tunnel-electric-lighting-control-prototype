#include "area.hpp"

// Class for creating an area object that consists of an LED group and a sensor group
area::area(uint8_t num_sensors, uint8_t num_leds, uint8_t led_index, float distance) : 
    num_sensors(num_sensors),
    num_leds(num_leds),
    led_index(led_index),
    distance(distance)
    {}
// Method to set sensor addresses for the area
void area::set_address(uint8_t arr[][3]) {
    for (uint8_t i = 0; i < num_sensors; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            addr[i][j] = arr[i][j];
        }
    }
}
// Method to initialize all sensors in the area
void area::sensors_init() {
    for (uint8_t n = 0; n < num_sensors; n++) {
        bool initialized = false;
        tca_select(TCA_ADDR_DEF + addr[n][0], addr[n][1]);
        if (addr[n][2] == 0) {
            initialized = sensor[n].begin(BH1750_TO_GROUND);
        }
        else {
            initialized = sensor[n].begin(BH1750_TO_VCC);
        }
        if (initialized) {
            sensor[n].calibrateTiming();
        }
        else {
            // Print error if no sensor is found
            Serial.print("Couldn't find sensor at {");
            Serial.print(addr[n][0] + ", ");
            Serial.print(addr[n][1] + ", ");
            Serial.println(addr[n][2] + "}");
        }
        tca_disable(TCA_ADDR_DEF + addr[n][0]);
    }
}
// Method to start measurement for all sensors in the area
void area::sensors_start(bool forcePreShot) {
    calculated = false;
    for (uint8_t n = 0; n < num_sensors; n++) {
        tca_select(TCA_ADDR_DEF + addr[n][0], addr[n][1]);
        sensor[n].adjustSettings(90, forcePreShot);
        sensor[n].start();
        tca_disable(TCA_ADDR_DEF + addr[n][0]);
    }
}
// Method to check if all sensors in the area have finished measuring
bool area::sensors_finished() {
    bool finished = true;
    for (uint8_t n = 0; n < num_sensors; n++) {
        tca_select(TCA_ADDR_DEF + addr[n][0], addr[n][1]);
        finished = finished && sensor[n].hasValue();
        tca_disable(TCA_ADDR_DEF + addr[n][0]);
    }
    return finished;
}
// Method to calculate the average illuminance of all sensors in the area
void area::calc_lux() {
    float sum = 0;
    for (uint8_t n = 0; n < num_sensors; n++) {
      sum += sensor[n].getLux();
    }
    avg_lux = sum / num_sensors;
    calculated = true;
}
// Method to check the average lux calculation status in the area
bool area::lux_calculated() {
    return calculated;
}
// Method to update the brightness and RGB value of the LED group
void area::leds_update() {
    if (automatic) {
        new_brightness =
        (brightness*(LUM_RED*red + LUM_GREEN*green + LUM_BLUE*blue) + (avg_lux_target - avg_lux)*255000*distance*distance/num_leds) /
        (LUM_RED*new_red + LUM_GREEN*new_green + LUM_BLUE*new_blue);
        // Some sanity checks
        if (new_brightness > 1) new_brightness = 1;
        else if (new_brightness < 0) new_brightness = 0;
    }
    else {
        // Reset the color back to white in manual mode (i.e. the new_brightness is set by the server)
        new_red = 255;
        new_green = 255;
        new_blue = 255;
        // Reset the mode back to automatic
        automatic = true;
    }
    brightness = new_brightness;
    red = new_red;
    green = new_green;
    blue = new_blue;
}
// Method to apply the brightness and RGB value of this LED group to the LED strip
void area::leds_apply(Adafruit_NeoPixel *strip) {
    uint8_t r = red * brightness;
    uint8_t g = green * brightness;
    uint8_t b = blue * brightness;
    uint32_t color = strip->Color(r, g, b);
    strip->fill(color, led_index, num_leds);
}