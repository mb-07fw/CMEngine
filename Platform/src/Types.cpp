#include "Types.hpp"
#include "Common/Cast.hpp"
#include "Common/Assert.hpp"

#include <string.h> // strnlen
#include <cstring>

namespace Platform
{
    template <typename StringLike>
    [[nodiscard]] bool StringEqual(const StringLike& lhs, const StringLike& rhs) noexcept
    {
        if ((lhs.Null() || rhs.Null()) ||
            lhs.Size() != rhs.Size())
            return false;

        return std::memcmp(lhs.Data(), rhs.Data(), lhs.Size()) == 0;
    }

    [[nodiscard]] size_t StringLength(const char* pStr, size_t maxLength = 1024) noexcept
    {
        return strnlen(pStr, maxLength);
    }

    StringView::StringView(const char* pStr) noexcept
    {
        if (!pStr)
            return;

        mP_Str = pStr;
        m_Size = StringLength(pStr);
    }

    [[nodiscard]] bool StringView::operator==(const StringView& other) const noexcept
    {
        return StringEqual(*this, other);
    }

    String::String(const char* pStr) noexcept
    {
        if (!pStr)
            return;

        const size_t length = StringLength(pStr);

        if (length < sizeof(m_InlineBuffer))
        {
            InlineCopy(pStr, length);
            return;
        }

        mP_Str = AllocCapacity(length);
        m_Capacity = length;

        constexpr size_t Offset = 0;
        HeapCopyOffset(Offset, pStr, length);

        m_Size = length;
    }

    String::String(const StringView& view) noexcept
    {
        Assign(view.Data(), view.Size());
    }

    String::String() noexcept
    {
    }

    String::String(const String& other) noexcept
    {
        Assign(other);
    }

    String::String(String&& other) noexcept
    {
        Assign(std::move(other));
    }

    String& String::operator=(const String& other) noexcept
    {
        Assign(other);
        return *this;
    }

    String& String::operator=(String&& other) noexcept
    {
        Assign(std::move(other));
        return *this;
    }

    String& String::operator+=(const char* pStr) noexcept
    {
        Append(pStr);
        return *this;
    }

    void String::Assign(const char* pStr, size_t length) noexcept
    {
        if (IsSmall() && length <= S_MaxSmallCapacity)
        {
            InlineCopy(pStr, length);
            return;
        }

        if (IsSmall() && length >= S_MaxSmallCapacity)
        {
            m_Capacity = length;
            mP_Str = AllocCapacity(m_Capacity);
        }
        else
            Resize(length);

        HeapCopy(pStr, length);
    }

    void String::Assign(const String& other) noexcept
    {
        if (other.Empty())
            return;

        if (IsSmall() && other.IsSmall())
        {
            InlineCopy(other.m_InlineBuffer, other.Size());
            return;
        }

        Resize(other.Capacity());
        HeapCopy(other.Data(), other.Size());
    }

    void String::Assign(String&& other) noexcept
    {
        if (!IsSmall() && mP_Str)
            Dealloc();

        if (other.Empty())
            return;

        if (other.IsSmall())
        {
            InlineCopy(other.m_InlineBuffer, other.Size());

            other.m_Size = 0;
            other.m_InlineBuffer[0] = '\0';

            return;
        }

        m_Capacity = other.Capacity();
        m_Size = other.Size();
        mP_Str = other.mP_Str;

        other.m_Size = 0;
        other.m_Capacity = S_MaxSmallCapacity;
        other.mP_Str = nullptr;

        /* Ensure moved from object defaults to initial empty state... */
        other.m_InlineBuffer[0] = '\0';
    }

    void String::Append(const char* pStr) noexcept
    {
        const size_t appendLength = StringLength(pStr);
        const size_t prevSize = m_Size;
        const size_t newSize = m_Size + appendLength;

        if (newSize <= S_MaxSmallCapacity)
        {
            std::memcpy(m_InlineBuffer + prevSize, pStr, appendLength);
            m_Size = newSize;
            return;
        }
        else if (newSize <= m_Capacity)
        {
            std::memcpy(mP_Str + prevSize, pStr, appendLength);
            m_Size = newSize;
            return;
        }

        const size_t minCapacity = newSize;
        Resize(minCapacity);

        const size_t offset = prevSize;
        HeapCopyOffset(offset, pStr, appendLength);
        m_Size = newSize;
        mP_Str[newSize] = '\0';
    }

    void String::Resize(size_t newCapacity) noexcept
    {
        if (newCapacity <= m_Capacity)
            return;

        const size_t geometricGrowth = m_Size + (m_Size / 2);
        newCapacity = std::max(geometricGrowth, newCapacity);

        char* pNewStr = AllocCapacity(newCapacity);

        if (IsSmall() && !Empty())
            InlineCopyTo(pNewStr, newCapacity);
        else if (mP_Str)
        {
            HeapCopyTo(pNewStr, newCapacity);
            Dealloc();
        }

        const size_t prevSize = m_Size;

        mP_Str = pNewStr;
        m_Size = prevSize;
        m_Capacity = newCapacity;
    }

    String::~String() noexcept
    {
        if (!IsSmall())
            Dealloc();
    }

    [[nodiscard]] char* String::AllocCapacity(size_t capacity) noexcept
    {
        ASSERT(capacity > S_MaxSmallCapacity, "should be copied inline...");

        char* pStr = new char[capacity + 1];

        const size_t nullTermIndex = capacity;
        pStr[nullTermIndex] = '\0';

        return pStr;
    }

    void String::Dealloc() noexcept
    {
        ASSERT(!IsSmall(), "small active...");

        delete[] mP_Str;
        mP_Str = nullptr;
        m_Size = 0;
        m_Capacity = 0;
    }

    void String::InlineCopy(const char* pStr, size_t length) noexcept
    {
        ASSERT(IsSmall(), "non-small active...");
        ASSERT(length < sizeof(m_InlineBuffer), "provided length overflows inline buffer...");

        std::memcpy(m_InlineBuffer, pStr, length);
        m_InlineBuffer[length] = '\0';
        m_Size = length;
    }

    void String::InlineCopyTo(char* pDest, size_t destLength) const noexcept
    {
        ASSERT(IsSmall(), "small should be active...");
        ASSERT(destLength >= m_Size, "dest length doesn't suffice...");

        std::memcpy(pDest, m_InlineBuffer, m_Size);
    }

    void String::HeapCopy(const char* pStr, size_t length) noexcept
    {
        ASSERT(!IsSmall(), "small active...");
        ASSERT(mP_Str, "should be allocated...");
        ASSERT(length <= m_Capacity, "overflowed capacity...");

        std::memcpy(mP_Str, pStr, length);
        mP_Str[length] = '\0';
        m_Size = length;
    }

    void String::HeapCopyOffset(size_t offset, const char* pStr, size_t length) noexcept
    {
        ASSERT(!IsSmall(), "small active...");
        ASSERT(mP_Str, "should be allocated...");
        ASSERT(length <= m_Capacity, "overflowed capacity...");
        ASSERT(offset <= m_Capacity, "offset overflowed capacity...");

        char* pDest = mP_Str + offset;
        std::memcpy(pDest, pStr, length);
    }

    void String::HeapCopyTo(char* pDest, size_t destLength) const noexcept
    {
        ASSERT(!IsSmall(), "small active...");
        ASSERT(mP_Str, "should be allocated...");
        ASSERT(destLength >= m_Size, "dest length doesn't suffice...");

        std::memcpy(pDest, mP_Str, m_Size);
    }

    [[nodiscard]] const char* String::ActiveStorage() const noexcept
    {
        if (IsSmall())
            return m_InlineBuffer;
        
        return mP_Str;
    }

    [[nodiscard]] bool String::IsSmall() const noexcept
    {
        return m_Capacity <= S_MaxSmallCapacity;
    }
}
