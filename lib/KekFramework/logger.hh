#pragma once
#include <Arduino.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

namespace Kek
{
    void log(const char *format, ...);
    void info(const char *format, ...);
    void warn(const char *format, ...);
    void error(const char *format, ...);
}