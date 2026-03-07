#pragma once

#include "Export.hpp"
#include "ILogger.hpp"
#include "IApi.hpp"

namespace Platform
{
    struct PLATFORM_API PlatformSettings
    {
        bool IsGraphicsDebugging = false;
    };

    /* WARNING: Lifetime of the logger is NOT managed by the platform backend. */
    PLATFORM_API [[nodiscard]] ILogger* GetActiveLogger() noexcept;
    PLATFORM_API void SetActiveLogger(ILogger* pActiveLogger) noexcept;
    PLATFORM_API void ResetActiveLogger() noexcept;

    class PLATFORM_API IPlatform
    {
    public:
        IPlatform() = default;
        virtual ~IPlatform() = default;

        IPlatform(const IPlatform&) = delete;
        IPlatform(IPlatform&&) = delete;
        IPlatform& operator=(const IPlatform&) = delete;
        IPlatform& operator=(IPlatform&&) = delete;

        /* These are shorthand functions so I don't have to type platform.context().api()... */
        virtual [[nodiscard]] IApi& GetGraphicsApi() noexcept = 0;
        virtual [[nodiscard]] IApiFactory& GetGraphicsApiFactory() noexcept = 0;
    };
}