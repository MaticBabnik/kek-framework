#pragma once
#include <ArduinoJson.h>

#include "logger.hh"
#include "scheduler.hh"

namespace Kek
{
class Module
{
  public:
    virtual const char *getModuleName();
    virtual int read(const char *key, JsonDocument &data);
    virtual int write(const char *key, JsonDocument &data,
                      JsonDocument &response);
    virtual void setup(
        Scheduler &scheduler); // TODO: figure out how to make it a friend
};
} // namespace Kek