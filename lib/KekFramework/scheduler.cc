#include "scheduler.hh"

Kek::Scheduler::Scheduler()
{
    log("Scheduler initialized");
}

bool Kek::Scheduler::findEmptySlot(size_t *dest)
{
    for (size_t i = 0; i < KEK_SCHEDULER_SLOTS; i++)
    {
        if (entries[i].time == 0)
        {
            *(dest) = i;
            return true;
        }
    }
    return false;
}

bool Kek::Scheduler::once(uint32_t timeout, ModCb func)
{
    auto now = millis();
    size_t i = 0;

    if (!this->findEmptySlot(&i))
    {
        Kek::error("No free scheduler slots");
        return false;
    }
    Kek::log("Got slot %u", i);

    entries[i].time = now + timeout;
    entries[i].func = func;
    entries[i].loop = 0;
    return true;
}

bool Kek::Scheduler::loop(uint32_t interval, ModCb func)
{
    auto now = millis();
    size_t i = 0;

    if (!this->findEmptySlot(&i))
    {
        Kek::error("No free scheduler slots");
        return false;
    }
    Kek::log("Got slot %u", i);

    entries[i].time = now + interval;
    entries[i].func = func;
    entries[i].loop = interval;
    return true;
}

uint32_t Kek::Scheduler::tick()
{
    auto now = millis();
    uint32_t next = 0xffffffff; // stores the time of next handler
    SchedulerEntry *e = entries;

    for (int i = 0; i < KEK_SCHEDULER_SLOTS; e++, i++)
    {
        if (e->time == 0)
            continue; // skip empty

        if (e->time <= now) // run handlers that are due
        {
            try
            {
                e->func();
            }
            catch (std::exception e)
            {
                Kek::error("%s", e.what());
            }

            now = millis(); // update now since the handler could be
                            // slow

            if (e->loop > 0)
            {
                e->time = now + e->loop;
            }
            else
            {
                e->time = 0;
                continue; // skip the next part because event is now
                          // invalid
            }
        }

        if (e->time < next)
            next = e->time;
    }

    if (next < now)
    {
        Kek::warn("Scheduler took too long (or time overflowed)");
        return 0; // report that the next handler is now to make sure we
                  // don't fuck up
    }

    return next - now;
}