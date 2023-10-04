#include "DhtModule.hh"

const char *DhtModule::getModuleName()
{
    return "Dht";
};

DhtModule::DhtModule(int pin, int type)
{
    this->dht = new DHT(pin, type);
    this->dht->begin();

    this->dht->readTemperature();
    this->dht->readHumidity();
    this->dht->computeHeatIndex(false);
}

int DhtModule::read(const char *key, JsonDocument &data)
{
    auto temp = this->dht->readTemperature();
    auto hum = this->dht->readHumidity();
    auto heat_index = this->dht->computeHeatIndex(false);

    if (isnan(temp) || isnan(hum) || isnan(heat_index))
    {
        data[key] = "sensor error";
        return 1;
    }

    data[key]["temperature"] = temp;
    data[key]["humidity"] = hum;
    data[key]["heat_index"] = heat_index;

    return 0;
}

int DhtModule::write(const char *key, JsonDocument &data,
                     JsonDocument &response)
{
    response[key] = "Read only";
    return 2;
}
