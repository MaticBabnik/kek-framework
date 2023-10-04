#include "RgbLedModule.hh"

const char *RgbLedModule::getModuleName()
{
    return "RgbLed";
};

int RgbLedModule::read(const char *key, JsonDocument &data)
{
    data[key].createNestedArray("state");
    data[key]["state"].add(this->state_r);
    data[key]["state"].add(this->state_g);
    data[key]["state"].add(this->state_b);
    return 0;
}

int RgbLedModule::write(const char *key, JsonDocument &data,
                        JsonDocument &response)
{
    if (!data[key]["state"].is<JsonArray>() || data[key]["state"].size() != 3)
    {
        response[key] = "Invalid JSON";
        return 1;
    }

    this->state_r = data[key]["state"][0];
    this->state_g = data[key]["state"][1];
    this->state_b = data[key]["state"][2];

    analogWrite(this->pin_r, this->state_r);
    analogWrite(this->pin_g, this->state_g);
    analogWrite(this->pin_b, this->state_b);

    response[key].createNestedArray("state");
    response[key]["state"].add(this->state_r);
    response[key]["state"].add(this->state_g);
    response[key]["state"].add(this->state_b);

    return 0;
}

RgbLedModule::RgbLedModule(int pr, int pg, int pb)
{
    this->state_r = this->state_g = this->state_b = 0;
    this->pin_r = pr;
    this->pin_g = pg;
    this->pin_b = pb;

    analogWrite(this->pin_r, this->state_r);
    analogWrite(this->pin_g, this->state_g);
    analogWrite(this->pin_b, this->state_b);
}