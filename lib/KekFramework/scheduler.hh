#pragma once

#include <functional>
#include <type_traits>
#include <Arduino.h>
#include <exception>

#include "logger.hh"

#ifndef KEK_SCHEDULER_SLOTS
#define KEK_SCHEDULER_SLOTS 4
#endif

namespace Kek
{
    typedef std::function<bool()> ModCb;

    struct SchedulerEntry
    {
        uint32_t time;
        ModCb func;
        uint32_t loop;
    };

    
    class Scheduler
    {
    protected:
        SchedulerEntry entries[KEK_SCHEDULER_SLOTS];
        bool findEmptySlot(size_t *dest);

    public:
        Scheduler();
        bool once(uint32_t timeout, ModCb func);
        bool loop(uint32_t interval, ModCb func);
        uint32_t tick();
    };
}