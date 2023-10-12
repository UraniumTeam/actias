#pragma once
#include <Actias/Base/Base.h>
#include <Actias/Utils/UUID.h>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace Actias
{
    using TypeID = UUID;

    //! \brief Define common RTTI functions for a struct.
    //!
    //! Same as ACTIAS_RTTI_Class, but for `final` classes that don't inherit from other classes.\n
    //! Use this whenever possible, because ACTIAS_RTTI_Class implements virtual functions.
    //!
    //! \see ACTIAS_RTTI_Class
#define ACTIAS_RTTI_Struct(name, uuid)                                                                                           \
    inline void ActiasRTTI_Checks()                                                                                              \
    {                                                                                                                            \
        using ThisType = std::remove_reference_t<decltype(*this)>;                                                               \
        static_assert(std::is_same_v<name, ThisType>);                                                                           \
    }                                                                                                                            \
                                                                                                                                 \
    inline static const ::Actias::TypeID& ActiasRTTI_GetSID()                                                                    \
    {                                                                                                                            \
        static ::Actias::TypeID id = ::Actias::TypeID(uuid);                                                                     \
        return id;                                                                                                               \
    }                                                                                                                            \
                                                                                                                                 \
    inline static std::string_view ActiasRTTI_GetSName()                                                                         \
    {                                                                                                                            \
        return ::Actias::TypeName<name>();                                                                                       \
    }

    //! \brief Define common RTTI functions for a class.
    //!
    //! This macro is a part of RTTI implementation.
    //! Use it to allow `un_dynamic_cast` and dependency injection.\n
    //!
    //! Example:
    //! \code{.cpp}
    //!     class Foo
    //!     {
    //!     public:
    //!         ACTIAS_RTTI_Class(Foo, "12CED1D1-6337-443F-A854-B4624A6133AE");
    //!         // members ...
    //!     };
    //! \endcode
    //!
    //! \note Use ACTIAS_RTTI_Struct for `final` structs that don't inherit from any other class.
#define ACTIAS_RTTI_Class(name, uuid)                                                                                            \
    ACTIAS_RTTI_Struct(name, uuid);                                                                                              \
    ACTIAS_PUSH_CLANG_WARNING("-Winconsistent-missing-override")                                                                 \
    inline virtual std::string_view ActiasRTTI_GetName() const                                                                   \
    {                                                                                                                            \
        return ::Actias::TypeName<name>();                                                                                       \
    }                                                                                                                            \
                                                                                                                                 \
    inline virtual const ::Actias::TypeID& ActiasRTTI_GetID() const                                                              \
    {                                                                                                                            \
        return name ::ActiasRTTI_GetSID();                                                                                       \
    }                                                                                                                            \
                                                                                                                                 \
    template<class ActiasRTTI_IS_TYPE>                                                                                           \
    inline bool ActiasRTTI_Is() const                                                                                            \
    {                                                                                                                            \
        return ActiasRTTI_IS_TYPE::ActiasRTTI_GetSID() == ActiasRTTI_GetID();                                                    \
    }                                                                                                                            \
    ACTIAS_POP_CLANG_WARNING

    //! \brief Cast a pointer to a base class to a derived class pointer if possible.
    //!
    //! Works just like normal `dynamic_cast<T*>`, except it uses only the classes that provide RTTI
    //! through ACTIAS_RTTI_Class.
    //!
    //! \tparam TDstPtr - Type of return value, e.g. `DerivedClass*`, _must_ be a pointer.
    //! \tparam TSrc    - Type of source value, e.g. `BaseClass`, _must not_ be a pointer.
    //! \param src - The source value.
    //!
    //! \return The result pointer if destination type was derived from source type, `nullptr` otherwise.
    template<class TDstPtr, class TSrc, std::enable_if_t<std::is_base_of_v<TSrc, std::remove_pointer_t<TDstPtr>>, bool> = true>
    inline TDstPtr ac_dynamic_cast(TSrc* src)
    {
        if (src->template ActiasRTTI_Is<std::remove_pointer_t<TDstPtr>>())
            return static_cast<TDstPtr>(src);

        return nullptr;
    }

    //! \brief Assert that a variable can be dynamically casted to another type.
    //!
    //! Works just like ac_dynamic_cast<T*>, except it will assert that a type can be casted and won't return
    //! a `nullptr`. Use this when you're certainly sure that you can use `static_cast` here, but want to check it
    //! in debug builds. In release builds, when assertions are disabled, this can lead to undefined behavior.
    //!
    //! \note The function only works with the classes that provide RTTI through ACTIAS_RTTI_Class.
    //!
    //! \tparam TDstPtr - Type of return value, e.g. `DerivedClass*`, _must_ be a pointer.
    //! \tparam TSrc    - Type of source value, e.g. `BaseClass`, _must not_ be a pointer.
    //! \param src - The source value.
    //!
    //! \return The result pointer if destination type was derived from source type.
    template<class TDstPtr, class TSrc, std::enable_if_t<std::is_base_of_v<TSrc, std::remove_pointer_t<TDstPtr>>, bool> = true>
    inline TDstPtr ac_assert_cast(TSrc* src)
    {
        assert(src->template ActiasRTTI_Is<std::remove_pointer_t<TDstPtr>>() && "Assert cast failed");
        return static_cast<TDstPtr>(src);
    }

    //! \brief Get name of a type.
    //!
    //! This function returns a short name provided in ACTIAS_RTTI_Class or ACTIAS_RTTI_Struct.\n
    //! Example:
    //! \code{.cpp}
    //!     template<class T>
    //!     class Foo
    //!     {
    //!         ACTIAS_RTTI_Class(Foo, "4BDF1868-0E22-48CF-9DBA-8DD10F2A9D0C");
    //!         // members...
    //!     };
    //!
    //!     auto fooName = ac_nameof<Foo<int>>(); // "Foo" - not "Foo<int>"!
    //! \endcode
    //!
    //! \note The provided type must implement RTTI through ACTIAS_RTTI_Class or ACTIAS_RTTI_Struct.
    //!
    //! \tparam T - Type to get the name of.
    //!
    //! \return Short name of type T.
    template<class T>
    inline std::string_view ac_nameof() noexcept
    {
        return T::ActiasRTTI_GetSName();
    }

    //! \brief Get name of a type.
    //!
    //! This functions is same as ac_nameof(), but can return name of derived class if called from a base class.\n
    //! Returns a short name provided in ACTIAS_RTTI_Class or ACTIAS_RTTI_Struct.\n
    //! Example:
    //! \code{.cpp}
    //!     class Base
    //!     {
    //!     public:
    //!         ACTIAS_RTTI_Class(Base, "AB26B8C7-827F-4212-88B4-F71A5EFD6EEB");
    //!     };
    //!     class Derived : public Base
    //!     {
    //!     public:
    //!         ACTIAS_RTTI_Class(Derived, "68CCD7DF-507F-4F3B-9EC3-001EEB33EB55");
    //!     };
    //!
    //!     auto derivedName = ac_nameof(*static_cast<Base*>(new Derived)); // "Derived"
    //! \endcode
    //!
    //! For additional information see overload of ac_nameof() without parameters.
    //!
    //! \note The provided type must implement RTTI through ACTIAS_RTTI_Class or ACTIAS_RTTI_Struct.
    //!
    //! \tparam T - Type to get name of.
    //!
    //! \return Short name of type T.
    //!
    //! \see ac_nameof()
    template<class T>
    inline std::string_view ac_nameof(const T& object)
    {
        return object.ActiasRTTI_GetName();
    }

    template<class T>
    inline const UUID& ac_typeid() noexcept
    {
        return T::ActiasRTTI_GetSID();
    }

    template<class T>
    inline const UUID& ac_typeid(const T& object)
    {
        return object.ActiasRTTI_GetID();
    }
} // namespace Actias
