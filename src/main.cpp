#include <Arduino.h>
#include <WiFiSSLClient.h>

#include "arduino_secrets.h"
#include "wifi_connection_manage.h"
#include "wifi_client_manage.h"

// URL
// Host for the Japan Meteorological Agency
#define JMA_HOST "www.jma.go.jp"
// Weather forecast summary for Tokyo
#define WFS_TOKYO "/bosai/forecast/data/overview_forecast/130000.json"

// WiFi connection management class
WifiConnectionManage wifiConManage;
// Client management class
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
  client.get(JMA_ROOT_CA, JMA_HOST, WFS_TOKYO);
}

void loop()
{
}
