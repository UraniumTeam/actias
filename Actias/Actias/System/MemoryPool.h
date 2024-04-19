/*
 * Copyright 2024 the Actias project authors (see the file AUTHORS
 * for a complete list)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <Actias/System/Memory.h>

ACTIAS_BEGIN_C

typedef struct ActiasMemoryPoolDesc
{
    UInt64 ElementByteSize;
    UInt32 ElementsInPage;
} ActiasMemoryPoolDesc;

typedef struct ActiasMemoryPoolPage
{
    struct ActiasMemoryPoolPage* pNext;
    void* pCurrent;
} ActiasMemoryPoolPage;

typedef struct ActiasMemoryPool
{
    UInt64 ElementByteSize;
    UInt64 PageByteSize;
    ActiasMemoryPoolPage* pPageList;
    void* pFreeList;
} ActiasMemoryPool;

inline static void ACTIAS_ABI ActiasCreateMemoryPool(const ActiasMemoryPoolDesc* pDesc, ActiasMemoryPool* pMemoryPool)
{
    pMemoryPool->ElementByteSize = ActiasAlignUp(pDesc->ElementByteSize, ACTIAS_DEFAULT_ALIGNMENT);
    pMemoryPool->PageByteSize    = pDesc->ElementsInPage * pMemoryPool->ElementByteSize + sizeof(ActiasMemoryPoolPage);
    pMemoryPool->pPageList       = NULL;
    pMemoryPool->pFreeList       = NULL;
}

inline static void ACTIAS_ABI ActiasDestroyMemoryPool(ActiasMemoryPool* pMemoryPool)
{
    ActiasMemoryPoolPage* pPage = pMemoryPool->pPageList;
    while (pPage)
    {
        ActiasMemoryPoolPage* pNext = pPage->pNext;
        ActiasAlignedFree(pPage);
        pPage = pNext;
    }
}

inline static ActiasMemoryPoolPage* ACTIAS_ABI ActiasMemoryPoolAllocatePage(ActiasMemoryPool* pMemoryPool)
{
    ActiasMemoryPoolPage* pResult =
        (ActiasMemoryPoolPage*)ActiasAlignedAlloc(ACTIAS_DEFAULT_ALIGNMENT, pMemoryPool->PageByteSize);

    pResult->pNext         = pMemoryPool->pPageList;
    pResult->pCurrent      = pResult + 1;
    pMemoryPool->pPageList = pResult;
    return pResult;
}

inline static void* ACTIAS_ABI ActiasMemoryPoolAllocateFromPage(ActiasMemoryPool* pMemoryPool, ActiasMemoryPoolPage* pPage)
{
    if (!pPage)
    {
        return NULL;
    }

    UInt8* pResult  = (UInt8*)pPage->pCurrent;
    UInt8* pPageEnd = (UInt8*)pPage + pMemoryPool->PageByteSize;

    if (pResult >= pPageEnd)
    {
        return NULL;
    }

    pPage->pCurrent = pResult + pMemoryPool->ElementByteSize;
    return pResult;
}

inline static void* ACTIAS_ABI ActiasMemoryPoolAllocate(ActiasMemoryPool* pMemoryPool)
{
    void* pResult;
    if (!pMemoryPool->pFreeList)
    {
        pResult = ActiasMemoryPoolAllocateFromPage(pMemoryPool, pMemoryPool->pPageList);
        if (!pResult)
        {
            ActiasMemoryPoolPage* pPage = ActiasMemoryPoolAllocatePage(pMemoryPool);

            pResult = ActiasMemoryPoolAllocateFromPage(pMemoryPool, pPage);
        }

        return pResult;
    }

    pResult                = pMemoryPool->pFreeList;
    pMemoryPool->pFreeList = *(void**)pResult;
    return pResult;
}

inline static void ACTIAS_ABI ActiasMemoryPoolFree(ActiasMemoryPool* pMemoryPool, void* p)
{
    *(void**)p             = pMemoryPool->pFreeList;
    pMemoryPool->pFreeList = p;
}

ACTIAS_END_C
