#pragma once

#ifdef _WIN32
    #include "Backend/Win32/PlatformOS_Win32.hpp"
    #include "Backend/Win32/PlatformFwd_Win32.hpp"
#else
    #error "Unsupported platform!!!"
#endif

#include <cstdint>
#include <cctype>
#include <cwctype>

#include <atomic>
#include <algorithm>
#include <condition_variable>
#include <iostream>
#include <exception> // std::terminate
#include <format>
#include <memory>
#include <mutex>
#include <source_location>
#include <stop_token>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>