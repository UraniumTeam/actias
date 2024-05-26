#include <Actias/System/Core.h>
#include <Actias/Time/DateTime.hpp>
#include <chrono>
#include <gtest/gtest.h>

inline constexpr USize OverflowCheckCount = 4;
inline constexpr USize BufferAlignment    = 256;
inline constexpr USize MaxMemcopyTestSize = 64 * 1024 * 1024;

static UInt8 g_Source[MaxMemcopyTestSize + OverflowCheckCount];
alignas(32) static UInt8 g_Destination[MaxMemcopyTestSize + OverflowCheckCount];

using FMemcpy = decltype(ActiasCopyMemory);

inline void TestMemcopy(USize bufferSize, FMemcpy* proc, UInt32 count = 1)
{
    const auto time = Actias::UTCDateTime::Now();
    srand(static_cast<unsigned int>(time.UnixTime()));

    for (USize i = 0; i < bufferSize; ++i)
    {
        g_Source[i] = static_cast<UInt8>(rand() & 0xFF);
    }

    for (USize i = 0; i < OverflowCheckCount; ++i)
    {
        g_Source[bufferSize + i]      = 0xFF;
        g_Destination[bufferSize + i] = 0;
    }

    for (UInt32 i = 0; i < count; ++i)
    {
        proc(g_Destination, g_Source, bufferSize);
    }

    for (USize i = 0; i < bufferSize; ++i)
    {
        ASSERT_EQ(g_Destination[i], g_Source[i]) << "Size = " << bufferSize << " bytes; i = " << i;
    }

    for (USize i = 0; i < OverflowCheckCount; ++i)
    {
        ASSERT_EQ(g_Destination[bufferSize + i], 0) << "Overflow error; Size = " << bufferSize << "bytes; i = " << i;
    }
}

TEST(CopyMemory, NormalCopySmall)
{
    for (USize i = 0; i < 1024; ++i)
    {
        TestMemcopy(i, ActiasCopyMemory);
    }
}

TEST(CopyMemory, InlineCopySmall)
{
    for (USize i = 0; i < 1024; ++i)
    {
        TestMemcopy(i, ActiasInlineCopyMemory);
    }
}

TEST(CopyMemory, NormalCopy)
{
    for (USize i = 1024; i < 4096; ++i)
    {
        TestMemcopy(i, ActiasCopyMemory);
    }
}

TEST(CopyMemory, AlignedCopy)
{
    for (USize i = 1024; i < 4096; i += BufferAlignment)
    {
        TestMemcopy(i, ActiasCopyAlignedMemory);
    }
}

TEST(CopyMemory, Stream)
{
    for (USize i = 1024; i < 4096; i += BufferAlignment)
    {
        TestMemcopy(i, ActiasStreamMemory);
    }
}

TEST(CopyMemory, InlineCopy)
{
    for (USize i = 1024; i < 4096; ++i)
    {
        TestMemcopy(i, ActiasInlineCopyMemory);
    }
}

TEST(CopyMemory, NormalCopyLarge)
{
    TestMemcopy(MaxMemcopyTestSize, ActiasCopyMemory, 128);
}

TEST(CopyMemory, AlignedCopyLarge)
{
    TestMemcopy(MaxMemcopyTestSize, ActiasCopyAlignedMemory, 128);
}

TEST(CopyMemory, StreamLarge)
{
    TestMemcopy(MaxMemcopyTestSize, ActiasStreamMemory, 128);
}

TEST(CopyMemory, InlineCopyLarge)
{
    TestMemcopy(MaxMemcopyTestSize, ActiasInlineCopyMemory, 128);
}
