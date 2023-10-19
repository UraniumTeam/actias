#pragma once
#include <Actias/Memory/IAllocator.hpp>
#include <Actias/RTTI/RTTI.hpp>
#include <Actias/System/Memory.h>
#include <atomic>

namespace Actias
{
    //! \brief This allocator uses platform-specific aligned versions of malloc() and free().
    class SystemAllocator : public IAllocator
    {
        static SystemAllocator m_Instance;

    public:
        ACTIAS_RTTI_Class(SystemAllocator, "6C2B53D6-3A9A-447F-A127-1052253189C4");

        void* Allocate(USize size, USize alignment) override;
        void Deallocate(void* pointer) override;
        [[nodiscard]] const char* GetName() const override;

        //! \brief Get global static instance of the system allocator.
        inline static SystemAllocator* Get()
        {
            return &m_Instance;
        }
    };

    inline SystemAllocator SystemAllocator::m_Instance;

    inline void* SystemAllocator::Allocate(USize size, USize alignment)
    {
        return ActiasAlignedAlloc(alignment, size);
    }

    inline void SystemAllocator::Deallocate(void* pointer)
    {
        return ActiasAlignedFree(pointer);
    }

    inline const char* SystemAllocator::GetName() const
    {
        return "System allocator";
    }
} // namespace Actias
