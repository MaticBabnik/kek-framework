#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFi.h>

#define KEK_SCHEDULER_SLOTS 32
#include <KekFramework.hh>

// modules
#include "modules/DhtModule.hh"
#include "modules/HeaterModule.hh"
#include "modules/PinModule.hh"
#include "modules/SystemModule.hh"

AsyncWebServer aws(80);
Kek::ModuleHandler mh(&aws);

#pragma region Entry
bool getConfig(JsonDocument *doc)
{
    File f = SPIFFS.open("/config.json", "r");
    if (!f)
    {
        Kek::error("Failed to open config file");
        return false;
    }

    DeserializationError err = deserializeJson(*doc, f);
    if (err != DeserializationError::Ok)
    {
        Serial.println("Failed to read config file");
        return false;
    }

    if (!(*doc)["ssid"].is<const char *>() || !(*doc)["key"].is<const char *>())
    {
        Kek::error("Invalid config file");
        return false;
    }

    if (!(*doc)["username"].is<const char *>() ||
        !(*doc)["password"].is<const char *>())
    {
        Kek::error("No auth!");
        return false;
    }

    return true;
}

void wifiDebug()
{
    Kek::log("Dumping wifi info");
    WiFi.printDiag(Serial);
}

void init()
{
    StaticJsonDocument<256> config;

    Serial.begin(115200);
    Kek::info("Serial up");

    // mount fs
    {
        auto spiffs = SPIFFS.begin(false, "/data");
        if (!spiffs)
        {
            Kek::error(" Failed to mount SPIFFS, restarting...");
            goto die;
        }
        Kek::info("SPIFFS mounted");
    }

    if (!getConfig(&config))
    {
        Kek::error("Failed to get config");
        goto die;
    }

    // connect to network if hotspot isn't set / is false
    if (!config["hotspot"].is<bool>() || !config["hotspot"].as<bool>())
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(config["ssid"].as<const char *>(),
                   config["key"].as<const char *>());
        int retries = 0;
        while (WiFi.status() != WL_CONNECTED)
        {
            if (retries > 10)
            {
                Kek::error("Network timeout (code: %d), restarting...",
                           WiFi.status());
                wifiDebug();
                goto die;
            }

            Kek::info("Waiting for network");
            retries++;
            delay(1000);
        }
        Kek::info("Network up");
        auto ip = WiFi.localIP();
        Kek::info("IP address: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    }
    else
    {
        WiFi.mode(WIFI_MODE_AP);
        WiFi.softAP(config["ssid"].as<const char *>(),
                    config["key"].as<const char *>());

        auto ip = WiFi.softAPIP();
        Kek::info("IP address: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    }

    mh.setAuth(config["username"].as<const char *>(),
               config["password"].as<const char *>());

    return;
die:
    delay(5000);
    ESP.restart();
}

#pragma endregion Entry

void setup()
{
    init();

    // Device modules here
    // mh.AddDevice("grelec delavnica", new PinModule(13));
    // mh.AddDevice("grelec", new PinModule(25));
    // mh.AddDevice("vlazilec", new PinModule(17));

    // mh.AddDevice("senzor", new DhtModule(26, DHT22));

    mh.AddDevice("delavnica", new HeaterModule(26, 25, 17));

    mh.AddDevice("system", new SystemModule());
    // end of device modules

    aws.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "no");
    });
    aws.serveStatic("/", SPIFFS, "/w").setDefaultFile("index.html");
    aws.begin();
}

void loop()
{
    mh.SchedTick(true);
}