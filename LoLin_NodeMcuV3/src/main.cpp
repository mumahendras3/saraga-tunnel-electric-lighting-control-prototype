/*
  Source code for LoLin NodeMCU V3 Microcontroller
*/

// Include some needed libraries
#include <pubsubclient_helper_func.hpp>
#include <area.hpp>
#include <RTClib.h>

// Pin definitions
#define PIN_LED_STRIP D7
#define PIN_SDA D3
#define PIN_SCL D2
#define PIN_INTERRUPT D1

// Global variables
const char* ssid = "Wisma Thullabul Ilmi";
const char* password = "udruskitabersama";
const char* mqtt_server = "192.168.1.3";
bool update_strip = false;
volatile bool start_measuring = false;
volatile bool send_data = false;

// Sensor addresses for each area
// one row per sensor, first column for I2C_Mux_Number, second column
// for I2C_Mux_Channel, and third column for Low(0)/High(1)_Addr_Pin
uint8_t meja_dan_kursi_1_address[NUM_BH1750_MEJA_DAN_KURSI_1][3] = {
    {0, 0, 0}
};
uint8_t meja_dan_kursi_2_address[NUM_BH1750_MEJA_DAN_KURSI_2][3] = {
    {0, 0, 0}
};
uint8_t meja_dan_kursi_3_address[NUM_BH1750_MEJA_DAN_KURSI_3][3] = {
    {0, 0, 0}
};
uint8_t pameran_poster_1_address[NUM_BH1750_PAMERAN_POSTER_1][3] = {
    {0, 0, 0}
};
uint8_t pameran_poster_2_address[NUM_BH1750_PAMERAN_POSTER_2][3] = {
    {0, 0, 0}
};
uint8_t panggung_serbaguna_address[NUM_BH1750_PANGGUNG_SERBAGUNA][3] = {
    {0, 0, 0},
    {0, 0, 0}
};
uint8_t pejalan_kaki_1_address[NUM_BH1750_PEJALAN_KAKI_1][3] = {
    {0, 0, 0},
    {0, 0, 0}
};
uint8_t pejalan_kaki_2_address[NUM_BH1750_PEJALAN_KAKI_2][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};
uint8_t pejalan_kaki_3_address[NUM_BH1750_PEJALAN_KAKI_3][3] = {
    {0, 0, 0},
    {0, 0, 0}
};
uint8_t pejalan_kaki_4_address[NUM_BH1750_PEJALAN_KAKI_4][3] = {
    {0, 7, 0},
    {0, 2, 1}
};

// Object initializations
// Wifi
WiFiClient espClient;
PubSubClient client(espClient);
// LED strip
Adafruit_NeoPixel strip(NUM_LED, PIN_LED_STRIP);
// Real Time Clock
RTC_DS3231 rtc;
// DateTime for storing current time
DateTime now;
// Area
// area meja_dan_kursi_1(NUM_BH1750_MEJA_DAN_KURSI_1, NUM_LED_MEJA_DAN_KURSI_1, INDEX_LED_MEJA_DAN_KURSI_1, DIST_LED_MEJA_DAN_KURSI_1);
// area meja_dan_kursi_2(NUM_BH1750_MEJA_DAN_KURSI_2, NUM_LED_MEJA_DAN_KURSI_2, INDEX_LED_MEJA_DAN_KURSI_2, DIST_LED_MEJA_DAN_KURSI_2);
// area meja_dan_kursi_3(NUM_BH1750_MEJA_DAN_KURSI_3, NUM_LED_MEJA_DAN_KURSI_3, INDEX_LED_MEJA_DAN_KURSI_3, DIST_LED_MEJA_DAN_KURSI_3);
// area pameran_poster_1(NUM_BH1750_PAMERAN_POSTER_1, NUM_LED_PAMERAN_POSTER_1, INDEX_LED_PAMERAN_POSTER_1, DIST_LED_PAMERAN_POSTER_1);
// area pameran_poster_2(NUM_BH1750_PAMERAN_POSTER_2, NUM_LED_PAMERAN_POSTER_2, INDEX_LED_PAMERAN_POSTER_2, DIST_LED_PAMERAN_POSTER_2);
// area panggung_serbaguna(NUM_BH1750_PANGGUNG_SERBAGUNA, NUM_LED_PANGGUNG_SERBAGUNA, INDEX_LED_PANGGUNG_SERBAGUNA, DIST_LED_PANGGUNG_SERBAGUNA);
// area pejalan_kaki_1(NUM_BH1750_PEJALAN_KAKI_1, NUM_LED_PEJALAN_KAKI_1, INDEX_LED_PEJALAN_KAKI_1, DIST_LED_PEJALAN_KAKI_1);
// area pejalan_kaki_2(NUM_BH1750_PEJALAN_KAKI_2, NUM_LED_PEJALAN_KAKI_2, INDEX_LED_PEJALAN_KAKI_2, DIST_LED_PEJALAN_KAKI_2);
// area pejalan_kaki_3(NUM_BH1750_PEJALAN_KAKI_3, NUM_LED_PEJALAN_KAKI_3, INDEX_LED_PEJALAN_KAKI_3, DIST_LED_PEJALAN_KAKI_3);
area pejalan_kaki_4(NUM_BH1750_PEJALAN_KAKI_4, NUM_LED_PEJALAN_KAKI_4, 0, 8.5/100);

// Callback function definition
void callback(char* topic, byte* payload, unsigned int length) {
    // For debugging
    //Serial.print("Message arrived [");
    //Serial.print(topic);
    //Serial.print("] ");
    //for (int i = 0; i < length; i++) {
    //  Serial.print((char)payload[i]);
    //}
    //Serial.println();

    // Set time to server time
    if (strcmp(topic, "set/time") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        rtc.adjust(DateTime(atoi(number)));
    }
    // Conditional statements to handle various topic
    // For area 'Pejalan Kaki 4'
    else if (strcmp(topic, "set/lamp/pejalan-kaki-4/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_4.new_brightness = atof(number)/100;
        pejalan_kaki_4.automatic = false;
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-4/red") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_4.new_red = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-4/green") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_4.new_green = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-4/blue") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_4.new_blue = atoi(number);
    }
    else if (strcmp(topic, "set/illuminance/pejalan-kaki-4") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_4.avg_lux_target = atof(number);
    }
}
ICACHE_RAM_ATTR void interrupt_function() {
    start_measuring = true;
    send_data = false;
}

// Setup routines
void setup() {
    Serial.begin(9600);
    while (!Serial); // loop until serial monitor is ready

    // Set sensor addresses for each area
    // meja_dan_kursi_1.set_address(meja_dan_kursi_1_address);
    // meja_dan_kursi_2.set_address(meja_dan_kursi_2_address);
    // meja_dan_kursi_3.set_address(meja_dan_kursi_3_address);
    // pameran_poster_1.set_address(pameran_poster_1_address);
    // pameran_poster_2.set_address(pameran_poster_2_address);
    // panggung_serbaguna.set_address(panggung_serbaguna_address);
    // pejalan_kaki_1.set_address(pejalan_kaki_1_address);
    // pejalan_kaki_2.set_address(pejalan_kaki_2_address);
    // pejalan_kaki_3.set_address(pejalan_kaki_3_address);
    pejalan_kaki_4.set_address(pejalan_kaki_4_address);

    // Start I2C communication bus
    Wire.begin(PIN_SDA, PIN_SCL);
    // Make sure all I2C mux channel are disabled at first start
    for (uint8_t i = 0; i < NUM_TCA; i++) tca_disable(TCA_ADDR_DEF + i);

    // Sensors initialization
    pejalan_kaki_4.sensors_init();
    // First time measurement
    pejalan_kaki_4.sensors_start(true);

    // RTC initialization
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        abort();
    }
    if (rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    // Disable the 32K pin
    rtc.disable32K();
    // Clear the last alarm status
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    // Stop oscillating signals at SQW Pin, otherwise setAlarm1 will fail
    // rtc.writeSqwPinMode(DS3231_OFF);
    // Disable alarm 2 since we don't use it
    rtc.disableAlarm(2);
    // Set alarm to trigger every minute
    if(!rtc.setAlarm1(
            DateTime(0, 0, 0, 0, 0, 0),
            DS3231_A1_Second // this mode triggers the alarm when the seconds match. See Doxygen for other options
    )) {
        Serial.println("Error, alarm wasn't set!");
    } else {
        Serial.println("Alarm will happen every minute!");  
    }

    // Interrupt service routine initialization
    pinMode(PIN_INTERRUPT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), interrupt_function, FALLING);

    // Setup LED Strip
    strip.begin();
    strip.setBrightness(255); // this should be called once for the entire sketch life
    strip.show(); // Initialize all pixels to 'off'

    // Setup WIFI and MQTT connection
    setup_wifi(ssid, password);
    client.setServer(mqtt_server, 1883);
    // Attach a callback function to handle incoming MQTT messages
    client.setCallback(callback);
}

// Main loop
void loop() {
    if (!client.connected()) {
        reconnect(&client);
    }
    // Process incoming MQTT messages
    client.loop();

    // Start measuring after 1 minute elapsed
    if (start_measuring) {
        pejalan_kaki_4.sensors_start(true);
        start_measuring = false;
        rtc.clearAlarm(1);
    }

    // Process incoming MQTT messages again
    client.loop();

    // If we are in first phase (LED filling phase)
    if (!send_data) update_strip = true;
    // Check if all sensors in area 'Pejalan Kaki 4' have finished measuring and
    // the average illuminance isn't calculated yet
    // We put lux_calculated() function call first to avoid costly I2C communication
    // to the sensors when the average illuminance for the area has already been calculated
    if (!pejalan_kaki_4.lux_calculated()) {
        if (pejalan_kaki_4.sensors_finished()) {
            pejalan_kaki_4.calc_lux();
            if (send_data) {
                Serial.print("Now the time is: ");
                Serial.println(now.timestamp(DateTime::TIMESTAMP_TIME));
                Serial.print("Avg Lux for 'Pejalan Kaki 4': ");
                Serial.println(pejalan_kaki_4.avg_lux);
                Serial.print("Avg Lux target for 'Pejalan Kaki 4': ");
                Serial.println(pejalan_kaki_4.avg_lux_target);
                Serial.print("brightness for 'Pejalan Kaki 4': ");
                Serial.println(pejalan_kaki_4.brightness);
                Serial.print("new_red for 'Pejalan Kaki 4': ");
                Serial.println(pejalan_kaki_4.new_red);
                Serial.print("new_green for 'Pejalan Kaki 4': ");
                Serial.println(pejalan_kaki_4.new_green);
                Serial.print("new_blue for 'Pejalan Kaki 4': ");
                Serial.println(pejalan_kaki_4.new_blue);
                Serial.println();
                String time = now.timestamp();
                String packet = String(pejalan_kaki_4.red);
                packet += "|" + String(pejalan_kaki_4.green);
                packet += "|" + String(pejalan_kaki_4.blue);
                packet += "|" + String(pejalan_kaki_4.brightness*100);
                packet += "|" + time;
                client.publish("status/lamp/pejalan-kaki-4", packet.c_str(), true);
                packet = String(pejalan_kaki_4.avg_lux);
                packet += "|" + String(pejalan_kaki_4.avg_lux_target);
                packet += "|" + time;
                client.publish("status/illuminance/pejalan-kaki-4", packet.c_str(), true);
            }
            else {
                pejalan_kaki_4.leds_update();
                pejalan_kaki_4.leds_apply(&strip);
            }
        }
    }
    // This make sure all area have applied their respective color/brightness settings to the
    // LED strip object before actually updating the LED strip
    update_strip = update_strip && pejalan_kaki_4.lux_calculated();

    // Process incoming MQTT messages again
    client.loop();

    // Update the LED strip at once
    if (update_strip) {
        strip.show();
        // Wait a couple milisecond to make sure that all LEDs have received
        // their intended RGB values (as sent by the microcontroller)
        delay(1000);
        // Start a new measurement after all LEDs stabilised to get the average illuminance of
        // the area after the brightness was adjusted
        // The results from this new measurement are the ones that will be sent to the server
        pejalan_kaki_4.sensors_start(false);
        now = rtc.now();
        // Send the current time of RTC to the server for time synchronization purposes
        String time = String(now.unixtime());
        client.publish("status/time", time.c_str());
        send_data = true;
        update_strip = false;
    }

    // Process incoming MQTT messages again
    client.loop();
}