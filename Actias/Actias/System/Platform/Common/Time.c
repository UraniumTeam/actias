#include <Actias/System/Time.h>

inline static ActiasBool IsYearLeap(Int64 year)
{
    return (!((year) % 4) && (((year) % 100) || !((year) % 400))) ? 1 : 0;
}

inline static Int64 GetDayCountInYear(Int64 year)
{
    return IsYearLeap(year) ? 366 : 365;
}

const static Int32 YearTable[2][12] = { { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
                                        { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } };

const static Int64 SecondsInDay = 24 * 60 * 60;

void ACTIAS_ABI ActiasDecomposeTime(ActiasTime time, ActiasDateTimeInfo* pResult)
{
    ActiasTime dayClock = time % SecondsInDay;
    ActiasTime srcDay   = time / SecondsInDay;

    pResult->Second    = dayClock % 60;
    pResult->Minute    = (dayClock % 3600) / 60;
    pResult->Hour      = (Int16)(dayClock / 3600);
    pResult->DayOfWeek = (srcDay + 4) % 7;

    Int64 year = 1970;
    while (srcDay >= GetDayCountInYear(year))
    {
        srcDay -= GetDayCountInYear(year);
        year++;
    }

    pResult->Year  = (Int16)(year - 1900);
    pResult->Month = 0;
    while (srcDay >= YearTable[IsYearLeap(year)][pResult->Month])
    {
        srcDay -= YearTable[IsYearLeap(year)][pResult->Month];
        pResult->Month++;
    }

    pResult->Day = (Int16)(srcDay + 1);
}

ActiasTime ACTIAS_ABI ActiasComposeTime(const ActiasDateTimeInfo* pDateTime)
{
    Int64 epochYear = 1970;
    Int64 srcYear   = (Int64)pDateTime->Year + 1900;
    Int64 srcDay    = (srcYear - epochYear) * 365;
    srcDay += (srcYear - epochYear) / 4 + ((srcYear % 4) && srcYear % 4 < epochYear % 4);
    srcDay -= (srcYear - epochYear) / 100 + ((srcYear % 100) && srcYear % 100 < epochYear % 100);
    srcDay += (srcYear - epochYear) / 400 + ((srcYear % 400) && srcYear % 400 < epochYear % 400);

    Int64 yearDay = 0;
    Int64 month   = 0;
    while (month < pDateTime->Month)
    {
        yearDay += YearTable[IsYearLeap(srcYear)][month];
        month++;
    }

    yearDay += (Int64)pDateTime->Day - 1;
    srcDay += yearDay;

    ActiasTime result = srcDay * SecondsInDay;
    result += ((Int64)pDateTime->Hour * 60 + pDateTime->Minute) * 60;
    result += pDateTime->Second;
    return result;
}
