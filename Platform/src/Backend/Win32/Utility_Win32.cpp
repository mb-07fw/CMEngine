#include "Backend/PlatformFailure.hpp"
#include "Utility_Win32.hpp"
#include "Common/Cast.hpp"

#include "PlatformOS_Win32.hpp"

namespace Platform::Backend::Win32
{
	[[nodiscard]] std::wstring MultibyteToWide(std::string_view str) noexcept
	{
		if (str.empty())
			return std::wstring(L"");

		std::wstring outStr;

		int outStrByteSize = ::MultiByteToWideChar(
			CP_UTF8,
			0,
			str.data(),
			-1,
			nullptr,
			0
		);

		PLATFORM_FAILURE_IF(
			outStrByteSize == 0,
			"(MultibyteToWide) Failed to get required size of wstring buffer."
		);

		outStr.resize(Cast<size_t>(outStrByteSize));

		int convertedByteSize = ::MultiByteToWideChar(
			CP_UTF8,
			0,
			str.data(),
			-1,
			outStr.data(),
			Cast<int>(outStr.size())
		);

		PLATFORM_FAILURE_IF_V(
			convertedByteSize == 0,
			"(MultibyteToWide) Failed to convert string. String: {}",
			str
		);

		return outStr;
	}
}