#include "DebugLog.h"
#include <windows.h>
#include <stdio.h>

DebugLog* DebugLog::instance_ = nullptr;

void DebugLog::LogFormattedMessage(const char* format, ...)
{
    char buffer[256]; 
    va_list args;
    va_start(args, format);
    vsprintf_s(buffer, sizeof(buffer), format, args);
    va_end(args);

    OutputDebugStringA(buffer);
}
