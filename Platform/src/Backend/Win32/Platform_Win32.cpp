#include "Backend/Win32/Platform_Win32.hpp"
#include "Backend/Win32/Window_Win32.hpp"
#include "Backend/Win32/Context_Win32.hpp"
#include "Backend/Win32/PlatformOS_Win32.hpp"
#include "Backend/Win32/D3D/_11/Api_D3D11.hpp"
#include "Common/Assert.hpp"
#include "efsw/efsw.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <string_view>

namespace Platform::Backend::Win32
{
    // Inherits from the abstract listener class, and implements the the file action handler
    class UpdateListener : public efsw::FileWatchListener
    {
    public:
        void handleFileAction(
            efsw::WatchID watchid,
            const std::string& dir,
            const std::string& filename, 
            efsw::Action action,
            std::string oldFilename
        ) noexcept override
        {
            if (filename == "test.txt")
                std::cout << "Ayyyyy target!!!!!!!\n";

            switch (action)
            {
            case efsw::Actions::Add:
                std::cout << "ID (" << watchid << ") DIR(" << dir << ") FILE(" << filename << ") has event Added"
                    << std::endl;
                break;
            case efsw::Actions::Delete:
                std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete"
                    << std::endl;
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

    struct Platform::Impl
    {
        Impl()
        {
            pFileWatcher = std::make_unique<efsw::FileWatcher>();
            pListener = std::make_unique<UpdateListener>();
            TestID = pFileWatcher->addWatch(CONFIG_SOURCE_DIR "test", pListener.get(), false);

            PLATFORM_FAILURE_IF_V(
                !TestID,
                "Failed to watch dir; Error: {}", TestID
            );

            // Start watching asynchronously the directories
            pFileWatcher->watch();
        }

        ~Impl() noexcept
        {
            pFileWatcher->removeWatch(TestID);
        }

        Win32::Window Window;
        Win32::Context Context;
        std::unique_ptr<efsw::FileWatcher> pFileWatcher;
        std::unique_ptr<UpdateListener> pListener;
        efsw::WatchID TestID;
    };

    Platform::Platform() noexcept
    {
        /* ... manual command line parsing shenanigans ...  */
        const wchar_t* pCmdLineWStr = GetCommandLineW();
		int numArgs = 0;

		const LPWSTR* pCmdLineArgsWStr = CommandLineToArgvW(pCmdLineWStr, &numArgs);

		if (numArgs > 1)
        {
            constexpr std::wstring_view GraphicsDebuggerFlag = L"-fGRAPHICS_DEBUGGER";

            for (int i = 1; i < numArgs; ++i)
                if (GraphicsDebuggerFlag == pCmdLineArgsWStr[i])
                    m_Settings.IsGraphicsDebugging = true;

        }

        mP_Impl = new Impl();

        std::cout << "Platform_Win32!\n";
    }

    Platform::~Platform() noexcept
    {
        delete mP_Impl;
        mP_Impl = nullptr;

        std::cout << "Platform_Win32 Destroyed!\n";
    }

    void Platform::CreateGraphicsContext(ApiType api) noexcept
    {
        ::HWND hWnd = mP_Impl->Window.Impl_hWnd();
        Win32::Context& context = mP_Impl->Context;
        Win32::Window& window = mP_Impl->Window;

        ASSERT(
            context.Impl_Create(api, m_Settings, hWnd),
            "(Platform_Win32) Failed to create a graphics api instance."
        );

        auto onResizeFunc = [](
            uint32_t width,
            uint32_t height,
            void* pUserData)
        {
            auto& platform = *Reinterpret<Platform*>(pUserData);

            auto& api = platform.Context().Api();

            /* TODO: Make window resizing deferred... */
            api.OnWindowResize(width, height);
        };

        window.SetWindowResizeCallback(onResizeFunc, this);
    }

    [[nodiscard]] IWindow& Platform::Window() noexcept
    {
        return mP_Impl->Window;
    }

    [[nodiscard]] IContext& Platform::Context() noexcept
    {
        return mP_Impl->Context;
    }

    [[nodiscard]] IApi& Platform::GetGraphicsApi() noexcept
    {
        return Context().Api();
    }
    
    [[nodiscard]] IApiFactory& Platform::GetGraphicsApiFactory() noexcept
    {
        return Context().Api().Factory();
    }
}