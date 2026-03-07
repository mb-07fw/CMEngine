#pragma once

#include "Export.hpp"
#include "Types.hpp"

#include <cstdint>

namespace Platform
{
	using FSUpdateUnderlying = uint8_t;

	enum class PLATFORM_API FSUpdateType : FSUpdateUnderlying
	{
		Invalid,
		Write = 1 << 0
	};

	using FSUpdateMask = FSUpdateUnderlying;

#include "Common/UtilityDef.hpp"
	ENUM_CLASS_BITWISE_OPS_IMPL(
		FSUpdateType,
		FSUpdateUnderlying,
		inline PLATFORM_API constexpr [[nodiscard]])
#include "Common/UtilityUndef.hpp"

	enum class PLATFORM_API FSUpdateProtocol : uint8_t
	{
		Invalid,
		Flush, // callback is notified synchronously when IFileWatcher::FlushUpdates() is called.
		Async_Immediate, // callback is notified asynchronously when a single event is processed.
		Async_Deferred // callback is notified asynchronously when all pending file events are processed.
	};

	enum class PLATFORM_API FSUpdateCallbackType : uint8_t
	{
		Invalid,
		Single, /* Corresponds to FSUpdateProtocol::Async_Immediate. */
		Batch /* Corresponds to FSUpdateProtocol::Flush, or FSUpdateProtocol::Async_Deferred. */
	};

	struct PLATFORM_API FSUpdate
	{
		FSUpdateType Type = FSUpdateType::Invalid;
		StringView Path;
		OpaquePtr pPayload = nullptr;
	};

	using FSSingleUpdateCallback = void (*)(void* pUserData, const FSUpdate&);
	using FSBatchUpdateCallback = void (*)(void* pUserData, const FSUpdate*, size_t numUpdates);

	class PLATFORM_API IFileWatcher
	{
	public:
		IFileWatcher() = default;
		virtual ~IFileWatcher() = default;

		virtual void WatchFile(
			const char* pFileName //,
			//FSUpdateMask updateMask,
			//FSUpdateProtocol protocol,
			//OpaquePtr pUserData = nullptr, /* this pointer's lifetime is NOT managed here... */
			//FSSingleUpdateCallback pOptSingleUpdateCallback = nullptr,
			//FSBatchUpdateCallback pOptBatchUpdateCallback = nullptr
		) noexcept = 0;

		//virtual void UnwatchFile(const char* pFileName) noexcept = 0;
	};
}