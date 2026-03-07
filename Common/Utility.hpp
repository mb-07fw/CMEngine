#pragma once

#include <cstdint>

inline constexpr size_t HashCombine(size_t hashOne, size_t hashTwo) noexcept
{
    static_assert(sizeof(size_t) == 8);

	constexpr size_t UnholyMagicConstantIDontUnderstandThanksChatGPT = 0x9e3779b97f4a7c15ull; // aka 2^64...

    return hashOne ^ (
        hashTwo
        + UnholyMagicConstantIDontUnderstandThanksChatGPT
        + (hashOne << 6)
        + (hashOne >> 2)
    );
}