#pragma once

#include "Export.hpp"

#include <cstdint>

namespace Platform
{
    using OpaquePtr = void*;

    enum class PLATFORM_API Byte : uint8_t {};

    class PLATFORM_API StringView
    {
    public:
        StringView(const char* pStr) noexcept;

        StringView() = default;
        ~StringView() = default;

        StringView(const StringView&) = default;
        StringView(StringView&&) = default;
        StringView& operator=(const StringView&) = default;
        StringView& operator=(StringView&&) = default;

        inline [[nodiscard]] const char* Data() const noexcept { return mP_Str; }
        inline [[nodiscard]] size_t Size() const noexcept { return m_Size; }
        inline [[nodiscard]] bool Empty() const noexcept { return m_Size == 0; }
        inline [[nodiscard]] bool Null() const noexcept { return !mP_Str; }

        [[nodiscard]] bool operator==(const StringView& other) const noexcept;
    private:
        const char* mP_Str = "";
        size_t m_Size = 0;
    };

    class PLATFORM_API String
    {
    public:
        String(const char* pStr) noexcept;
        String(const StringView& view) noexcept;

        String() noexcept;
        String(const String&) noexcept;
        String(String&&) noexcept;
        String& operator=(const String&) noexcept;
        String& operator=(String&&) noexcept;

        ~String() noexcept;

        String& operator+=(const char* pStr) noexcept;

        void Assign(const char* pStr, size_t length) noexcept;
        void Assign(const String& other) noexcept;
        void Assign(String&& other) noexcept;

        void Append(const char* pStr) noexcept;
        void Resize(size_t newCapacity) noexcept;

        inline [[nodiscard]] const char* Data() const noexcept { return ActiveStorage(); }
        inline [[nodiscard]] size_t Size() const noexcept { return m_Size; }
        inline [[nodiscard]] size_t Capacity() const noexcept { return m_Capacity; }
        inline [[nodiscard]] bool Empty() const noexcept { return m_Size == 0; }
    private:
        [[nodiscard]] char* AllocCapacity(size_t capacity) noexcept;
        void Dealloc() noexcept;

        void InlineCopy(const char* pStr, size_t length) noexcept;
        void InlineCopyTo(char* pDest, size_t destLength) const noexcept;
        void HeapCopy(const char* pStr, size_t length) noexcept;
        void HeapCopyOffset(size_t offset, const char* pStr, size_t length) noexcept;
        void HeapCopyTo(char* pDest, size_t destLength) const noexcept;

        [[nodiscard]] const char* ActiveStorage() const noexcept;

        [[nodiscard]] bool IsSmall() const noexcept;
    private:
        union
        {
            char* mP_Str;
            char m_InlineBuffer[16] = { '\0' };
        };
        static constexpr size_t S_MaxSmallCapacity = sizeof(m_InlineBuffer) - 1;
        size_t m_Size = 0;
        size_t m_Capacity = S_MaxSmallCapacity;
    };
}