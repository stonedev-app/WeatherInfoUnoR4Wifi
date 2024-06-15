#pragma once

#include <Arduino.h>
#include <WiFiSSLClient.h>

#include "arduino_secrets.h"

class WifiClientManage
{
public:
    WifiClientManage();
    ~WifiClientManage();
    void get();
private:
    WiFiSSLClient* m_client;
    void setCACert();
    void writeRequest();
    void readResponse();
    void stop();
};