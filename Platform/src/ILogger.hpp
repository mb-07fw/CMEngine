#pragma once

#include "Export.hpp"

namespace Platform
{
    class PLATFORM_API ILogger
    {
    public:
        ILogger() = default;
        virtual ~ILogger() = default;

        virtual void LogInfo(const char* pStr) noexcept = 0;
        virtual void LogWarning(const char* pStr) noexcept = 0;
        virtual void LogCritical(const char* pStr) noexcept = 0;
        virtual void LogFatal(const char* pStr) noexcept = 0;

        virtual void Flush() noexcept = 0;
    };
}