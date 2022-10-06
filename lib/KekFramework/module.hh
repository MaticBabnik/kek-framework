#pragma once
#include <ArduinoJson.h>

namespace Kek
{
    class Module
    {
    public:
        virtual const char *getModuleName();
        virtual int read(const char *key, JsonDocument &data);
        virtual int write(const char *key, JsonDocument &data, JsonDocument &response);
    };
}