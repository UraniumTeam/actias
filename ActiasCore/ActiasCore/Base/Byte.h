#pragma once
#include <ActiasCore/Base/Flags.h>

namespace Actias
{
    //! \brief A type-safe integral with size of a single byte.
    enum class Byte : UInt8
    {
    };

    static_assert(sizeof(Byte) == 1);

    //! \brief Cast a pointer to the pointer to Byte.
    template<class T>
    inline std::enable_if_t<std::is_integral_v<T>, Byte*> actias_byte_cast(T* ptr)
    {
        return reinterpret_cast<Byte*>(ptr);
    }

    //! \brief Cast a pointer to the pointer to Byte.
    template<class T>
    inline std::enable_if_t<std::is_integral_v<T>, const Byte*> actias_byte_cast(const T* ptr)
    {
        return reinterpret_cast<const Byte*>(ptr);
    }
} // namespace Actias
