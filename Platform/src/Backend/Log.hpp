#pragma once

#include <string>
#include <string_view>
#include <format>

#include "IPlatform.hpp"

namespace Platform
{
    void LogInfo(std::string_view msg) noexcept;
    void LogWarning(std::string_view msg) noexcept;
    void LogFatal(std::string_view msg) noexcept;

    template <typename... Args>
    [[nodiscard]] std::string VFormat(std::string_view fmt, Args&&... args) noexcept
    {
        return std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
    }

    template <typename... Args>
    void LogInfoV(std::string_view fmt, Args&&... args) noexcept
    {
        ILogger* pLogger = GetActiveLogger();

        if (!pLogger)
            return;

        pLogger->LogInfo(VFormat(fmt, std::forward<Args>(args)...).c_str());
    }

    template <typename... Args>
    void LogWarningV(std::string_view fmt, Args&&... args) noexcept
    {
        ILogger* pLogger = GetActiveLogger();

        if (!pLogger)
            return;

        pLogger->LogWarning(VFormat(fmt, std::forward<Args>(args)...).c_str());
    }

    template <typename... Args>
    void LogFatalV(std::string_view fmt, Args&&... args) noexcept
    {
        ILogger* pLogger = GetActiveLogger();

        if (!pLogger)
            return;

        pLogger->LogFatal(VFormat(fmt, std::forward<Args>(args)...).c_str());
    }
}