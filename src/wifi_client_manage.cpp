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
        char *body = new char[contentLength + 1]();
        readResponseBody(contentLength, body);
        parseJson(body);
        delete[] body;
    }
    stop();
}

void WifiClientManage::setCACert(const char *rootCA)
{
    m_client->setCACert(rootCA);
}

void WifiClientManage::writeRequest(const char *host, const char *path)
{
    Serial.println();
    Serial.println("Starting connection to server...");
    // if you get a connection, report back via serial:

    if (m_client->connect(host, 443))
    {
        Serial.println("connected to server");
        Serial.println();
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
            Serial.println();
            Serial.println("header reading timeout");
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
            Serial.println("header received");
            Serial.println();
            break;
        }
    }
}

void WifiClientManage::readResponseBody(int contentLength, char *body)
{
    if (m_client->connected())
    {
        size_t bodySize = m_client->readBytes(body, contentLength);
        if (bodySize == contentLength)
        {
            /* print data to serial port */
            Serial.println(body);
            Serial.println();
            Serial.println("body received");
        }
        // timeout
        else
        {
            Serial.println("body reading timeout");
            return;
        }
    }
}

void WifiClientManage::parseJson(const char *json)
{
    Serial.println();
    Serial.println("== json parse ==");

    JSONVar myObject = JSON.parse(json);

    // JSON.typeof(jsonVar) can be used to get the type of the variable
    if (JSON.typeof(myObject) == "undefined")
    {
        Serial.println("Parsing failed!");
        return;
    }

    Serial.println();
    Serial.print("JSON.typeof = ");
    Serial.println(JSON.typeof(myObject)); // prints: object

    // JSONVars can be printed using print or println
    Serial.println();
    Serial.println(myObject);

    if (myObject.hasOwnProperty("text"))
    {
        Serial.println();
        Serial.print("[\"text\"] = ");
        Serial.println((const char *)myObject["text"]);
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
