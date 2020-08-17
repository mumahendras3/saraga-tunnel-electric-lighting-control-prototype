/*
  Kode sumber untuk mikrokontroler LoLin NodeMCU V3
*/

// Include some needed libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <BH1750.h>
#include <Wire.h>

// Some pin definitions
#define PIN_LED D3
#define PIN_SDA D1
#define PIN_SCL D2

// Some location definitions
#define SENSOR_1 "bawah-skylight"
#define LAMPU_1 "dekat-skylight"
#define BATAS_1 "jalan"

// Update these with values suitable for your network.
const char* ssid = "RN8Pro";
const char* password = "Mahendras9708";
const char* mqtt_server = "192.168.43.188";

WiFiClient espClient;
PubSubClient client(espClient);
BH1750 sensorCahaya;
float batas = 43; // untuk iluminance threshold
float iluminansi; // untuk sensor readings

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
  // Untuk debugging
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  //for (int i = 0; i < length; i++) {
  //  Serial.print((char)payload[i]);
  //}
  //Serial.println();

  // Jika menerima pesan dari topik set/batas/pameran, simpan sebagai batas iluminansi baru
  if (strcmp(topic,"set/batas/"BATAS_1) == 0) {
    // Mengubah pesan menjadi string terlebih dahulu
    char str[length+1];
    for (int i = 0; i < length; i++) {
      str[i] = (char)payload[i];
    }
    str[length] = '\0';
    // Konversi string pesan menjadi angka float sebagai nilai batas baru
    batas = atof(str);
    // Untuk debugging
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
      client.subscribe("set/batas/"BATAS_1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  Wire.begin(PIN_SDA, PIN_SCL);
  sensorCahaya.begin(BH1750::ONE_TIME_HIGH_RES_MODE, 0x23);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  iluminansi = sensorCahaya.readLightLevel();
//  Serial.print("Iluminansi: ");
//  Serial.print(iluminansi);
//  Serial.println(" lx");
  if (iluminansi > batas) {
    digitalWrite(PIN_LED, LOW);
    client.publish("status/lampu/"LAMPU_1, "OFF");
  }
  else {
    digitalWrite(PIN_LED, HIGH);
    client.publish("status/lampu/"LAMPU_1, "ON");
  }
  client.publish("status/sensor/"SENSOR_1, String(iluminansi).c_str());
  delay(3000);
}