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

void WifiClientManage::get(const char *rootCA, const char *host, const char *path)
{
    setCACert(rootCA);
    writeRequest(host, path);
    readResponse();
    stop();
}

void WifiClientManage::setCACert(const char *rootCA)
{
    m_client->setCACert(rootCA);
}

void WifiClientManage::writeRequest(const char *host, const char *path)
{
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:

    if (m_client->connect(host, 443))
    {
        Serial.println("connected to server\n");
        // Make a HTTP request:
        m_client->println("GET https://" + String(host) + String(path) + " HTTP/1.1");
        m_client->println("Host: " + String(host));
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
