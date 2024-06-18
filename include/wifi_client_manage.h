#pragma once

#include <Arduino.h>
#include <WiFiSSLClient.h>

#include <Arduino_JSON.h>

class WifiClientManage
{
public:
    WifiClientManage();
    ~WifiClientManage();
    void get(const char *rootCA, const char *host, const char *path, JSONVar &response);

private:
    WiFiSSLClient *m_client;
    void setCACert(const char *rootCA);
    void writeRequest(const char *host, const char *path);
    void readResponseHeader(int &contentLength);
    void readResponseBody(int contentLength, char *body);
    void parseJson(const char *json, JSONVar &response);
    void stop();
    bool validateNumber(const char *input);
};