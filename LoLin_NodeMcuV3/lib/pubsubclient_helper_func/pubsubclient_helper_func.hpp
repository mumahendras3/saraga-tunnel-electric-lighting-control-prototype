#ifndef PUBSUBCLIENT_HELPER_FUNC_H
#define PUBSUBCLIENT_HELPER_FUNC_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void setup_wifi(const char* ssid, const char* password);
void reconnect(PubSubClient * client);
double parse_float(byte* payload, unsigned int length);
int parse_int(byte* payload, unsigned int length);

#endif