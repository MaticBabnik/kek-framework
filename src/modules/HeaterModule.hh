#pragma once

#include "Adafruit_Sensor.h"
#include "DHT.h"
#include "KekFramework.hh"
#include "util/hysteresis.hh"

class HeaterModule final : public Kek::Module
{
  public:
    virtual const char *getModuleName();
    virtual int read(const char *key, JsonDocument &data);
    virtual int write(const char *key, JsonDocument &data,
                      JsonDocument &response);
    void heaterTick();

    virtual void setup(Kek::Scheduler &scheduler) override;
    HeaterModule(int sensorPin, int heaterPin, int humidifierPin);

  protected:
    bool enabled;
    bool heaterOn, humidifierOn;
    float temp, humidity;
    float setHumidity, setTemp;

    DHT dht;
    HysteresisController<float> tc, hc;

    int heaterPin;
    int humidifierPin;
};