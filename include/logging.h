#pragma once

#define NOMINMAX
#include <windows.h>

#include <cstdarg>
#include <cstdio>

namespace logging
{
inline void Debug(const char* format, ...)
{
    char message[1024]{};

    va_list args;
    va_start(args, format);
    vsnprintf_s(message, sizeof(message), _TRUNCATE, format, args);
    va_end(args);

    OutputDebugStringA("[NoStatReqs] ");
    OutputDebugStringA(message);
    OutputDebugStringA("\n");
}
}
