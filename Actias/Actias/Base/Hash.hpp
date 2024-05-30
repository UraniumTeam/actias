#pragma once
#include <Actias/Base/wyhash.h>
#include <Actias/System/Platform.h>
#include <type_traits>

namespace Actias
{
    template<class T>
    struct Hash final
    {
        inline std::enable_if_t<std::is_pod_v<T>, UInt64> operator()(const T& value) const noexcept
        {
            return wyhash(&value, sizeof(T), 0, _wyp);
        }
    };

    inline void HashCombine(UInt64& /* seed */) {}

    //! \brief Combine hashes of specified values with seed.
    //!
    //! \tparam Args - Types of values.
    //!
    //! \param seed - Initial hash value to combine with.
    //! \param args - The values to calculate hash of.
    template<typename T, typename... Args>
    inline void HashCombine(UInt64& seed, const T& value, const Args&... args)
    {
        Hash<T> hasher;
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        HashCombine(seed, args...);
    }

    //! \brief Define Actias::Hash<T> for a type.
#define ACTIAS_MAKE_HASHABLE(TypeName, Template, ...)                                                                            \
    template<Template>                                                                                                           \
    struct Hash<TypeName>                                                                                                        \
    {                                                                                                                            \
        inline UInt64 operator()(const TypeName& value) const noexcept                                                           \
        {                                                                                                                        \
            UInt64 seed = 0;                                                                                                     \
            ::Actias::HashCombine(seed, __VA_ARGS__);                                                                            \
            return seed;                                                                                                         \
        }                                                                                                                        \
    };
} // namespace Actias
