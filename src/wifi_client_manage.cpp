#include "wifi_client_manage.h"

WifiClientManage::WifiClientManage()
{
    m_client = new WiFiSSLClient();
}

WifiClientManage::~WifiClientManage()
{
    if (m_client)
    {
        delete m_client;
        m_client = NULL;
    }
}

void WifiClientManage::get()
{
    char server[] = "www.google.com"; // name address for Google (using DNS)

    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:

    if (m_client->connect(server, 443))
    {
        Serial.println("connected to server");
        // Make a HTTP request:
        m_client->println("GET / HTTP/1.1");
        m_client->println("Host: www.google.com");
        m_client->println("Connection: close");
        m_client->println();
        m_client->flush();
        m_client->peek();
    }

    readResponse();

    // if the server's disconnected, stop the client:
    if (!m_client->connected())
    {
        Serial.println();
        Serial.println("disconnecting from server.");
        m_client->stop();
    }
}

void WifiClientManage::readResponse()
{
    uint32_t received_data_num = 0;
    while (m_client->available())
    {
        /* actual data reception */
        char c = m_client->read();
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
