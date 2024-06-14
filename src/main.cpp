#include <Arduino.h>
#include <WiFiSSLClient.h>

#include "wifi_connection_manage.h"

char server[] = "www.google.com"; // name address for Google (using DNS)

// WiFi management class
WifiConnectionManage wifiConManage;

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiSSLClient client;

void setup()
{
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Executed WiFi connection
  wifiConManage.init();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:

  if (client.connect(server, 443))
  {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET / HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }
}

void read_response()
{
  uint32_t received_data_num = 0;
  while (client.available())
  {
    /* actual data reception */
    char c = client.read();
    /* print data to serial port */
    Serial.print(c);
    /* wrap data to 80 columns*/
    received_data_num++;
    if (received_data_num % 80 == 0)
    {
      Serial.println();
    }
  }
}

void loop()
{
  read_response();

  // if the server's disconnected, stop the client:
  if (!client.connected())
  {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true)
      ;
  }
}
