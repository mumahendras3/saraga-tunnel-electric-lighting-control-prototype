#include "pubsubclient_helper_func.hpp"

void setup_wifi(const char* ssid, const char* password) {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
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

void reconnect(PubSubClient * client) {
    // Loop until we're reconnected
    while (!client->connected()) {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "NodeMcuV3";
        // Attempt to connect
        if (client->connect(clientId.c_str())) {
            Serial.println("connected");
            // subscribe to all topic under set/
            client->subscribe("set/#");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client->state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

double parse_float(byte* payload, unsigned int length) {
    char number[length + 1];
    for (uint8_t i = 0; i < length; i++) {
        number[i] = (char)payload[i];
    }
    number[length] = '\0';
    return atof(number);
}

int parse_int(byte* payload, unsigned int length) {
    char number[length + 1];
    for (uint8_t i = 0; i < length; i++) {
        number[i] = (char)payload[i];
    }
    number[length] = '\0';
    return atoi(number);
}