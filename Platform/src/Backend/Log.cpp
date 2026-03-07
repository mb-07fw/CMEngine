#include "Backend/Log.hpp"
#include "IPlatform.hpp"

namespace Platform
{
    void LogInfo(std::string_view msg) noexcept
    {
        if (ILogger* pLogger = GetActiveLogger())
            pLogger->LogInfo(msg.data());
    }

    void LogWarning(std::string_view msg) noexcept
    {
        if (ILogger* pLogger = GetActiveLogger())
            pLogger->LogWarning(msg.data());
    }

    void LogFatal(std::string_view msg) noexcept
    {
        if (ILogger* pLogger = GetActiveLogger())
        {
            pLogger->LogFatal(msg.data());
            pLogger->Flush();
        }
    }
}