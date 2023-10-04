#pragma once
#include "KekFramework.hh"

class SystemModule : public Kek::Module
{
  public:
    virtual const char *getModuleName();
    virtual int read(const char *key, JsonDocument &data);
    virtual int write(const char *key, JsonDocument &data,
                      JsonDocument &response);
    void setup(Kek::Scheduler &scheduler) override;
};