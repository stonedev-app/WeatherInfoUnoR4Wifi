#pragma once

#include <Arduino.h>
#include <WiFiS3.h>

class WifiConnectionManage
{
public:
    void init(const char *ssid, const char *pass);

private:
    void printWifiStatus();
};