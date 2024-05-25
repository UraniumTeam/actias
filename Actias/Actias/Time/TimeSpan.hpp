#pragma once
#include <Actias/Base/Base.hpp>
#include <Actias/System/Platform.h>
#include <Actias/System/Time.h>

namespace Actias
{
    //! \brief Represents a time span: an interval between two instances of DateTime.
    class TimeSpan final
    {
        ActiasDateTimeInfo m_Data;

        inline explicit TimeSpan(ActiasTime time)
        {
            ActiasDecomposeTime(time, &m_Data);
        }

    public:
        ACTIAS_RTTI_Struct(TimeSpan, "F42DAA6C-53F3-4AA5-9971-9783D8754F6C");

        [[nodiscard]] inline ActiasTime UnixTime() const
        {
            ActiasDateTimeInfo copy = m_Data;
            return ActiasComposeTime(&copy);
        }

        [[nodiscard]] inline Int32 Year() const
        {
            return m_Data.Year;
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

        [[nodiscard]] inline static TimeSpan FromUnixTime(ActiasTime seconds)
        {
            return TimeSpan(seconds);
        }
    };
} // namespace Actias
