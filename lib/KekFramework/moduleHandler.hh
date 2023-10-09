#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>

#include <functional>
#include <map>
#include <type_traits>

#include "logger.hh"
#include "module.hh"
#include "scheduler.hh"

#ifndef KEK_JSON_LIST_SIZE
#define KEK_JSON_LIST_SIZE 384
#endif

#ifndef KEK_JSON_READ_SIZE
#define KEK_JSON_READ_SIZE 2048
#endif

#ifndef KEK_JSON_WRITE_SIZE
#define KEK_JSON_WRITE_SIZE 512
#endif

namespace Kek
{
class CstrCmp
{
  public:
    bool operator()(const char *a, const char *b) const;
};

class ModuleHandler
{
    // no destructor since it would only get called once the ESP reboots
  public:
    ModuleHandler(AsyncWebServer *aws);
    void AddDevice(const char *name, Module *mod);
    void setAuth(const char *user, const char *pass);
    void SchedTick(bool sleep = false);

  protected:
    char *user, *pass;
    std::map<const char *, Module *, CstrCmp> *modules;
    Scheduler scheduler;

    void handleReadRequest(AsyncWebServerRequest *req);
    void handleReadRequestWithBody(AsyncWebServerRequest *req, uint8_t *data,
                                   size_t len);
    void handleWriteRequestWithBody(AsyncWebServerRequest *req, uint8_t *data,
                                    size_t len);
    void handleListRequest(AsyncWebServerRequest *req);
};
} // namespace Kek