#pragma once
#include <Actias/RTTI/RTTI.hpp>
#include <Actias/Strings/FixedString.hpp>
#include <Actias/Strings/String.hpp>
#include <Actias/Time/TimeSpan.hpp>

namespace Actias
{
    enum class DateTimeFormatKind
    {
        ISO8601,
        Short,
        Long,
    };

    namespace Internal
    {
        class DateTimeBase
        {
            friend struct DateTimeConvert;

        protected:
            ActiasDateTimeInfo m_Data;

            inline DateTimeBase(ActiasDateTimeInfo data)
                : m_Data(data)
            {
            }

        public:
            [[nodiscard]] inline Int32 Year() const
            {
                return m_Data.Year + 1900;
            }

            [[nodiscard]] inline Int32 Month() const
            {
                return m_Data.Month;
            }

            [[nodiscard]] inline Int32 Day() const
            {
                return m_Data.Day;
            }

            [[nodiscard]] inline Int32 DayOfWeek() const
            {
                return m_Data.DayOfWeek;
            }

            [[nodiscard]] inline Int32 Hour() const
            {
                return m_Data.Hour;
            }

            [[nodiscard]] inline Int32 Minute() const
            {
                return m_Data.Minute;
            }

            [[nodiscard]] inline Int32 Second() const
            {
                return m_Data.Second;
            }

            FixStr128 ToString(DateTimeFormatKind formatKind) const;
        };
    } // namespace Internal

    //! \brief Represents date and time in UTC.
    class UTCDateTime final : public Internal::DateTimeBase
    {
        friend struct DateTimeConvert;

        inline UTCDateTime(ActiasDateTimeInfo data)
            : DateTimeBase(data)
        {
        }

    public:
        ACTIAS_RTTI_Struct(UTCDateTime, "9927EE14-F009-464A-A067-4E4CB7AFE56C");

        [[nodiscard]] inline ActiasTime UnixTime() const
        {
            return ActiasComposeTime(&m_Data);
        }

        [[nodiscard]] inline static UTCDateTime FromUnixTime(ActiasTime time)
        {
            ActiasDateTimeInfo decomposed;
            ActiasDecomposeTime(time, &decomposed);
            return UTCDateTime{ decomposed };
        }

        [[nodiscard]] inline static UTCDateTime Now()
        {
            return FromUnixTime(ActiasGetCurrentTimeUTC());
        }

        [[nodiscard]] inline friend TimeSpan operator-(const UTCDateTime& lhs, const UTCDateTime& rhs)
        {
            return TimeSpan::FromUnixTime(ActiasComposeTime(&lhs.m_Data) - ActiasComposeTime(&rhs.m_Data));
        }
    };

    class LocalDateTime final : public Internal::DateTimeBase
    {
        friend struct DateTimeConvert;

        inline LocalDateTime(ActiasDateTimeInfo data)
            : DateTimeBase(data)
        {
        }

    public:
        ACTIAS_RTTI_Struct(LocalDateTime, "7D309125-A8C2-4FCB-BDCF-CCDA5DAF775D");

        [[nodiscard]] inline ActiasTime UnixTime() const
        {
            ActiasDateTimeInfo utc;
            ActiasConvertLocalTimeToUTC(&m_Data, &utc);
            return ActiasComposeTime(&utc);
        }

        [[nodiscard]] inline static LocalDateTime FromUnixTime(ActiasTime time)
        {
            ActiasDateTimeInfo decomposed, local;
            ActiasDecomposeTime(time, &decomposed);
            ActiasConvertUTCToLocalTime(&decomposed, &local);
            return LocalDateTime{ local };
        }

        [[nodiscard]] inline static LocalDateTime Now()
        {
            return FromUnixTime(ActiasGetCurrentTimeUTC());
        }

        [[nodiscard]] inline friend TimeSpan operator-(const LocalDateTime& lhs, const LocalDateTime& rhs)
        {
            return TimeSpan::FromUnixTime(ActiasComposeTime(&lhs.m_Data) - ActiasComposeTime(&rhs.m_Data));
        }
    };

    struct DateTimeConvert
    {
        [[nodiscard]] inline static UTCDateTime ToUTC(const LocalDateTime& localDateTime)
        {
            UTCDateTime result{ ActiasDateTimeInfo{ 0 } };
            ActiasConvertLocalTimeToUTC(&localDateTime.m_Data, &result.m_Data);
            return result;
        }

        [[nodiscard]] inline static LocalDateTime ToLocal(const UTCDateTime& utcDateTime)
        {
            LocalDateTime result{ ActiasDateTimeInfo{ 0 } };
            ActiasConvertUTCToLocalTime(&utcDateTime.m_Data, &result.m_Data);
            return result;
        }
    };
} // namespace Actias
