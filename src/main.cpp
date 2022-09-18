#include <Arduino.h>

#include <SPIFFS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <KekFramework.hh>

AsyncWebServer aws(80);

void init()
{
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

  // network
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin("Infinity IOT", "homeAndBeyond");
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      if (retries > 10)
      {
        Kek::error("Network timeout (code: %d), restarting...", WiFi.status());
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


  return;
die:
  delay(1000);
  ESP.restart();
}

void setup()
{
  init();
  aws.onNotFound([](AsyncWebServerRequest *request)
                 { request->send(404, "text/plain", "no"); });
  aws.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  aws.begin();
}

void loop()
{
}