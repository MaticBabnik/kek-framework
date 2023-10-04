#pragma once
#include "Arduino.h"
#include "KekFramework.hh"

class RgbLedModule : public Kek::Module
{
  public:
    virtual const char *getModuleName();
    virtual int read(const char *key, JsonDocument &data);
    virtual int write(const char *key, JsonDocument &data,
                      JsonDocument &response);

    RgbLedModule(int pr, int pg, int pb);

  protected:
    int pin_r, pin_g, pin_b;
    int state_r, state_g, state_b;
};