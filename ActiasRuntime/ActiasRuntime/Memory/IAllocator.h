#pragma once
#include <ActiasRuntime/RTTI/RTTI.h>
#include <ActiasRuntime/Base/Base.h>

namespace Actias
{
    //! \brief An interface for memory allocators.
    class IAllocator
    {
    public:
        ACTIAS_RTTI_Class(IAllocator, "3584BACB-1A4B-4492-9214-BF348D350474");

        //! \brief Allocate memory.
        //!
        //! \param size - Size of the allocation in bytes.
        //! \param alignment - Alignment of the allocation in bytes.
        //!
        //! \return Pointer to the allocated block of memory.
        virtual void* Allocate(USize size, USize alignment) = 0;

        //! \brief Deallocate memory.
        //!
        //! \param pointer - A pointer to the block of memory to deallocate.
        virtual void Deallocate(void* pointer) = 0;

        //! \brief Get debug name of the allocator.
        [[nodiscard]] virtual const char* GetName() const = 0;
    };
} // namespace Actias
