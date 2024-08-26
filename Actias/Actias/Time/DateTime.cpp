#include <Actias/Time/DateTime.hpp>

namespace Actias::Internal
{
    constexpr StringSlice DayNames[] = {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday",
    };

    constexpr StringSlice DayNamesShort[] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
    };

    constexpr StringSlice MonthNames[] = {
        "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December",
    };

    constexpr StringSlice MonthNamesShort[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
    };

    FixStr128 DateTimeBase::ToString(DateTimeFormatKind formatKind) const
    {
        char result[64];
        result[0] = 0;

        char* iter = result;

        switch (formatKind)
        {
        default:
        case DateTimeFormatKind::ISO8601:
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Year(), 4);
            *iter++ = '-';
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Month() + 1, 2);
            *iter++ = '-';
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Day(), 2);
            *iter++ = 'T';
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Hour(), 2);
            *iter++ = ':';
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Minute(), 2);
            *iter++ = ':';
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Second(), 2);
            *iter++ = 'Z';
            break;
        case DateTimeFormatKind::Short:
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Month() + 1);
            *iter++ = '/';
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Day());
            *iter++ = '/';
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Year());
            break;
        case DateTimeFormatKind::Long:
            iter = Str::Copy(iter, sizeof(result) - (iter - result), DayNames[DayOfWeek()].Data(), DayNames[DayOfWeek()].Size());
            *iter++ = ',';
            *iter++ = ' ';
            iter    = Str::Copy(iter, sizeof(result) - (iter - result), MonthNames[Month()].Data(), MonthNames[Month()].Size());
            *iter++ = ' ';
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Day());
            *iter++ = ',';
            *iter++ = ' ';
            iter    = StrConvert::Decimal(iter, sizeof(result) - (iter - result), Year());
            break;
        }

        return { result, static_cast<USize>(iter - result) };
    }
} // namespace Actias::Internal
