/**
 * @file gregorian.h
 * @copyright (c) 2018, natersoz. Distributed under the Apache 2.0 license.
 */

#pragma once
#include <cstdint>

namespace utility
{

/**
 * @struct gregorian
 * A calendar date/time C++ structure using an epoch of Jan 1, 1601.
 */
struct gregorian
{
    static constexpr uint16_t const epoch_year              = 1601u;
    static constexpr uint8_t  const months_in_year          =   12u;
    static constexpr uint16_t const days_in_non_leap_year   =  365u;

    static constexpr uint8_t  const hours_in_days           =   24u;
    static constexpr uint8_t  const minutes_in_hours        =   60u;
    static constexpr uint8_t  const seconds_in_minutes      =   60u;
    static constexpr uint32_t const seconds_in_hour = minutes_in_hours * seconds_in_minutes;
    static constexpr uint32_t const seconds_in_day  = hours_in_days    * seconds_in_hour;

    enum month : uint8_t
    {
        January = 0u,
        February,
        March,
        April,
        May,
        June,
        July,
        August,
        September,
        November,
        December
    };

    enum class day_of_week: uint8_t /// @note Not yet used.
    {
        monday      = 0,
        tuesday,
        wednesday,
        thursday,
        friday,
        saturday,
        sunday,
    };

    ~gregorian() = default;

    gregorian(gregorian const&)             = default;
    gregorian(gregorian &&)                 = default;
    gregorian& operator=(gregorian const&)  = default;
    gregorian& operator=(gregorian&&)       = default;

    gregorian()
    : year(0u), month(0u), day_of_month(0u), day_of_year(0U),
      hours(0u), minutes(0u), seconds(0u)
    {
    }

    gregorian(uint16_t _year,
              uint8_t  _month,
              uint8_t  _day_of_month,
              uint8_t  _hours,
              uint8_t  _minutes,
              uint8_t  _seconds,
              uint16_t _day_of_year = UINT16_MAX)
    : year(_year),
      month(_month),
      day_of_month(_day_of_month),
      day_of_year(_day_of_year),
      hours(_hours),
      minutes(_minutes),
      seconds(_seconds)
    {
        if (this->day_of_year == UINT16_MAX)
        {

        }
    }

    gregorian(uint64_t seconds_since_epoch)
    {
        *this = to_calendar(seconds_since_epoch);
    }

    uint16_t year;              ///< [1601:UINT16_MAX]
    uint8_t  month;             ///< [0:11],
    uint8_t  day_of_month;      ///< [0:30],
    uint16_t day_of_year;       ///< [0:365], 365 is day 366 of a leap year
    uint8_t  hours;             ///< [0:23]
    uint8_t  minutes;           ///< [0:59]
    uint8_t  seconds;           ///< [0:59]

    /**
     * Convert from the number of seconds since the epoch to the gregorian
     * calendar date, time.
     *
     * @param seconds_since_epoch The number of seconds since gregorian::epoch.
     * @return gregorian          The gregorian date/time struct.
     */
    static gregorian to_calendar(uint64_t seconds_since_epoch);

    /**
     * Convert from the gregorian date/time struct to the number of seconds
     * since the gregorian::epoch.
     *
     * @param greg      The gregorian date/time struct.
     * @return uint64_t The number of seconds since the gregorian::epoch.
     */
    static uint64_t seconds_since_epoch(gregorian const &greg);

    /**
     * Determine whether a year is a leap year.
     *
     * @param year  The year in question.
     * @return bool true if the year is a leap year; false if it is not.
     */
    static bool is_leap_year(uint16_t year);

    /**
     * Determine the number of days in a month.
     *
     * @param month The month in question [0:11] or enum gregorian::month.
     * @param year  The year - required to determin February's leap day.
     * @return uint8_t The number of days in the month.
     */
    static uint8_t get_days_in_month(uint8_t month, uint16_t year);

    /**
     * Determine the number of leap years since the gregoian::epoch.
     *
     * @param year      The year in question.
     * @return uint16_t The number of leap years since the gregorian::epoch.
     */
    static uint16_t leap_years_since_epoch(uint16_t year);

    static uint8_t get_day_of_year(uint16_t year, uint8_t month, uint8_t day_of_month);

private:
    /// There are 97 leap years in each span of 400 years:
    /// + 100 years are divisible by 4:        +100
    /// + but 4 of those are divisible by 100:   -4
    /// + and one of them is divisible by 400:   +1
    static constexpr uint16_t const leaps_in_400_years = (400u / 4u) - (400u / 100u) + 1u;
    static_assert(leaps_in_400_years == 97u);

    /// In any 100 year span there are 24 leap years.
    static constexpr uint16_t const leaps_in_100_years = (100u / 4u) - 1u;
    static_assert(leaps_in_100_years == 24u);

    static constexpr uint64_t const days_in_400_years    = 400u * days_in_non_leap_year + leaps_in_400_years;
    static constexpr uint64_t const seconds_in_400_years = days_in_400_years * seconds_in_day;

    static constexpr uint64_t const days_in_100_years    = 100u * days_in_non_leap_year + leaps_in_100_years;
    static constexpr uint64_t const seconds_in_100_years = days_in_100_years * seconds_in_day;

    static constexpr uint64_t const days_in_4_years      = 4u * days_in_non_leap_year + 1u;
    static constexpr uint64_t const seconds_in_4_years   = days_in_4_years * seconds_in_day;
    static constexpr uint64_t const seconds_in_1_years   = days_in_non_leap_year * seconds_in_day;
};

} // namespace utility
