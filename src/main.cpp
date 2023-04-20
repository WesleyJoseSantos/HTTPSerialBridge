/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "WebServer.h"

#define CONFIG_WIFI_SSID "HTTP Serial Bridge"
#define CONFIG_HTTP_PORT 80

WebServer server(CONFIG_HTTP_PORT);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74400);
  WiFi.softAP(CONFIG_WIFI_SSID);
  server.start();
}

void loop() {
  // put your main code here, to run repeatedly:
}