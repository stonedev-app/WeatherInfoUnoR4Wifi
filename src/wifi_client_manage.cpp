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
    setCACert();
    writeRequest();
    readResponse();
    stop();
}

void WifiClientManage::setCACert()
{
    m_client->setCACert(ROOT_CA);
}

void WifiClientManage::writeRequest()
{
    char server[] = "www.jma.go.jp"; // name address for Google (using DNS)

    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:

    if (m_client->connect(server, 443))
    {
        Serial.println("connected to server\n");
        // Make a HTTP request:
        m_client->println("GET https://www.jma.go.jp/bosai/forecast/data/overview_forecast/130000.json HTTP/1.1");
        m_client->println("Host: www.jma.go.jp");
        m_client->println("Connection: close");
        m_client->println();
        // Wait for the entire request content to be written, just in case
        m_client->flush();
        // Wait for some response data to return using peek
        m_client->peek();
    }
}

void WifiClientManage::readResponse()
{
    while (m_client->available())
    {
        /* actual data reception */
        char c = m_client->read();
        /* print data to serial port */
        Serial.print(c);
    }
}

void WifiClientManage::stop()
{
    // if the server's disconnected, stop the client:
    if (!m_client->connected())
    {
        Serial.println();
        Serial.println("\ndisconnecting from server.");
        m_client->stop();
    }
}
