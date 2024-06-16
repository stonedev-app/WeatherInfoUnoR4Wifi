#pragma once

#include <Arduino.h>
#include <WiFiSSLClient.h>

class WifiClientManage
{
public:
    WifiClientManage();
    ~WifiClientManage();
    void get(const char *rootCA, const char *host, const char *path);

private:
    WiFiSSLClient *m_client;
    void setCACert(const char *rootCA);
    void writeRequest(const char *host, const char *path);
    void readResponseHeader(int &contentLength);
    void readResponseBody(int contentLength);
    void stop();
    bool validateNumber(const char *input);
};