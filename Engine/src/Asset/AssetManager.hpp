#pragma once

#include "Asset/Asset.hpp"
#include "Asset/FileWatcher.hpp"
#include "Common/Cast.hpp"
#include "Common/View.hpp"

#include <cstdint>

#include <filesystem>
#include <functional>
#include <string_view>
#include <unordered_map>

namespace Engine::Asset
{
	using GlobalID = uint32_t;

	struct File
	{
		std::filesystem::path Path;
	};

	enum class ResultType : int8_t
	{
		Invalid = -127,
		Failed_File_Absent,
		Failed_File_Import,
		Failed_File_Import_Invalid_Format,
		Failed_File_Serialize,
		Failed_Handle_Not_Registered,
		Failed_Handle_Not_Mapped,
		Failed_Handle_Already_Registered,
		Failed_Handle_Already_Mapped,
		Failed_Handle_Mismatching_Asset_Type,
		Failed_Handle_Mismatching_Mapped_Type,
		Failed = 0,
		Succeeded
	};

	struct Result
	{
		inline constexpr Result(ResultType type) noexcept
			: Type(type)
		{
		}

		constexpr Result() = default;
		constexpr ~Result() = default;

		inline static constexpr [[nodiscard]] std::string_view TypeToStringView(ResultType type) noexcept
		{
			switch (type)
			{
			case ResultType::Invalid:
				return std::string_view("Invalid");
			case ResultType::Failed_File_Absent:
				return std::string_view("Failed_File_Absent");
			case ResultType::Failed_File_Import:
				return std::string_view("Failed_File_Import");
			case ResultType::Failed_File_Import_Invalid_Format:
				return std::string_view("Failed_File_Import_Invalid_Format");
			case ResultType::Failed_File_Serialize:
				return std::string_view("Failed_File_Serialize");
			case ResultType::Failed_Handle_Not_Registered:
				return std::string_view("Failed_Handle_Not_Registered");
			case ResultType::Failed_Handle_Not_Mapped:
				return std::string_view("Failed_Handle_Not_Mapped");
			case ResultType::Failed_Handle_Already_Registered:
				return std::string_view("Failed_Handle_Already_Registered");
			case ResultType::Failed_Handle_Already_Mapped:
				return std::string_view("Failed_Handle_Already_Mapped");
			case ResultType::Failed_Handle_Mismatching_Asset_Type:
				return std::string_view("Failed_Handle_Mismatching_Asset_Type");
			case ResultType::Failed_Handle_Mismatching_Mapped_Type:
				return std::string_view("Failed_Handle_Mismatching_Mapped_Type");
			case ResultType::Failed:
				return std::string_view("Failed");
			case ResultType::Succeeded:
				return std::string_view("Succeeded");
			default:
				return std::string_view("Unknown");
			}
		}

		inline constexpr operator ResultType() const noexcept { return Type; }

		inline constexpr [[nodiscard]] int8_t ToInt8() const noexcept { return Cast<int8_t>(Type); }
		inline constexpr [[nodiscard]] int16_t ToInt16() const noexcept { return Cast<int16_t>(Type); }
		inline constexpr [[nodiscard]] std::string_view ToStringView() const noexcept { return TypeToStringView(Type); }
		inline constexpr [[nodiscard]] bool Succeeded() const noexcept { return Type == ResultType::Succeeded; }
		inline constexpr [[nodiscard]] bool Failed() const noexcept { return Cast<int8_t>(Type) < Cast<int8_t>(ResultType::Succeeded); }
		inline constexpr [[nodiscard]] bool Invalid() const noexcept { return Type == ResultType::Invalid; }

		inline constexpr operator bool() const noexcept { return Succeeded(); }
		inline constexpr [[nodiscard]] bool operator!() const noexcept { return Failed(); }

		ResultType Type = ResultType::Invalid;
	};

	enum class WatchProtocol : uint8_t
	{ 
		Invalid,
		None,
		Reload,			/* Asset is watched. Any overwrites are deferred until AssetManager::Update. */
		ReloadAndNotify /* Asset is watched. Any overwrites and notifications are deferred until AssetManager::Update. */

	};

	class AssetManager
	{
	public:
		AssetManager() noexcept;
		~AssetManager() noexcept;

		AssetManager(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;
		AssetManager& operator=(AssetManager&&) = delete;

		/* Applies all pending asset reloads, and dispatches update notifications.
		 * 
		 * (internal file watcher is async, meaning to ensure 
		 *  ease of use and prevent data races, asset reloading 
		 *  should have a single deterministic operation point,
		 *  of which it is deferred) */
		void Update() noexcept;

		[[nodiscard]] AssetID LoadAsset(
			AssetType type,
			const std::filesystem::path& filePath,
			View<Result> optOutResult = View<Result>()
		) noexcept;

		[[nodiscard]] Result SetWatch(
			AssetID id,
			WatchProtocol protocol,
			const std::function<void(const Asset* pUpdated)> onUpdateFunc
		) noexcept;

		/* Returns true if the handle was registered previously; false otherwise. */
		bool UnregisterID(AssetID& outID) noexcept;

		[[nodiscard]] bool IsMapped(AssetID id) noexcept;
	private:
		[[nodiscard]] bool LoadFileRaw(
			const std::filesystem::path& filePath,
			std::vector<char>& outFileBuffer, 
			View<Result>& optOutResult
		) noexcept;

		[[nodiscard]] AssetID LoadShader(
			const std::filesystem::path& filePath,
			View<Result>& optOutResult
		) noexcept;

		[[nodiscard]] ConstView<Shader> GetShader(AssetID id) const noexcept;

		[[nodiscard]] uint32_t NextGlobalID() noexcept;

		void CleanupID(AssetID& outID) noexcept;

		void SetResultIfValid(View<Result> result, ResultType type) noexcept;

		template <typename Asset, typename Map>
		inline [[nodiscard]] ConstView<Asset> GetAsset(
			const Map& map,
			AssetType type,
			AssetID id
		) const noexcept;
	private:
		uint32_t m_TotalAssetCount = 0;
		std::vector<GlobalID> m_FreeGlobalIDs;
		std::unordered_map<AssetID, Shader> m_ShaderMap;
		FileWatcher m_FileWatcher;
	};

	template <typename Asset, typename Map>
	inline [[nodiscard]] ConstView<Asset> AssetManager::GetAsset(
		const Map& map,
		AssetType type,
		AssetID id
	) const noexcept
	{
		if (id.Type() != type || !id.IsRegistered())
			return nullptr;

		auto it = map.find(id);
		if (it == map.end())
			return nullptr;

		return &it->second;
	}
}