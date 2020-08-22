/*
  Source code for LoLin NodeMCU V3 Microcontroller
*/

// Include some needed libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <hp_BH1750.h>
#include <Wire.h>

// Pin definitions
#define PIN_LED D7
#define PIN_SDA D3
#define PIN_SCL D2

// I2C device address definitions
#define TCA_ADDR_DEF 0x70

// Total number of various things definitions
#define NUM_TCA 2
#define NUM_TCA_CH 8
#define NUM_BH1750 25

// Location definitions
#define SENSOR_1 "bawah-skylight"
#define LAMPU_1 "dekat-skylight"
#define BATAS_1 "jalan"

// Object initializations
// Wifi
WiFiClient espClient;
PubSubClient client(espClient);
// Sensors
// Numbering format: [I2C_Mux_Number][I2C_Mux_Channel][Low/High_Addr_Pin]
hp_BH1750 sensors[NUM_TCA][NUM_TCA_CH][2];

// Global variables
const char* ssid = "Wisma Thullabul Ilmi";
const char* password = "udruskitabersama";
const char* mqtt_server = "192.168.43.188";
bool start_measurement;
bool processed_all;
bool processed[NUM_TCA][NUM_TCA_CH][2];
bool skip_index[NUM_TCA][NUM_TCA_CH][2] = {false};
float lux[NUM_TCA][NUM_TCA_CH][2]; // For light sensor readings
float batas = 43; // For illuminance threshold

// Function definitions
void sensors_init() {
  uint8_t count = 0;
  bool sensor_is_found;
  for (uint8_t i = 0; count < NUM_BH1750 && i < NUM_TCA; i++) {
    for (uint8_t j = 0; count < NUM_BH1750 && j < NUM_TCA_CH; j++) {
      tca_select(TCA_ADDR_DEF + i, j);
      for (uint8_t k = 0; count < NUM_BH1750 && k < 2; k++) {
        if (k == 0) {
          sensor_is_found = sensors[i][j][k].begin(BH1750_TO_GROUND);
        }
        else {
          sensor_is_found = sensors[i][j][k].begin(BH1750_TO_VCC);
        }
        if (sensor_is_found) {
          sensors[i][j][k].calibrateTiming();
          count++;
        }
        // Print error if no sensor is found and set the skip_index flag to "true"
        else {
          Serial.println("Couldn't find sensor" + i + j + k);
          skip_index[i][j][k] = true;
        }
      }
    }
    tca_disable(TCA_ADDR_DEF + i);
  }
}

void sensors_start() {
  processed_all = false;
  start_measurement = false;
  uint8_t count = 0;
  for (uint8_t i = 0; count < NUM_BH1750 && i < NUM_TCA; i++) {
    for (uint8_t j = 0; count < NUM_BH1750 && j < NUM_TCA_CH; j++) {
      tca_select(TCA_ADDR_DEF + i, j);
      for (uint8_t k = 0; count < NUM_BH1750 && k < 2; k++) {
        if (!skip_index[i][j][k]) {
          sensors[i][j][k].adjustSettings(90, true);
          sensors[i][j][k].start();
          processed[i][j][k] = false;
          count++;
        }
      }
    }
    tca_disable(TCA_ADDR_DEF + i);
  }
}

bool sensors_read() {
  uint8_t count = 0;
  bool _processed_all = true;
  for (uint8_t i = 0; count < NUM_BH1750 && i < NUM_TCA; i++) {
    for (uint8_t j = 0; count < NUM_BH1750 && j < NUM_TCA_CH; j++) {
      tca_select(TCA_ADDR_DEF + i, j);
      for (uint8_t k = 0; count < NUM_BH1750 && k < 2; k++) {
        if (!skip_index[i][j][k]) {
          if (sensors[i][j][k].hasValue() && !processed[i][j][k]) {
            lux[i][j][k] = sensors[i][j][k].getLux();
            processed[i][j][k] = true;
          }
          _processed_all = _processed_all && processed[i][j][k];
          count++;
        }
      }
    }
    tca_disable(TCA_ADDR_DEF + i);
  }
  return _processed_all;
}

float lux_avg(const uint8_t index[][3], uint8_t num_sensors) {
  uint8_t n;
  // Return -1 if one of the sensors with the given index hasn't finished measuring yet
  for (n = 0; n < num_sensors; n++) {
    if (!processed[index[n][0]][index[n][1]][index[n][2]]) return -1;
  }
  // Calculating the average lux
  float sum = 0;
  for (n = 0; n < num_sensors; n++) {
    sum += lux[index[n][0]][index[n][1]][index[n][2]];
  }
  return sum / num_sensors;
}

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

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // For debugging
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  //for (int i = 0; i < length; i++) {
  //  Serial.print((char)payload[i]);
  //}
  //Serial.println();

  // Jika menerima pesan dari topik set/batas/pameran, simpan sebagai batas iluminansi baru
  if (strcmp(topic, "set/batas/" BATAS_1) == 0) {
    // Mengubah pesan menjadi string terlebih dahulu
    char str[length+1];
    for (unsigned int i = 0; i < length; i++) {
      str[i] = (char)payload[i];
    }
    str[length] = '\0';
    // Konversi string pesan menjadi angka float sebagai nilai batas baru
    batas = atof(str);
    // For debugging
    //Serial.print("batas = ");
    //Serial.print(batas);
    //Serial.println(" lx");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "NodeMcuV3";
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // subscribe ke channel batas
      client.subscribe("set/batas/" BATAS_1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Setup routines
void setup() {
  Serial.begin(9600);
  while (!Serial); // loop until serial monitor is ready
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  Wire.begin(PIN_SDA, PIN_SCL);
  // Make sure all I2C mux channel are disabled at first start
  for (uint8_t i = 0; i < NUM_TCA; i++) tca_disable(TCA_ADDR_DEF + i);
  // Initialize all sensors
  sensors_init();
  // Start first time measurement
  sensors_start();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// Main loop
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (start_measurement) sensors_start();
  if (!processed_all) processed_all = sensors_read();
  // Serial.print("Iluminansi: ");
  // Serial.print(iluminansi);
  // Serial.println(" lx");
  if (iluminansi > batas) {
    digitalWrite(PIN_LED, LOW);
    client.publish("status/lampu/" LAMPU_1, "OFF");
  }
  else {
    digitalWrite(PIN_LED, HIGH);
    client.publish("status/lampu/" LAMPU_1, "ON");
  }
  client.publish("status/sensor/" SENSOR_1, String(iluminansi).c_str());
  delay(3000);
}