#pragma once
#include <Actias/Base/Flags.h>
#include <iomanip>

namespace Actias
{
    //! \brief A type-safe integral with size of a single byte, used to store raw memory as ArraySlice<T> or HeapArray<T>.
    enum class Byte : UInt8
    {
    };

    static_assert(sizeof(Byte) == 1);

    //! \brief Cast a pointer to the pointer to Byte.
    template<class T>
    inline std::enable_if_t<std::is_integral_v<T> || std::is_void_v<T>, Byte*> ac_byte_cast(T* ptr)
    {
        return reinterpret_cast<Byte*>(ptr);
    }

    //! \brief Cast a pointer to the pointer to Byte.
    template<class T>
    inline std::enable_if_t<std::is_integral_v<T> || std::is_void_v<T>, const Byte*> ac_byte_cast(const T* ptr)
    {
        return reinterpret_cast<const Byte*>(ptr);
    }

    template<class TOStream>
    inline TOStream& operator<<(TOStream& stream, Byte byte)
    {
        return stream << "0x" << std::setfill('0') << std::setw(2) << std::right << std::hex << static_cast<Int32>(byte);
    }
} // namespace Actias
