#include "Asset/FileWatcher.hpp"
#include "efsw/efsw.hpp" // file watcher

#include <iostream>
#include <unordered_map>

namespace Engine::Asset
{
	class UpdateListener : public efsw::FileWatchListener
	{
	public:
		void handleFileAction(
			efsw::WatchID id,
			const std::string& dir,
			const std::string& filename,
			efsw::Action action,
			std::string oldFilename
		) noexcept override
        {
            switch (action)
            {
            case efsw::Actions::Add:
                std::cout << "ID (" << id << ") DIR(" << dir << ") FILE(" << filename << ") has event Added"
                    << std::endl;
                break;
            case efsw::Actions::Delete:
                if (oldFilename.empty() || oldFilename == filename)
                    std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete" << std::endl;
                else
                    std::cout << "DIR (" << dir << ") FILE (" << oldFilename << ") was renamed to `"
                    << filename << '`' << std::endl;

                break;
            case efsw::Actions::Modified:
                std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Modified"
                    << std::endl;
                break;
            case efsw::Actions::Moved:
                std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from ("
                    << oldFilename << ")" << std::endl;
                break;
            default:
                std::cout << "File event should never happen!" << std::endl;
            }
        }
	};

    struct FileWatcher::Impl
    {
        Impl() noexcept
        {
            pFileWatcher = std::make_unique<efsw::FileWatcher>();
            pListener = std::make_unique<UpdateListener>();

            /* Start watching asynchronously... */
            pFileWatcher->watch();
        }

        ~Impl() = default;

        std::unique_ptr<efsw::FileWatcher> pFileWatcher;
        /* NOTE: Currently, all watches are set under a single listener. */
        std::unique_ptr<UpdateListener> pListener;
    };

    FileWatcher::FileWatcher() noexcept
    {
        mP_Impl = new Impl;
    }

    FileWatcher::~FileWatcher() noexcept
    {
        delete mP_Impl;
    }

    void FileWatcher::WatchFile(const std::filesystem::path& filePath) noexcept
    {
        
    }
}