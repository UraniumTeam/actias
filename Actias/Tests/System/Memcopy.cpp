#include <Actias/System/Core.h>
#include <Actias/Time/DateTime.hpp>
#include <gtest/gtest.h>

inline constexpr USize MaxMemcopyTestSize = 16 * 1024;

static alignas(32) Int32 g_Source[MaxMemcopyTestSize];
static alignas(32) Int32 g_Destination[MaxMemcopyTestSize];

using FMemcpy = decltype(ActiasCopyMemory);

inline void TestMemcopy(USize bufferSize, FMemcpy* proc)
{
    auto time = Actias::DateTime::UtcNow();
    srand(static_cast<unsigned int>(time.Ticks()));

    for (USize i = 0; i < bufferSize; ++i)
    {
        g_Source[i] = rand();
    }

    proc(g_Destination, g_Source, bufferSize * 4);

    for (USize i = 0; i < bufferSize; ++i)
    {
        ASSERT_EQ(g_Destination[i], g_Source[i]) << "Size = " << bufferSize << "; i = " << i;
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
    for (USize i = 1024; i < 4096; i += 32)
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
