#include "PinModule.hh"

const char *PinModule::getModuleName()
{
    return "Pin";
};

int PinModule::read(const char *key, JsonDocument &data)
{
    data[key]["state"] = this->state;
    return 0;
}

int PinModule::write(const char *key, JsonDocument &data,
                     JsonDocument &response)
{
    if (!data[key]["state"].is<bool>())
    {
        response[key] = "Invalid JSON";
        return 1;
    }
    this->state = data[key]["state"];
    digitalWrite(this->pin, this->state);
    response[key]["state"] = this->state;
    return 0;
}

PinModule::PinModule(int pin, int mode, bool initialState)
{
    this->pin = pin;
    this->state = initialState;
    pinMode(this->pin, mode);
    digitalWrite(this->pin, this->state);
}