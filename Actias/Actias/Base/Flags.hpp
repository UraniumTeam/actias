#pragma once
#include <Actias/Base/Base.hpp>

namespace Actias
{
//! \brief Create a single-bit literal.
#define ACTIAS_BIT(n) (1 << (n))

    //! \brief Cast an enum to underlying type.
    template<class TEnum>
    inline constexpr std::enable_if_t<std::is_enum_v<TEnum>, std::underlying_type_t<TEnum>> ac_enum_cast(TEnum value)
    {
        return static_cast<std::underlying_type_t<TEnum>>(value);
    }

    //! \brief Cast an enum to flags type.
    template<class TFlagsEnum, class TEnum>
    inline constexpr std::enable_if_t<std::is_enum_v<TEnum>, TFlagsEnum> ac_flag_cast(TEnum value)
    {
        return static_cast<TFlagsEnum>(1 << ac_enum_cast(value));
    }

//! \brief Define bitwise operations on `enum`.
//!
//! The macro defines bitwise or, and, xor operators.
#define ACTIAS_ENUM_OPERATORS(Name)                                                                                                  \
    inline constexpr Name operator|(Name a, Name b)                                                                              \
    {                                                                                                                            \
        return static_cast<Name>(ac_enum_cast(a) | ac_enum_cast(b));                                                             \
    }                                                                                                                            \
    inline constexpr Name& operator|=(Name& a, Name b)                                                                           \
    {                                                                                                                            \
        return a = a | b;                                                                                                        \
    }                                                                                                                            \
    inline constexpr Name operator&(Name a, Name b)                                                                              \
    {                                                                                                                            \
        return static_cast<Name>(ac_enum_cast(a) & ac_enum_cast(b));                                                             \
    }                                                                                                                            \
    inline constexpr Name& operator&=(Name& a, Name b)                                                                           \
    {                                                                                                                            \
        return a = a & b;                                                                                                        \
    }                                                                                                                            \
    inline constexpr Name operator^(Name a, Name b)                                                                              \
    {                                                                                                                            \
        return static_cast<Name>(ac_enum_cast(a) ^ ac_enum_cast(b));                                                             \
    }                                                                                                                            \
    inline constexpr Name& operator^=(Name& a, Name b)                                                                           \
    {                                                                                                                            \
        return a = a ^ b;                                                                                                        \
    }                                                                                                                            \
    inline constexpr Name operator~(Name a)                                                                                      \
    {                                                                                                                            \
        return static_cast<Name>(~ac_enum_cast(a));                                                                              \
    }

    //! \internal
    namespace Internal
    {
        template<class T>
        inline constexpr bool IsAllowedAsFlags = std::is_enum_v<T> | std::is_integral_v<T>;
    }

    //! \brief Check if any flags from test are active in source.
    template<class TFlags>
    inline constexpr std::enable_if_t<Internal::IsAllowedAsFlags<TFlags>, bool> AnyFlagsActive(TFlags source, TFlags test)
    {
        return (source & test) != static_cast<TFlags>(0);
    }

    //! \brief Check if all flags from test are active in source.
    template<class TFlags>
    inline constexpr std::enable_if_t<Internal::IsAllowedAsFlags<TFlags>, bool> AllFlagsActive(TFlags source, TFlags test)
    {
        return (source & test) == test;
    }

    //! \brief Remove some bits from flags.
    template<class TFlags>
    inline constexpr std::enable_if_t<Internal::IsAllowedAsFlags<TFlags>, TFlags> RemoveFlags(TFlags source, TFlags remove)
    {
        return source & ~remove;
    }
} // namespace Actias
