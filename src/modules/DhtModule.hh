#pragma once
#include "KekFramework.hh"
#include "Adafruit_Sensor.h"
#include "DHT.h"

class DhtModule : public Kek::Module
{
public:
    virtual const char *getModuleName();
    virtual int read(const char *key, JsonDocument &data);
    virtual int write(const char *key, JsonDocument &data, JsonDocument &response);

    DhtModule(int pin, int type = 22);

protected:
    DHT *dht;
};