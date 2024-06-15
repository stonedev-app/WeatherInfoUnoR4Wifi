#pragma once

#include <Arduino.h>
#include <WiFiSSLClient.h>

class WifiClientManage
{
public:
    WifiClientManage();
    ~WifiClientManage();
    void get();
private:
    WiFiSSLClient* m_client;
    void writeRequest();
    void readResponse();
    void stop();
};