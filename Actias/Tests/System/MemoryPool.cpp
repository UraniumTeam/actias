#include <Actias/Containers/List.hpp>
#include <Actias/Memory/MemoryPool.hpp>
#include <algorithm>
#include <gtest/gtest.h>
#include <random>

TEST(MemoryPool, AllocOnePage)
{
    ActiasMemoryPool pool;
    ActiasMemoryPoolDesc desc;
    desc.ElementByteSize = sizeof(USize);
    desc.ElementsInPage  = 4;

    ActiasCreateMemoryPool(&desc, &pool);

    // 16-byte alignment
    ASSERT_EQ(pool.ElementByteSize, 16);
    ASSERT_EQ(pool.PageByteSize, 16 * 4 + sizeof(ActiasMemoryPoolPage));

    USize* p = (USize*)ActiasMemoryPoolAllocate(&pool);
    *p       = 123;
    ActiasMemoryPoolFree(&pool, p);

    ASSERT_EQ(*p, 0);

    USize* pPrev = p;

    p  = (USize*)ActiasMemoryPoolAllocate(&pool);
    *p = 123;

    USize* pNext = (USize*)ActiasMemoryPoolAllocate(&pool);

    ActiasMemoryPoolFree(&pool, p);
    ActiasMemoryPoolFree(&pool, pNext);

    ASSERT_EQ(*p, 0);
    ASSERT_EQ(pPrev, p);
    ASSERT_EQ((UInt8*)p + pool.ElementByteSize, (UInt8*)pNext);

    ActiasDestroyMemoryPool(&pool);
}

TEST(MemoryPool, AllocMultiPage)
{
    ActiasMemoryPool pool;
    ActiasMemoryPoolDesc desc;
    desc.ElementByteSize = sizeof(USize);
    desc.ElementsInPage  = 4;

    ActiasCreateMemoryPool(&desc, &pool);

    Actias::List<USize*> allocations;
    for (USize i = 0; i < 64; ++i)
    {
        allocations.Push((USize*)ActiasMemoryPoolAllocate(&pool));
    }

    for (USize i = 0; i < allocations.Size(); ++i)
    {
        for (USize j = 0; j < allocations.Size(); ++j)
        {
            if (i == j)
            {
                continue;
            }

            ASSERT_NE(allocations[i], allocations[j]);
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allocations.begin(), allocations.end(), g);

    USize* pLastFree = NULL;
    for (USize* p : allocations)
    {
        pLastFree = p;
        ActiasMemoryPoolFree(&pool, p);
    }

    USize* ptr = (USize*)ActiasMemoryPoolAllocate(&pool);
    ASSERT_EQ(ptr, pLastFree);
    ActiasMemoryPoolFree(&pool, ptr);

    ActiasDestroyMemoryPool(&pool);
}

TEST(MemoryPool, CppWrapper)
{
    struct Dummy
    {
        UInt64 data[7];
    };

    Actias::PoolAllocator pool;
    pool.Init<Dummy>(128);

    Dummy* p = Actias::Memory::New<Dummy>(&pool);
    Actias::Memory::Delete(&pool, p);
}
