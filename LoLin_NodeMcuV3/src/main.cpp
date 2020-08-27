/*
  Source code for LoLin NodeMCU V3 Microcontroller
*/

// Include some needed libraries
#include <pubsubclient_helper_func.hpp>
#include <area.hpp>

// Pin definitions
#define PIN_LED_STRIP D7
#define PIN_SDA D3
#define PIN_SCL D2

// Total number of light sensors used at area 'Meja dan Kursi 1'
#define NUM_BH1750_MEJA_DAN_KURSI_1 1
// Total number of light sensors used at area 'Meja dan Kursi 2'
#define NUM_BH1750_MEJA_DAN_KURSI_2 1
// Total number of light sensors used at area 'Meja dan Kursi 3'
#define NUM_BH1750_MEJA_DAN_KURSI_3 1
// Total number of light sensors used at area 'Pameran Poster 1'
#define NUM_BH1750_PAMERAN_POSTER_1 1
// Total number of light sensors used at area 'Pameran Poster 2'
#define NUM_BH1750_PAMERAN_POSTER_2 1
// Total number of light sensors used at area 'Panggung Serbaguna'
#define NUM_BH1750_PANGGUNG_SERBAGUNA 2
// Total number of light sensors used at area 'Pejalan Kaki 1'
#define NUM_BH1750_PEJALAN_KAKI_1 2
// Total number of light sensors used at area 'Pejalan Kaki 2'
#define NUM_BH1750_PEJALAN_KAKI_2 6
// Total number of light sensors used at area 'Pejalan Kaki 3'
#define NUM_BH1750_PEJALAN_KAKI_3 2
// Total number of light sensors used at area 'Pejalan Kaki 4'
#define NUM_BH1750_PEJALAN_KAKI_4 2

// Index of first LED in area 'Meja dan Kursi 1'
#define INDEX_LED_MEJA_DAN_KURSI_1 10
// Total number of LEDs used at area 'Meja dan Kursi 1'
#define NUM_LED_MEJA_DAN_KURSI_1 6
// Distance (in meter) from LEDs to measuring point in area 'Meja dan Kursi 1'
#define DIST_LED_MEJA_DAN_KURSI_1 0.16

// Index of first LED in area 'Meja dan Kursi 2'
#define INDEX_LED_MEJA_DAN_KURSI_2 16
// Total number of LEDs used at area 'Meja dan Kursi 2'
#define NUM_LED_MEJA_DAN_KURSI_2 5
// Distance (in meter) from LEDs to measuring point in area 'Meja dan Kursi 2'
#define DIST_LED_MEJA_DAN_KURSI_2 0.16

// Index of first LED in area 'Meja dan Kursi 3'
#define INDEX_LED_MEJA_DAN_KURSI_3 21
// Total number of LEDs used at area 'Meja dan Kursi 3'
#define NUM_LED_MEJA_DAN_KURSI_3 6
// Distance (in meter) from LEDs to measuring point in area 'Meja dan Kursi 3'
#define DIST_LED_MEJA_DAN_KURSI_3 0.16

// Index of first LED in area 'Pameran Poster 1'
#define INDEX_LED_PAMERAN_POSTER_1 34
// Total number of LEDs used at area 'Pameran Poster 1'
#define NUM_LED_PAMERAN_POSTER_1 3
// Distance (in meter) from LEDs to measuring point in area 'Pameran Poster 1'
#define DIST_LED_PAMERAN_POSTER_1 0.22

// Index of first LED in area 'Pameran Poster 2'
#define INDEX_LED_PAMERAN_POSTER_2 37
// Total number of LEDs used at area 'Pameran Poster 2'
#define NUM_LED_PAMERAN_POSTER_2 4
// Distance (in meter) from LEDs to measuring point in area 'Pameran Poster 2'
#define DIST_LED_PAMERAN_POSTER_2 0.22

// Index of first LED in area 'Panggung Serbaguna'
#define INDEX_LED_PANGGUNG_SERBAGUNA 8
// Total number of LEDs used at area 'Panggung Serbaguna'
#define NUM_LED_PANGGUNG_SERBAGUNA 2
// Distance (in meter) from LEDs to measuring point in area 'Panggung Serbaguna'
#define DIST_LED_PANGGUNG_SERBAGUNA 0.15

// Index of first LED in area 'Pejalan Kaki 1'
#define INDEX_LED_PEJALAN_KAKI_1 0
// Total number of LEDs used at area 'Pejalan Kaki 1'
#define NUM_LED_PEJALAN_KAKI_1 8
// Distance (in meter) from LEDs to measuring point in area 'Pejalan Kaki 1'
#define DIST_LED_PEJALAN_KAKI_1 0.22

// Index of first LED in area 'Pejalan Kaki 2'
#define INDEX_LED_PEJALAN_KAKI_2 27
// Total number of LEDs used at area 'Pejalan Kaki 2'
#define NUM_LED_PEJALAN_KAKI_2 7
// Distance (in meter) from LEDs to measuring point in area 'Pejalan Kaki 2'
#define DIST_LED_PEJALAN_KAKI_2 0.22

// Index of first LED in area 'Pejalan Kaki 3'
#define INDEX_LED_PEJALAN_KAKI_3 41
// Total number of LEDs used at area 'Pejalan Kaki 3'
#define NUM_LED_PEJALAN_KAKI_3 3
// Distance (in meter) from LEDs to measuring point in area 'Pejalan Kaki 3'
#define DIST_LED_PEJALAN_KAKI_3 0.22

// Index of first LED in area 'Pejalan Kaki 4'
#define INDEX_LED_PEJALAN_KAKI_4 44
// Total number of LEDs used at area 'Pejalan Kaki 4'
#define NUM_LED_PEJALAN_KAKI_4 3
// Distance (in meter) from LEDs to measuring point in area 'Pejalan Kaki 4'
#define DIST_LED_PEJALAN_KAKI_4 0.22

// Total number of LEDs used
#define NUM_LED NUM_LED_MEJA_DAN_KURSI_1+NUM_LED_MEJA_DAN_KURSI_2+NUM_LED_MEJA_DAN_KURSI_3+NUM_LED_PAMERAN_POSTER_1+NUM_LED_PAMERAN_POSTER_2+NUM_LED_PANGGUNG_SERBAGUNA+NUM_LED_PEJALAN_KAKI_1+NUM_LED_PEJALAN_KAKI_2+NUM_LED_PEJALAN_KAKI_3+NUM_LED_PEJALAN_KAKI_4

// Global variables
const char* ssid = "Wisma Thullabul Ilmi";
const char* password = "udruskitabersama";
const char* mqtt_server = "192.168.1.3";
bool start_measure = false;

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
    {0, 0, 0},
    {0, 0, 0}
};

// Object initializations
// Wifi
WiFiClient espClient;
PubSubClient client(espClient);
// LED strip
Adafruit_NeoPixel strip(NUM_LED, PIN_LED_STRIP);
// Area
area meja_dan_kursi_1(NUM_BH1750_MEJA_DAN_KURSI_1, NUM_LED_MEJA_DAN_KURSI_1, INDEX_LED_MEJA_DAN_KURSI_1, DIST_LED_MEJA_DAN_KURSI_1);
area meja_dan_kursi_2(NUM_BH1750_MEJA_DAN_KURSI_2, NUM_LED_MEJA_DAN_KURSI_2, INDEX_LED_MEJA_DAN_KURSI_2, DIST_LED_MEJA_DAN_KURSI_2);
area meja_dan_kursi_3(NUM_BH1750_MEJA_DAN_KURSI_3, NUM_LED_MEJA_DAN_KURSI_3, INDEX_LED_MEJA_DAN_KURSI_3, DIST_LED_MEJA_DAN_KURSI_3);
area pameran_poster_1(NUM_BH1750_PAMERAN_POSTER_1, NUM_LED_PAMERAN_POSTER_1, INDEX_LED_PAMERAN_POSTER_1, DIST_LED_PAMERAN_POSTER_1);
area pameran_poster_2(NUM_BH1750_PAMERAN_POSTER_2, NUM_LED_PAMERAN_POSTER_2, INDEX_LED_PAMERAN_POSTER_2, DIST_LED_PAMERAN_POSTER_2);
area panggung_serbaguna(NUM_BH1750_PANGGUNG_SERBAGUNA, NUM_LED_PANGGUNG_SERBAGUNA, INDEX_LED_PANGGUNG_SERBAGUNA, DIST_LED_PANGGUNG_SERBAGUNA);
area pejalan_kaki_1(NUM_BH1750_PEJALAN_KAKI_1, NUM_LED_PEJALAN_KAKI_1, INDEX_LED_PEJALAN_KAKI_1, DIST_LED_PEJALAN_KAKI_1);
area pejalan_kaki_2(NUM_BH1750_PEJALAN_KAKI_2, NUM_LED_PEJALAN_KAKI_2, INDEX_LED_PEJALAN_KAKI_2, DIST_LED_PEJALAN_KAKI_2);
area pejalan_kaki_3(NUM_BH1750_PEJALAN_KAKI_3, NUM_LED_PEJALAN_KAKI_3, INDEX_LED_PEJALAN_KAKI_3, DIST_LED_PEJALAN_KAKI_3);
area pejalan_kaki_4(NUM_BH1750_PEJALAN_KAKI_4, NUM_LED_PEJALAN_KAKI_4, INDEX_LED_PEJALAN_KAKI_4, DIST_LED_PEJALAN_KAKI_4);

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

    // Conditional statements to handle various topic
    // For area 'Meja dan Kursi 1'
    if (strcmp(topic, "set/lamp/meja-dan-kursi-1/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_1.brightness = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/meja-dan-kursi-1/red") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_1.new_red = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/meja-dan-kursi-1/green") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_1.new_green = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/meja-dan-kursi-1/blue") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_1.new_blue = atoi(number);
    }
    else if (strcmp(topic, "set/illuminance/meja-dan-kursi-1") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_1.avg_lux_target = atof(number);
    }

    // For area 'Meja dan Kursi 2'
    if (strcmp(topic, "set/lamp/meja-dan-kursi-2/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_2.brightness = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/meja-dan-kursi-2/red") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_2.new_red = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/meja-dan-kursi-2/green") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_2.new_green = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/meja-dan-kursi-2/blue") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_2.new_blue = atoi(number);
    }
    else if (strcmp(topic, "set/illuminance/meja-dan-kursi-2") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_2.avg_lux_target = atof(number);
    }

    // For area 'Meja dan Kursi 3'
    if (strcmp(topic, "set/lamp/meja-dan-kursi-3/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_3.brightness = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/meja-dan-kursi-3/red") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_3.new_red = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/meja-dan-kursi-3/green") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_3.new_green = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/meja-dan-kursi-3/blue") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_3.new_blue = atoi(number);
    }
    else if (strcmp(topic, "set/illuminance/meja-dan-kursi-3") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        meja_dan_kursi_3.avg_lux_target = atof(number);
    }

    // For area 'Pameran Poster 1'
    if (strcmp(topic, "set/lamp/pameran-poster-1/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pameran_poster_1.brightness = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pameran-poster-1/red") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pameran_poster_1.new_red = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pameran-poster-1/green") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pameran_poster_1.new_green = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pameran-poster-1/blue") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pameran_poster_1.new_blue = atoi(number);
    }
    else if (strcmp(topic, "set/illuminance/pameran-poster-1") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pameran_poster_1.avg_lux_target = atof(number);
    }

    // For area 'Pameran Poster 2'
    if (strcmp(topic, "set/lamp/pameran-poster-2/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pameran_poster_2.brightness = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pameran-poster-2/red") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pameran_poster_2.new_red = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pameran-poster-2/green") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pameran_poster_2.new_green = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pameran-poster-2/blue") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pameran_poster_2.new_blue = atoi(number);
    }
    else if (strcmp(topic, "set/illuminance/pameran-poster-2") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pameran_poster_2.avg_lux_target = atof(number);
    }

    // For area 'Panggung Serbaguna'
    if (strcmp(topic, "set/lamp/panggung-serbaguna/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        panggung_serbaguna.brightness = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/panggung-serbaguna/red") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        panggung_serbaguna.new_red = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/panggung-serbaguna/green") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        panggung_serbaguna.new_green = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/panggung-serbaguna/blue") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        panggung_serbaguna.new_blue = atoi(number);
    }
    else if (strcmp(topic, "set/illuminance/panggung-serbaguna") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        panggung_serbaguna.avg_lux_target = atof(number);
    }

    // For area 'Pejalan Kaki 1'
    if (strcmp(topic, "set/lamp/pejalan-kaki-1/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_1.brightness = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-1/red") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_1.new_red = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-1/green") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_1.new_green = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-1/blue") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_1.new_blue = atoi(number);
    }
    else if (strcmp(topic, "set/illuminance/pejalan-kaki-1") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_1.avg_lux_target = atof(number);
    }

    // For area 'Pejalan Kaki 2'
    if (strcmp(topic, "set/lamp/pejalan-kaki-2/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_2.brightness = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-2/red") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_2.new_red = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-2/green") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_2.new_green = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-2/blue") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_2.new_blue = atoi(number);
    }
    else if (strcmp(topic, "set/illuminance/pejalan-kaki-2") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_2.avg_lux_target = atof(number);
    }

    // For area 'Pejalan Kaki 3'
    if (strcmp(topic, "set/lamp/pejalan-kaki-3/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_3.brightness = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-3/red") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_3.new_red = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-3/green") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_3.new_green = atoi(number);
    }
    else if (strcmp(topic, "set/lamp/pejalan-kaki-3/blue") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_3.new_blue = atoi(number);
    }
    else if (strcmp(topic, "set/illuminance/pejalan-kaki-3") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_3.avg_lux_target = atof(number);
    }

    // For area 'Pejalan Kaki 4'
    if (strcmp(topic, "set/lamp/pejalan-kaki-4/brightness") == 0) {
        char number[length + 1];
        for (uint8_t i = 0; i < length; i++) {
            number[i] = (char)payload[i];
        }
        number[length] = '\0';
        pejalan_kaki_4.brightness = atoi(number);
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
    start_measure = true;
}

// Setup routines
void setup() {
    Serial.begin(9600);
    while (!Serial); // loop until serial monitor is ready

    // Set sensor addresses for each area
    meja_dan_kursi_1.set_address(meja_dan_kursi_1_address);
    meja_dan_kursi_2.set_address(meja_dan_kursi_2_address);
    meja_dan_kursi_3.set_address(meja_dan_kursi_3_address);
    pameran_poster_1.set_address(pameran_poster_1_address);
    pameran_poster_2.set_address(pameran_poster_2_address);
    panggung_serbaguna.set_address(panggung_serbaguna_address);
    pejalan_kaki_1.set_address(pejalan_kaki_1_address);
    pejalan_kaki_2.set_address(pejalan_kaki_2_address);
    pejalan_kaki_3.set_address(pejalan_kaki_3_address);
    pejalan_kaki_4.set_address(pejalan_kaki_4_address);
    
    // Start I2C communication bus
    Wire.begin(PIN_SDA, PIN_SCL);
    // Make sure all I2C mux channel are disabled at first start
    for (uint8_t i = 0; i < NUM_TCA; i++) tca_disable(TCA_ADDR_DEF + i);
    
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
    client.loop();
}