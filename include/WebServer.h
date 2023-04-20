/**
 * @file WebServer.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __WEBSERVER__H__
#define __WEBSERVER__H__

#include "ESPAsyncWebServer.h"

class WebServer
{
private:
    AsyncWebServer server;
public:
    WebServer(int port);
    void start();
};

#endif  //!__WEBSERVER__H__