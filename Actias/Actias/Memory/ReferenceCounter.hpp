#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/Memory/IAllocator.hpp>
#include <Actias/RTTI/RTTI.hpp>
#include <Actias/System/Atomic.h>

namespace Actias
{
    class IObject;

    //! \brief The reference counter that holds the number of references to the object.
    //!
    //! This class holds number of references to the object and a pointer to the allocator that
    //! was used to allocate this object. It assumes the following memory layout:
    //! \code
    //!     +------------------+ <--- this pointer
    //!     | ReferenceCounter |
    //!     --------------------
    //!     |      Object      |
    //!     +------------------+
    //! \endcode
    //!
    //! It will delete `this` assuming that a single block was used to allocate the object and the counter.\n
    //!
    //! Example (pseudo-code):
    //! \code{.cpp}
    //!     class MyObject : Object<IObject> {};
    //!
    //!     ReferenceCounter* rc = ActiasAlloc(sizeof(ReferenceCounter) + sizeof(MyObject));
    //!     MyObject* obj        = rc + sizeof(ReferenceCounter);
    //!     // ...
    //!     ActiasFree(rc); // frees memory of both the counter and the object.
    //! \endcode
    class ReferenceCounter final
    {
        ActiasAtomic32 m_StrongRefCount = { 0 };
        mutable IAllocator* m_pAllocator;

    public:
        ACTIAS_RTTI_Struct(ReferenceCounter, "CEDB61B5-D75B-4D08-BE6F-E1C11806989B");

        //! \brief Create a new reference counter with specified allocator.
        //!
        //! The specified allocator will be used to free memory after the counter reaches zero.
        //! This constructor initializes the counter to _zero_.
        //!
        //! \param pAllocator - The allocator to use to free memory.
        inline explicit ReferenceCounter(IAllocator* pAllocator)
            : m_pAllocator(pAllocator)
        {
        }

        //! \brief Add a strong reference to the counter.
        //!
        //! \return The new (incremented) number of strong references.
        inline UInt32 AddStrongRef()
        {
            return ActiasAtomicFetchAdd32(&m_StrongRefCount, 1) + 1;
        }

        //! \brief Remove a strong reference from the counter.
        //!
        //! This function will delete the counter itself if number of references reaches zero.
        //!
        //! \param destroyCallback - A function to invoke _before_ deallocation if the counter reached zero.
        //!                          This is typically a lambda that calls object destructor.
        //! \tparam F - Type of callback function.
        //!
        //! \return The new (decremented) number of strong references.
        template<class F>
        inline UInt32 ReleaseStrongRef(F&& destroyCallback)
        {
            UInt32 refCount = ActiasAtomicFetchAdd32(&m_StrongRefCount, -1) - 1;
            if (refCount == 0)
            {
                destroyCallback();
                m_pAllocator->Deallocate(this);
            }

            return refCount;
        }

        //! \brief Get number of strong references.
        inline UInt32 GetStrongRefCount() const
        {
            return ActiasAtomicLoad32Relaxed(&m_StrongRefCount);
        }
    };
} // namespace Actias
