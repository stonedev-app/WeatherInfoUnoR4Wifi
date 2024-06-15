#include <Arduino.h>
#include <WiFiSSLClient.h>

#include "arduino_secrets.h"
#include "wifi_connection_manage.h"
#include "wifi_client_manage.h"

WifiConnectionManage wifiConManage;
WifiClientManage client;

void setup()
{
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // WiFi connection
  wifiConManage.init();

  // GET request
  client.get(ROOT_CA, "www.jma.go.jp", "/bosai/forecast/data/overview_forecast/130000.json");
}

void loop()
{
}
