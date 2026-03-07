#pragma once

#include <string>
#include <string_view>

namespace Platform::Backend::Win32
{
	[[nodiscard]] std::wstring MultibyteToWide(std::string_view str) noexcept;
}