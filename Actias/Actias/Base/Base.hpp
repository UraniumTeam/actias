#pragma once
#include <Actias/Base/Hash.hpp>
#include <Actias/System/Assert.h>
#include <Actias/System/Platform.h>
#include <atomic>
#include <cstdint>
#include <cstring>
#include <emmintrin.h>
#include <memory>
#include <string_view>
#include <xmmintrin.h>

namespace Actias
{
    //! \brief Empty structure with no members.
    struct EmptyStruct
    {
    };

    //! \brief Indicates the size of a pointer for an architecture.
    enum class ArchPointerSize
    {
        Arch32Bit = 32, //!< Indicates 32-bit pointer size.
        Arch64Bit = 64, //!< Indicates 64-bit pointer size.

#if ACTIAS_ARCH_64_BIT
        Current = Arch64Bit, //!< Current pointer size.
#else
        Current = Arch32Bit, //!< Current pointer size.
#endif

        Invalid = -1, //!< Indicates an invalid value.
    };

    //! \internal
    namespace Internal
    {
        //! \brief A simple `std::string_view` wrapper
        //!
        //! This is useful for function signatures when compiling with MSVC. `std::string_view` is a template class
        //! (`std::basic_string_view< ... >`). It makes difficult to retrieve typename from a template function
        //! signature since it makes more than one template.
        struct SVWrapper
        {
            std::string_view value; //!< Actual value of the string view.
        };

        //! \brief Remove leading and trailing spaces from a string view.
        inline constexpr std::string_view TrimTypeName(std::string_view name)
        {
            name.remove_prefix(name.find_first_not_of(' '));
            name.remove_suffix(name.length() - name.find_last_not_of(' ') - 1);
            return name;
        }

        //! \brief Internal implementation of Actias::TypeName.
        template<class T>
        inline constexpr SVWrapper TypeNameImpl()
        {
            // TODO: we can't use RTTI in ACBX modules because of this std::string_view
#if ACTIAS_COMPILER_MSVC
            std::string_view fn = __FUNCSIG__;
            fn.remove_prefix(fn.find_first_of("<") + 1);
            fn.remove_suffix(fn.length() - fn.find_last_of(">"));
#else
            std::string_view fn = __PRETTY_FUNCTION__;
            fn.remove_prefix(fn.find_first_of('=') + 1);
            fn.remove_suffix(fn.length() - fn.find_last_of(']'));
#endif
            constexpr std::string_view classPrefix = "class ";
            if (fn.substr(0, classPrefix.length()) == classPrefix)
            {
                fn.remove_prefix(classPrefix.length());
            }

            return SVWrapper{ TrimTypeName(fn) };
        }
    } // namespace Internal
    //! \endinternal

    //! \brief Get name of a type as a compile-time constant.
    //!
    //! This implementation uses the `__PRETTY_FUNCTION__` hack to retrieve typename from a function signature
    //! at compile-time.
    template<class T>
    inline constexpr std::string_view TypeName()
    {
        return Internal::TypeNameImpl<T>().value;
    }

    //! \brief Maximum alignment value, should be enough for anything
    inline constexpr USize MaximumAlignment = 16;

    //! \brief Align up an integer.
    //!
    //! \param x     - Value to align.
    //! \param align - Alignment to use.
    template<class T, class U = T>
    inline T AlignUp(T x, U align)
    {
        auto alignT = static_cast<T>(align);
        return (x + (alignT - 1u)) & ~(alignT - 1u);
    }

    //! \brief Align up a pointer.
    //!
    //! \param x     - Value to align.
    //! \param align - Alignment to use.
    template<class T>
    inline T* AlignUpPtr(const T* x, USize align)
    {
        return reinterpret_cast<T*>(AlignUp(reinterpret_cast<USize>(x), align));
    }

    //! \brief Align up an integer.
    //!
    //! \param x  - Value to align.
    //! \tparam A - Alignment to use.
    template<UInt32 A, class T>
    inline constexpr T AlignUp(T x)
    {
        return (x + (A - 1)) & ~(A - 1);
    }

    //! \brief Align down an integer.
    //!
    //! \param x     - Value to align.
    //! \param align - Alignment to use.
    template<class T, class U = T>
    inline T AlignDown(T x, U align)
    {
        return (x & ~(align - 1));
    }

    //! \brief Align down a pointer.
    //!
    //! \param x     - Value to align.
    //! \param align - Alignment to use.
    template<class T>
    inline constexpr T* AlignDownPtr(const T* x, USize align)
    {
        return reinterpret_cast<T*>(AlignDown(reinterpret_cast<USize>(x), align));
    }

    //! \brief Align down an integer.
    //!
    //! \param x  - Value to align.
    //! \tparam A - Alignment to use.
    template<UInt32 A, class T>
    inline constexpr T AlignDown(T x)
    {
        return (x & ~(A - 1));
    }

    //! \brief Create a bitmask.
    //!
    //! \param bitCount  - The number of ones in the created mask.
    //! \param leftShift - The number of zeros to the right of the created mask.
    template<class T>
    inline constexpr T MakeMask(T bitCount, T leftShift)
    {
        auto typeBitCount = sizeof(T) * 8;
        auto mask         = bitCount == typeBitCount ? static_cast<T>(-1) : ((1 << bitCount) - 1);
        return static_cast<T>(mask << leftShift);
    }

    template<class T>
    inline void Swap(T& lhs, T& rhs)
    {
        T temp{ std::move(lhs) };
        lhs = std::move(rhs);
        rhs = std::move(temp);
    }

    template<class T, USize N>
    inline void Swap(T (&a)[N], T (&b)[N])
    {
        for (USize i = 0; i < N; ++i)
        {
            Swap(a[i], b[i]);
        }
    }

#if __cpp_lib_assume_aligned
#    define ACTIAS_AssumeAligned(align, value) std::assume_aligned<align>(value)
#else
#    define ACTIAS_AssumeAligned(align, value) (value)
#endif
} // namespace Actias
