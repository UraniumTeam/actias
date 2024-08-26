/*
 * Copyright 2024 the Actias project authors (see the file AUTHORS
 * for a complete list)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include <Actias/System/Core.h>

ACTIAS_BEGIN_C

//! \brief Time in Actias is represented in number of seconds from UNIX epoch as a 64-bit integer.
typedef Int64 ActiasTime;

//! \brief Decomposed date and time info.
typedef struct ActiasDateTimeInfo
{
    Int16 Year;      //!< Year since 1900.
    Int16 Month;     //!< Month 0-11.
    Int16 Day;       //!< Day of month 1-31.
    Int16 DayOfWeek; //!< Day since Sunday 0-6.
    Int16 Hour;      //!< Hour 0-23.
    Int16 Minute;    //!< Minutes 0-59.
    Int16 Second;    //!< Seconds 0-60.
} ActiasDateTimeInfo;

//! \brief Get current system time in UTC format.
ACTIAS_SYSTEM_API ActiasTime ACTIAS_ABI ActiasGetCurrentTimeUTC();

//! \brief Decompose ActiasTime into ActiasDateTimeInfo.
//!
//! \param time    - The time value to decompose.
//! \param pResult - A pointer to the variable that receives the resulting ActiasDateTimeInfo.
ACTIAS_SYSTEM_API void ACTIAS_ABI ActiasDecomposeTime(ActiasTime time, ActiasDateTimeInfo* pResult);

//! \brief Compose ActiasTime from ActiasDateTimeInfo.
//!
//! \param pDateTime - A pointer to the variable that holds ActiasDateTimeInfo to compose from.
//!
//! \return The composed ActiasTime value.
ACTIAS_SYSTEM_API ActiasTime ACTIAS_ABI ActiasComposeTime(const ActiasDateTimeInfo* pDateTime);

//! \brief Convert date and time in UTC format to timezone specific local format.
//!
//! \param pDateTime - A pointer to the variable that holds ActiasDateTimeInfo to convert from.
//! \param pResult   - A pointer to the variable that holds ActiasDateTimeInfo to write the result to.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasConvertUTCToLocalTime(const ActiasDateTimeInfo* pDateTime,
                                                                      ActiasDateTimeInfo* pResult);

//! \brief Convert timezone specific local date and time to UTC format.
//!
//! \param pDateTime - A pointer to the variable that holds ActiasDateTimeInfo to convert from.
//! \param pResult   - A pointer to the variable that holds ActiasDateTimeInfo to write the result to.
//!
//! \return ActiasResult that indicates the status of the operation.
ACTIAS_SYSTEM_API ActiasResult ACTIAS_ABI ActiasConvertLocalTimeToUTC(const ActiasDateTimeInfo* pDateTime,
                                                                      ActiasDateTimeInfo* pResult);

ACTIAS_END_C
