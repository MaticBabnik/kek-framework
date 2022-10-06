#include <module.hh>

const char *Kek::Module::getModuleName()
{
    return "Dummy";
}

int Kek::Module::read(const char *key, JsonDocument &data)
{
    data[key]["status"] = "ok";
    return 0;
}

int Kek::Module::write(const char *key, JsonDocument &data, JsonDocument &response)
{
    response[key]["status"] = "ok";
    return 0;
}