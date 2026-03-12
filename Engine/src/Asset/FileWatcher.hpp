#pragma once

#include <filesystem>

namespace Engine::Asset
{
	class FileWatcher
	{
	public:
		FileWatcher() noexcept;
		~FileWatcher() noexcept;

		void WatchFile(const std::filesystem::path& filePath) noexcept;
	private:
		struct Impl;
		Impl* mP_Impl = nullptr;
	};
}