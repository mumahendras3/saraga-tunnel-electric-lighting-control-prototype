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
#define TCA_ADDR0 0x70
#define TCA_ADDR1 0x71

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
float batas = 43; // untuk iluminance threshold
float iluminansi; // untuk sensor readings

// Function definitions
void sensorsInit() {
  uint8_t count = 0;
  bool sensorIsFound;
  for (uint8_t i = 0; count < NUM_BH1750 && i < NUM_TCA; i++) {
    for (uint8_t j = 0; count < NUM_BH1750 && j < NUM_TCA_CH; j++) {
      for (uint8_t k = 0; count < NUM_BH1750 && k < 2; k++) {
        if (k == 0) {
          sensorIsFound = sensors[i][j][k].begin(BH1750_TO_GROUND);
        }
        else {
          sensorIsFound = sensors[i][j][k].begin(BH1750_TO_VCC);
        }
        // Print error and abort the program if no sensor is found
        if (!sensorIsFound) {
          Serial.println("Couldn't find sensor" + i + j + k);
          Serial.flush();
          abort();
        }
        sensors[i][j][k].calibrateTiming();
        count++;
      }
    }
  }
}

void sensorsStart() {
  uint8_t count = 0;
  for (uint8_t i = 0; count < NUM_BH1750 && i < NUM_TCA; i++) {
    for (uint8_t j = 0; count < NUM_BH1750 && j < NUM_TCA_CH; j++) {
      for (uint8_t k = 0; count < NUM_BH1750 && k < 2; k++) {
        sensors[i][j][k].start();
        count++;
      }
    }
  }
}

void tcaSelect(uint8_t address, uint8_t channel) {
  Wire.beginTransmission(address);
  Wire.write(1 << channel);
  Wire.endTransmission();  
}

void tcaDisable(uint8_t address) {
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
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  Wire.begin(PIN_SDA, PIN_SCL);
  Serial.begin(9600);
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
//  Serial.print("Iluminansi: ");
//  Serial.print(iluminansi);
//  Serial.println(" lx");
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