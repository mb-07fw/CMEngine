#include "IPlatform.hpp"

namespace Platform
{
    ILogger* gP_ActiveLogger = nullptr;

    [[nodiscard]] ILogger* GetActiveLogger() noexcept
    {
        return gP_ActiveLogger;
    }

    void SetActiveLogger(ILogger* pActiveLogger) noexcept
    {
        gP_ActiveLogger = pActiveLogger;
    }

    void ResetActiveLogger() noexcept
    {
        gP_ActiveLogger = nullptr;
    }
}