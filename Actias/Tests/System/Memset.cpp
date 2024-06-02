#include <Actias/System/Core.h>
#include <Actias/Time/DateTime.hpp>
#include <chrono>
#include <gtest/gtest.h>

inline constexpr USize OverflowCheckCount = 4;
inline constexpr USize BufferAlignment    = 256;
inline constexpr USize MaxMemcopyTestSize = 64 * 1024 * 1024;

alignas(32) static UInt8 g_Destination[MaxMemcopyTestSize + OverflowCheckCount];

using FMemset = decltype(ActiasSetMemory);

inline void TestMemset(USize bufferSize, FMemset* proc, UInt32 count = 1)
{
    const auto time = Actias::UTCDateTime::Now();
    srand(static_cast<unsigned int>(time.UnixTime()));

    UInt8 value = rand() % 0xFF;
    UInt8 guard = rand() % 0xFF;

    for (USize i = 0; i < OverflowCheckCount; ++i)
    {
        g_Destination[bufferSize + i] = guard;
    }

    for (UInt32 i = 0; i < count; ++i)
    {
        proc(g_Destination, value, bufferSize);
    }

    for (USize i = 0; i < bufferSize; ++i)
    {
        ASSERT_EQ(g_Destination[i], value) << "Size = " << bufferSize << " bytes; i = " << i;
    }

    for (USize i = 0; i < OverflowCheckCount; ++i)
    {
        ASSERT_EQ(g_Destination[bufferSize + i], guard) << "Overflow error; Size = " << bufferSize << "bytes; i = " << i;
    }
}

TEST(SetMemory, NormalSetSmall)
{
    for (USize i = 0; i < 1024; ++i)
    {
        TestMemset(i, ActiasSetMemory);
    }
}

TEST(SetMemory, InlineSetSmall)
{
    for (USize i = 0; i < 1024; ++i)
    {
        TestMemset(i, ActiasInlineSetMemory);
    }
}

TEST(SetMemory, NormalSet)
{
    for (USize i = 1024; i < 4096; ++i)
    {
        TestMemset(i, ActiasSetMemory);
    }
}

TEST(SetMemory, InlineSet)
{
    for (USize i = 1024; i < 4096; ++i)
    {
        TestMemset(i, ActiasInlineSetMemory);
    }
}

TEST(SetMemory, NormalSetLarge)
{
    TestMemset(MaxMemcopyTestSize, ActiasSetMemory, 128);
}

inline void ACTIAS_ABI StdlibMemset(void* dst, UInt8 value, USize byteCount)
{
    memset(dst, value, byteCount);
}

TEST(SetMemory, StdSetLarge)
{
    TestMemset(MaxMemcopyTestSize, ActiasInlineSetMemory, 128);
}

TEST(SetMemory, InlineSetLarge)
{
    TestMemset(MaxMemcopyTestSize, ActiasInlineSetMemory, 128);
}
