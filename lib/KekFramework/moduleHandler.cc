#include "moduleHandler.hh"

bool Kek::CstrCmp::operator()(const char *a, const char *b) const
{
    return strcmp(a, b) < 0;
}

Kek::ModuleHandler::ModuleHandler(AsyncWebServer *aws)
{
    this->user = this->pass = nullptr;
    this->modules = new std::map<const char *, Module *, CstrCmp>();

    aws->on(
        "/api/read", HTTP_POST, [this](AsyncWebServerRequest *req)
        { this->handleReadRequest(req); },
        nullptr, [this](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total)
        { this->handleReadRequestWithBody(req, data, len); });
    aws->on(
        "/api/write", HTTP_POST, [this](AsyncWebServerRequest *req)
        { req->send(400, "text/plain", "No body"); },
        nullptr, [this](AsyncWebServerRequest *req, uint8_t *data, size_t len, size_t index, size_t total)
        { this->handleWriteRequestWithBody(req, data, len); });
    aws->on(
        "/api/list", HTTP_GET, [this](AsyncWebServerRequest *req)
        { this->handleListRequest(req); });
    aws->on("/api/auth", HTTP_ANY, [this](AsyncWebServerRequest *req)
            {
        auto auth = req->authenticate(this->user, this->pass);
        if (!auth)
            req->send(401, "text/plain", "Not authenticated");
        else
            req->send(200, "text/plain", "OK"); });
}

void Kek::ModuleHandler::AddDevice(const char *name, Kek::Module *mod)
{
    this->modules->insert(std::pair<const char *, Module *>(name, mod));
    mod->setup(this->scheduler);
}

void Kek::ModuleHandler::setAuth(const char *user, const char *pass)
{
    this->user = strdup(user);
    this->pass = strdup(pass);
}

void Kek::ModuleHandler::handleReadRequest(AsyncWebServerRequest *req)
{
    auto ip = req->client()->remoteIP();

    if (this->user != nullptr && !req->authenticate(this->user, this->pass))
    {
        Kek::info("Unauthorized request from %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        req->send(401, "text/plain", "Unauthorized");
    }

    Kek::info("Read * request from %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    StaticJsonDocument<KEK_JSON_READ_SIZE> doc;
    AsyncResponseStream *response = req->beginResponseStream("application/json");

    for (const auto &p : *this->modules)
    {
        auto mod = p.second;
        auto name = p.first;
        mod->read(name, doc);
    }
    Kek::log("Json doc %d/%dB", doc.memoryUsage(), doc.capacity());
    serializeJson(doc, *response);
    req->send(response);
}

void Kek::ModuleHandler::handleReadRequestWithBody(AsyncWebServerRequest *req, uint8_t *data, size_t len)
{
    req->send(501, "text/plain", "Nism se napisu te kode sefe");
}

void Kek::ModuleHandler::handleWriteRequestWithBody(AsyncWebServerRequest *req, uint8_t *data, size_t len)
{
    auto ip = req->client()->remoteIP();

    if (this->user != nullptr && !req->authenticate(this->user, this->pass))
    {
        Kek::info("Unauthorized request from %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        req->send(401, "text/plain", "Unauthorized");
    }

    Kek::info("Write request from %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    auto mbase = ESP.getFreeHeap();
    StaticJsonDocument<KEK_JSON_WRITE_SIZE> idoc;
    StaticJsonDocument<KEK_JSON_READ_SIZE> odoc;

    auto jsonerr = deserializeJson(idoc, data, len);

    if (jsonerr != DeserializationError::Ok)
    {
        req->send(400, "text/plain", "Invalid JSON");
        return;
    }

    if (!idoc.is<JsonObject>())
    {
        req->send(400, "text/plain", "Invalid JSON");
        return;
    }

    auto obj = idoc.as<JsonObject>();

    for (const JsonPair &p : obj)
    {
        auto name = p.key().c_str();
        try
        {
            auto mod = this->modules->at(name);
            auto code = mod->write(name, idoc, odoc);
            if (code != 0)
            {
                Kek::error("Write failed for %s with code %d", name, code);
            }
        }
        catch (const std::out_of_range &e)
        {
            odoc[name]["error"] = "missing";
            Kek::error("Device '%s' does not exist", name);
        }
    }

    AsyncResponseStream *response = req->beginResponseStream("application/json");

    serializeJson(odoc, *response);
    req->send(response);

    delete response;
}

void Kek::ModuleHandler::handleListRequest(AsyncWebServerRequest *req)
{
    auto ip = req->client()->remoteIP();

    if (this->user != nullptr && !req->authenticate(this->user, this->pass))
    {
        Kek::info("Unauthorized request from %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        req->send(401, "text/plain", "Unauthorized");
    }

    Kek::info("List request from %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    StaticJsonDocument<KEK_JSON_LIST_SIZE> doc;
    AsyncResponseStream *response = req->beginResponseStream("application/json");

    for (const auto &p : *this->modules)
    {
        doc[p.first] = p.second->getModuleName();
    }

    Kek::log("Json doc %d/%dB", doc.memoryUsage(), doc.capacity());

    serializeJson(doc, *response);
    req->send(response);
}

void Kek::ModuleHandler::SchedTick(bool sleep)
{
    auto t = this->scheduler.tick();
    if (sleep)
        delay(t);
}