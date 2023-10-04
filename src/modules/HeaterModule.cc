#include "HeaterModule.hh"
#include "util/hysteresis.cc"

const char *HeaterModule::getModuleName()
{
    return "Heater";
}

int HeaterModule::read(const char *key, JsonDocument &data)
{
    data[key]["enabled"] = this->enabled;

    data[key]["temperature"]["cur"] = this->temp;
    data[key]["temperature"]["set"] = this->setTemp;
    data[key]["temperature"]["on"] = this->heaterOn;

    data[key]["humidity"]["cur"] = this->humidity;
    data[key]["humidity"]["set"] = this->setHumidity;
    data[key]["humidity"]["on"] = this->humidifierOn;
    return 0;
}

int HeaterModule::write(const char *key, JsonDocument &data,
                        JsonDocument &response)
{
    Kek::log("Write");
    if (!data[key]["enabled"].is<bool>() ||
        !data[key]["setTemperature"].is<float>() ||
        !data[key]["setHumidity"].is<float>())
    {
        Kek::log("Fail");
        response[key] = "Invalid JSON";
        return 1;
    }

    this->enabled = data[key]["enabled"].as<bool>();
    this->setTemp = data[key]["setTemperature"].as<float>();
    this->setHumidity = data[key]["setHumidity"].as<float>();

    Kek::log("write request: %u %f %f", this->enabled, this->setTemp,
             this->setHumidity);

    this->tc.setTarget(this->setTemp);
    this->hc.setTarget(this->setHumidity);

    // write out all data if everything is fine
    this->read(key, response);
    return 0;
}

void HeaterModule::heaterTick()
{
    this->temp = dht.readTemperature();
    this->humidity = dht.readHumidity();

    if (!this->enabled || isnan(this->temp) || isnan(this->humidity))
    {
        // disable heating if DHT fails
        this->heaterOn = false;
        this->humidifierOn = false;
    }
    else
    {
        this->heaterOn = this->tc.act(this->temp);
        this->humidifierOn = this->hc.act(this->humidity);
    }

    digitalWrite(this->heaterPin, this->heaterOn);
    digitalWrite(this->humidifierPin, this->humidifierOn);
}

void HeaterModule::setup(Kek::Scheduler &scheduler)
{
    scheduler.loop(10000, [this]() {
        this->heaterTick();
        return true;
    });
}

HeaterModule::HeaterModule(int sensorPin, int heaterPin, int humidifierPin)
    : dht(sensorPin, DHT22), tc(2.0f, 21.0f), hc(10.0f, 40.0f),
      heaterPin(heaterPin), humidifierPin(humidifierPin)
{
    this->dht.begin();
    this->setTemp = 21;
    this->setHumidity = 40;

    this->enabled = false;
    this->heaterOn = this->humidifierOn = false;

    pinMode(this->heaterPin, OUTPUT);
    pinMode(this->humidifierPin, OUTPUT);
    digitalWrite(this->heaterPin, 0);
    digitalWrite(this->humidifierPin, 0);

    this->heaterTick();
}
