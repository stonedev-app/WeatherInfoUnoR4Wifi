#pragma once

#include <Arduino.h>
#include <WiFiS3.h>

#include "arduino_secrets.h"

class WifiConnectionManage
{
public:
    void init();

private:
    void printWifiStatus();
};