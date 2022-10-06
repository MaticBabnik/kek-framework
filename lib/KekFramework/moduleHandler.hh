#pragma once

#include <map>
#include <type_traits>

#include <Arduino.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

#include "module.hh"
#include "logger.hh"

namespace Kek
{
    class CstrCmp
    {
    public:
        bool operator()(const char *a, const char *b) const;
    };

    class ModuleHandler
    {
        // no destructor since it would only get called once the ESP reboots
    public:
        ModuleHandler(AsyncWebServer *aws);
        void AddDevice(const char *name, Module *mod);
        void setAuth(const char *user, const char *pass);
    protected:
        char *user, *pass;
        std::map<const char *, Module *, CstrCmp> *modules;
        void handleReadRequest(AsyncWebServerRequest *req);
        void handleReadRequestWithBody(AsyncWebServerRequest *req, uint8_t *data, size_t len);
        void handleWriteRequestWithBody(AsyncWebServerRequest *req, uint8_t *data, size_t len);
        void handleListRequest(AsyncWebServerRequest *req);
    };
}