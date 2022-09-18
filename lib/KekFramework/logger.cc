#include "logger.hh"

void Kek::log(const char *format, ...)
{
    Serial.write("\x1B[37m[DEBUG] ");
    char loc_buf[64];
    char *temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);

    if (len < 0)
        goto end;

    if (len >= sizeof(loc_buf))
    {
        temp = (char *)malloc(len + 1);
        if (temp == NULL)
            goto end;
        len = vsnprintf(temp, len + 1, format, arg);
    }

    len = Serial.write((uint8_t *)temp, len);

    if (temp != loc_buf)
        free(temp);

end:
    Serial.write("\x1B[0m\n");
    va_end(arg);
    return;
}

void Kek::info(const char *format, ...)
{
    Serial.write("\x1B[96m [INFO] ");
    char loc_buf[64];
    char *temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);

    if (len < 0)
        goto end;

    if (len >= sizeof(loc_buf))
    {
        temp = (char *)malloc(len + 1);
        if (temp == NULL)
            goto end;
        len = vsnprintf(temp, len + 1, format, arg);
    }

    len = Serial.write((uint8_t *)temp, len);

    if (temp != loc_buf)
        free(temp);

end:
    Serial.write("\x1B[0m\n");
    va_end(arg);
    return;
}

void Kek::warn(const char *format, ...)
{
    Serial.write("\x1B[93m [WARN] ");
    char loc_buf[64];
    char *temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);

    if (len < 0)
        goto end;

    if (len >= sizeof(loc_buf))
    {
        temp = (char *)malloc(len + 1);
        if (temp == NULL)
            goto end;
        len = vsnprintf(temp, len + 1, format, arg);
    }

    len = Serial.write((uint8_t *)temp, len);

    if (temp != loc_buf)
        free(temp);

end:
    Serial.write("\x1B[0m\n");
    va_end(arg);
    return;
}

void Kek::error(const char *format, ...)
{
    Serial.write("\x1B[91m[ERROR] ");
    char loc_buf[64];
    char *temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);

    if (len < 0)
        goto end;

    if (len >= sizeof(loc_buf))
    {
        temp = (char *)malloc(len + 1);
        if (temp == NULL)
            goto end;
        len = vsnprintf(temp, len + 1, format, arg);
    }

    len = Serial.write((uint8_t *)temp, len);

    if (temp != loc_buf)
        free(temp);

end:
    Serial.write("\x1B[0m\n");
    va_end(arg);
    return;
}