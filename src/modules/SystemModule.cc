#include "SystemModule.hh"

const char *SystemModule::getModuleName()
{
    return "ESP32";
}

int SystemModule::read(const char *key, JsonDocument &data)
{
    data[key]["heap"]["free"] = ESP.getFreeHeap();
    data[key]["heap"]["total"] = ESP.getHeapSize();
    data[key]["chip"]["cores"] = ESP.getChipCores();
    data[key]["chip"]["model"] = ESP.getChipModel();
    data[key]["chip"]["rev"] = ESP.getChipRevision();
    data[key]["chip"]["freq"] = ESP.getCpuFreqMHz();
    data[key]["uptime"] = millis() / 1000;
    return 0;
}

int SystemModule::write(const char *key, JsonDocument &data, JsonDocument &response)
{
    response[key] = "Read only";
    return 2;
}

void SystemModule::setup(Kek::Scheduler &scheduler)
{
    Kek::log("Sys::setup");
    scheduler.once(10000, []()
                   {
                    Kek::info("Alive for 10s");
                    return false; });
    scheduler.loop(1000, []()
                   {
                        Kek::info("Still alive");
                        return true; });
}