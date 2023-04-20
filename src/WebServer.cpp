/**
 * @file WebServer.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-04-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "WebServer.h"

WebServer::WebServer(int port) : server(port) {}

static void bytesToHex(char *bytes, char *hex, int len);
static int hexToBytes(char *hex, uint8_t *bytes, size_t len);

void WebServer::start()
{
    server.on("/v1/serial/begin", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("baud", true))
        { 
            String baud = request->getParam("baud", true)->value();
            Serial.begin(baud.toInt());

            request->send(200, "text/plain", "OK");
        }
        else
        {
            request->send(400, "text/plain", "ERROR");
        } 
    });

    server.on("/v1/serial/print", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("data", true))
        { 
            String data = request->getParam("data", true)->value();
            Serial.print(data);

            request->send(200, "text/plain", "OK");
        }
        else
        {
            request->send(400, "text/plain", "ERROR");
        }
    });

    server.on("/v1/serial/write", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("data", true))
        {
            String data = request->getParam("data", true)->value();
            size_t len = request->hasParam("len", true) 
                ? request->getParam("len")->value().toInt()
                : data.length();

            uint8_t bytes[16] = {0};
            char hex[32] = {0};
            strcpy(hex, data.c_str());

            len = hexToBytes(hex, bytes, len);
            Serial.write(bytes, len);

            request->send(200, "text/plain", "OK");
        }
        else
        {
            request->send(400, "text/plain", "ERROR");
        }
    });

    server.on("/v1/serial/readString", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (Serial.available())
        { 
            String data = Serial.readString();
            request->send(200, "text/plain", data);
        }
        else
        {
            request->send(404, "text/plain", "ERROR");
        }
    });

    server.on("/v1/serial/readBytes", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (Serial.available())
        { 
            int len = request->hasParam("len") 
                ? request->getParam("len")->value().toInt()
                : Serial.available();

            char bytes[len];
            char data[len * 2];
            Serial.readBytes(bytes, len);
            bytesToHex(bytes, data, len);
            request->send(200, "text/plain", data);
        }
        else
        {
            request->send(404, "text/plain", "ERROR");
        }
    });

    server.begin();
}

static void bytesToHex(char *bytes, char *hex, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (bytes[i] <= 0x0F)
        {
            sprintf(&hex[i << 1], "0%X", bytes[i]);
        }
        else
        {
            sprintf(&hex[i << 1], "%X", bytes[i]);
        }
    }
}

static int hexToBytes(char *hex, uint8_t *bytes, size_t len)
{
    if (bytes == NULL)
        return (-1);

    if ((len % 2) != 0)
        return (-1);

    size_t idx = 0;
    while (idx < len)
    {
        char c = hex[idx];
        int val = 0;
        if (c >= '0' && c <= '9')
            val = c - '0';
        else if (c >= 'a' && c <= 'f')
            val = 10 + (c - 'a');
        else if (c >= 'A' && c <= 'F')
            val = 10 + (c - 'A');
        else
        {
            return (-1);
        }

        bytes[idx / 2] += val << (((idx + 1) % 2) * 4);
        idx++;
    }

    return (len / 2);
}
