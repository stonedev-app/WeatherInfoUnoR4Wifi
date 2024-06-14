#include <Arduino.h>
#include <WiFiS3.h>

#include "arduino_secrets.h"
#include "wifi_connection_manage.h"

void WifiConnectionManage::init()
{
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true)
            ;
    }

    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    {
        Serial.println("Please upgrade the firmware");
    }

    int status = WL_IDLE_STATUS;

    // attempt to connect to WiFi network:
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(SECRET_SSID);
        // Connect to WPA/WPA2 network.
        status = WiFi.begin(SECRET_SSID, SECRET_PASS);

        // wait 10 seconds for connection:
        delay(10000);
    }

    printWifiStatus();
}

void WifiConnectionManage::printWifiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
