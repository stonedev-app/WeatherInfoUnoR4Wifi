#include "wifi_client_manage.h"

#define HEADER_MAX_SIZE 512

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
    int contentLength = 0;
    setCACert(rootCA);
    writeRequest(host, path);
    readResponseHeader(contentLength);
    if (contentLength > 0)
    {
        readResponseBody(contentLength);
    }
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
    }
}

void WifiClientManage::readResponseHeader(int &contentLength)
{
    // Read the header one line at a time
    while (m_client->connected())
    {
        // One line of header
        char headerLine[HEADER_MAX_SIZE] = {0};
        // content length
        int conLen = 0;

        // Read until the newline character to obtain one line of the header
        size_t hrSize = m_client->readBytesUntil('\n', headerLine, sizeof(headerLine));
        // timeout
        if (hrSize == 0)
        {
            Serial.println("\nheader reading timeout");
            break;
        }

        // Remove the trailing \r.
        char *endCrChar = strstr(headerLine, "\r");
        if (endCrChar != NULL)
        {
            *endCrChar = '\0';
        }

        // Output one header line to the serial log
        Serial.println(headerLine);

        // Get Content length
        char *startPos = strstr(headerLine, "Content-Length: ");
        // if 'Content-Length: ' is included in headerLine
        if (startPos != NULL)
        {
            // move pointer Next to 'Content-Length: '
            startPos += sizeof("Content-Length: ") - 1;
            // conten length
            char conLenStr[HEADER_MAX_SIZE] = {0};
            strcpy(conLenStr, startPos);
            // if conLenStr is enable to cast Number
            if (validateNumber(conLenStr))
            {
                contentLength = atoi(conLenStr);
            }
        }

        // If it's a blank line
        // In the case of a blank line,
        // the string length will be 0 because \r has been removed beforehand
        if (strlen(headerLine) == 0)
        {
            Serial.println("header received\n");
            break;
        }
    }
}

void WifiClientManage::readResponseBody(int contentLength)
{
    if (m_client->connected())
    {
        char bodyBuf[contentLength + 1] = {0};
        size_t bodySize = m_client->readBytes(bodyBuf, sizeof(bodyBuf) - 1);
        if (bodySize == contentLength)
        {
            /* print data to serial port */
            Serial.println(bodyBuf);
            Serial.println("\nbody received");
        }
        // timeout
        else
        {
            Serial.println("body reading timeout");
            return;
        }
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
        Serial.println("disconnecting from server.");
        m_client->stop();
    }
}

bool WifiClientManage::validateNumber(const char *input)
{
    int length = strlen(input);
    for (int i = 0; i < length; i++)
    {
        if (!isdigit(input[i]))
        { // if include not Number
            return false;
        }
    }
    return true;
}
