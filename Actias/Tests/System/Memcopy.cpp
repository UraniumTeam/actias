#include <Actias/System/Core.h>
#include <Actias/Time/DateTime.hpp>
#include <gtest/gtest.h>

inline constexpr USize OverflowCheckCount = 4;
inline constexpr USize BufferAlignment    = 256;
inline constexpr USize MaxMemcopyTestSize = 64 * 1024;

static alignas(BufferAlignment) UInt8 g_Source[MaxMemcopyTestSize + OverflowCheckCount];
static alignas(BufferAlignment) UInt8 g_Destination[MaxMemcopyTestSize + OverflowCheckCount];

using FMemcpy = decltype(ActiasCopyMemory);

inline void TestMemcopy(USize bufferSize, FMemcpy* proc)
{
    const auto time = Actias::DateTime::UtcNow();
    srand(static_cast<unsigned int>(time.Ticks()));

    for (USize i = 0; i < bufferSize; ++i)
    {
        g_Source[i] = static_cast<UInt8>(rand() & 0xFF);
    }

    for (USize i = 0; i < OverflowCheckCount; ++i)
    {
        g_Source[bufferSize + i]      = 0xFF;
        g_Destination[bufferSize + i] = 0;
    }

    proc(g_Destination, g_Source, bufferSize);

    for (USize i = 0; i < bufferSize; ++i)
    {
        ASSERT_EQ(g_Destination[i], g_Source[i])
            << "Size = " << bufferSize << " bytes; i = " << i;
    }

    for (USize i = 0; i < OverflowCheckCount; ++i)
    {
        ASSERT_EQ(g_Destination[bufferSize + i], 0)
            << "Overflow error; Size = " << bufferSize << "bytes; i = " << i;
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

TEST(CopyMemory, InlineCopy)
{
    for (USize i = 1024; i < 4096; ++i)
    {
        TestMemcopy(i, ActiasInlineCopyMemory);
    }
}

TEST(CopyMemory, NormalCopyLarge)
{
    for (USize i = 0; i < 4096; ++i)
    {
        TestMemcopy(MaxMemcopyTestSize, ActiasCopyMemory);
    }
}

TEST(CopyMemory, AlignedCopyLarge)
{
    for (USize i = 0; i < 4096; ++i)
    {
        TestMemcopy(MaxMemcopyTestSize, ActiasCopyAlignedMemory);
    }
}

TEST(CopyMemory, InlineCopyLarge)
{
    for (USize i = 0; i < 4096; ++i)
    {
        TestMemcopy(MaxMemcopyTestSize, ActiasInlineCopyMemory);
    }
}
