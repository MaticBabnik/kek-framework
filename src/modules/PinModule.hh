#pragma once
#include "KekFramework.hh"

class PinModule : public Kek::Module
{
public:
    virtual const char *getModuleName();
    virtual int read(const char *key, JsonDocument &data);
    virtual int write(const char *key, JsonDocument &data, JsonDocument &response);

    PinModule(int pin, int mode = OUTPUT, bool initialState = 0);

protected:
    int pin;
    bool state;
};