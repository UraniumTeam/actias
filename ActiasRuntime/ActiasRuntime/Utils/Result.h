#pragma once
#include <ActiasRuntime/RTTI/RTTI.h>
#include <variant>

namespace Actias
{
    template<class T>
    struct Err
    {
        T Value;

        inline Err(const T& value) // NOLINT(google-explicit-constructor)
            : Value(value)
        {
        }

        inline Err(T&& value) // NOLINT(google-explicit-constructor)
            : Value(value)
        {
        }
    };

    template<class T, class TError>
    class Result final
    {
        std::variant<T, TError> m_Data;

        struct CreateFromError
        {
        };

        inline Result(CreateFromError, const TError& error)
            : m_Data(error)
        {
        }

        inline Result(CreateFromError, TError&& error)
            : m_Data(std::move(error))
        {
        }

    public:
        ACTIAS_RTTI_Struct(Result, "87C787E0-D455-4480-8BB8-E7ACAD4738ED");

        inline Result() = default;

        inline Result(const T& value) // NOLINT(google-explicit-constructor)
            : m_Data(value)
        {
        }

        inline Result(T&& value) // NOLINT(google-explicit-constructor)
            : m_Data(std::move(value))
        {
        }

        inline Result(const Err<TError>& error) // NOLINT(google-explicit-constructor)
            : m_Data(error.Value)
        {
        }

        inline Result(Err<TError>&& error) // NOLINT(google-explicit-constructor)
            : m_Data(std::move(error.Value))
        {
        }

        inline Result(const Result& other)     = default;
        inline Result(Result&& other) noexcept = default;

        inline Result& operator=(const Result& other)     = default;
        inline Result& operator=(Result&& other) noexcept = default;

        inline static Result Ok(const T& value)
        {
            return Result(value);
        }

        inline static Result Ok(T&& value)
        {
            return Result(std::move(value));
        }

        inline static Result Err(const TError& error)
        {
            return Result(CreateFromError{}, error);
        }

        inline static Result Err(TError&& error)
        {
            return Result(CreateFromError{}, std::move(error));
        }

        [[nodiscard]] inline bool IsOk() const
        {
            return m_Data.index() == 0;
        }

        [[nodiscard]] inline bool IsErr() const
        {
            return !IsOk();
        }

        inline TError UnwrapErr() const
        {
            return ExpectErr("UnwrapErr() called on OK result");
        }

        inline TError ExpectErr([[maybe_unused]] const char* msg) const
        {
            ACTIAS_Assert(IsErr(), msg);
            return std::get<TError>(m_Data);
        }

        [[nodiscard]] inline explicit operator bool() const
        {
            return IsOk();
        }

        [[nodiscard]] inline T UnwrapOr(const T& defaultValue) const
        {
            if (IsOk())
            {
                return std::get<T>(m_Data);
            }

            return defaultValue;
        }

        [[nodiscard]] inline T UnwrapOrDefault() const
        {
            if (IsOk())
            {
                return std::get<T>(m_Data);
            }

            return {};
        }

        inline T Unwrap() const
        {
            return Expect("Unwrap() called on error result");
        }

        inline T Expect([[maybe_unused]] const char* msg) const
        {
            ACTIAS_Assert(IsOk(), msg);
            return std::get<T>(m_Data);
        }

        template<class F>
        inline T UnwrapOrElse(F&& f) const&
        {
            return IsOk() ? std::get<T>(m_Data) : std::invoke(std::forward<F>(f), std::get<TError>(m_Data));
        }

        template<class F>
        inline T UnwrapOrElse(F&& f) &&
        {
            return IsOk() ? std::get<T>(m_Data)
                          : std::invoke(std::forward<F>(f), static_cast<TError&&>(std::get<TError>(m_Data)));
        }

        template<class F>
        inline constexpr auto OrElse(F&& f) const& -> Result<T, TError>
        {
            using ResultType = Result<T, TError>;

            return IsOk() ? ResultType::Ok(std::get<T>(m_Data))
                          : ResultType::Err(std::invoke(std::forward<F>(f), std::get<TError>(m_Data)));
        }

        template<class F>
        inline constexpr auto OrElse(F&& f) && -> Result<T, TError>
        {
            using ResultType = Result<T, TError>;

            return IsOk() ? ResultType::Ok(static_cast<T&&>(std::get<T>(m_Data)))
                          : ResultType::Err(std::invoke(std::forward<F>(f), static_cast<TError&&>(std::get<TError>(m_Data))));
        }

        template<class F>
        inline constexpr auto AndThen(F&& f) const& -> Result<T, TError>
        {
            using ResultType = Result<T, TError>;

            return IsOk() ? ResultType::Ok(std::invoke(std::forward<F>(f), std::get<T>(m_Data)))
                          : ResultType::Err(std::get<TError>(m_Data));
        }

        template<class F>
        inline constexpr auto AndThen(F&& f) && -> Result<T, TError>
        {
            using ResultType = Result<T, TError>;

            return IsOk() ? ResultType::Ok(std::invoke(std::forward<F>(f), static_cast<T&&>(std::get<T>(m_Data))))
                          : ResultType::Err(static_cast<TError&&>(std::get<TError>(m_Data)));
        }

        template<class F>
        [[nodiscard]] inline constexpr auto Map(F&& f) const& -> Result<std::invoke_result_t<F, const T&>, TError>
        {
            using InvokeResultType = std::invoke_result_t<F, const T&>;
            using ResultType       = Result<InvokeResultType, TError>;

            return IsOk() ? ResultType::Ok(std::invoke(std::forward<F>(f), std::get<T>(m_Data)))
                          : ResultType::Err(std::get<TError>(m_Data));
        }

        template<class F>
        [[nodiscard]] inline constexpr auto Map(F&& f) && -> Result<std::invoke_result_t<F, T&&>, TError>
        {
            using InvokeResultType = std::invoke_result_t<F, T&&>;
            using ResultType       = Result<InvokeResultType, TError>;

            return IsOk() ? ResultType::Ok(std::invoke(std::forward<F>(f), static_cast<T&&>(std::get<T>(m_Data))))
                          : ResultType::Err(static_cast<TError&&>(std::get<TError>(m_Data)));
        }
    };

    using OK = EmptyStruct;
    template<class TError>
    using VoidResult = Result<OK, TError>;

#define ACTIAS_Guard(Value, Error)                                                                                                   \
    do                                                                                                                           \
    {                                                                                                                            \
        if (!(Value))                                                                                                            \
        {                                                                                                                        \
            return ::Actias::Err(Error);                                                                                             \
        }                                                                                                                        \
    }                                                                                                                            \
    while (false)

#define ACTIAS_GuardResult(Value) ACTIAS_Guard((Value), (Value).UnwrapErr())
} // namespace Actias
