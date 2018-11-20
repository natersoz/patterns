/**
 * @file gregorian.h
 * @copyright (c) 2018, natersoz. Distributed under the Apache 2.0 license.
 */


#include "gregorian.h"
#include "logger.h"
#include "project_assert.h"

namespace utility
{

constexpr uint8_t const days_in_month[gregorian::months_in_year] =
{
    31u,    // January
    28u,    // February     note: except in a leap year.
    30u,    // March
    30u,    // April
    31u,    // May
    30u,    // June
    31u,    // July
    31u,    // August
    31u,    // September
    30u,    // November
    31u,    // December
};

gregorian gregorian::to_calendar(uint64_t seconds_since_epoch)
{
    uint64_t const span_400 = seconds_since_epoch / seconds_in_400_years;
    seconds_since_epoch -= span_400 * seconds_in_400_years;
    uint64_t const span_100 = seconds_since_epoch / seconds_in_100_years;
    seconds_since_epoch -= span_100 * seconds_in_100_years;
    uint64_t const span_4 = seconds_since_epoch / seconds_in_4_years;
    seconds_since_epoch -= span_4   * seconds_in_4_years;
    uint64_t const span_1 = seconds_since_epoch / seconds_in_1_years;
    seconds_since_epoch -= span_1   * seconds_in_1_years;

    uint16_t const year_span =
        span_400 * 400u +
        span_100 * 100u +
        span_4   *   4u +
        span_1   *   1u ;

    uint16_t const year         = year_span + gregorian::epoch_year;
    uint16_t const day_of_year  = seconds_since_epoch / seconds_in_day;
    seconds_since_epoch -= day_of_year * gregorian::seconds_in_day;

    uint16_t day_count = day_of_year;
    for (uint8_t month_iter = 0u; month_iter < months_in_year; ++month_iter)
    {
        uint16_t const days_in_month = get_days_in_month(month_iter, year);
        if (day_count >= days_in_month)
        {
            day_count -= days_in_month;
        }
        else
        {
            uint8_t const hours = seconds_since_epoch / seconds_in_hour;
            seconds_since_epoch -= hours * seconds_in_hour;

            uint8_t const minutes = seconds_since_epoch / seconds_in_minutes;
            seconds_since_epoch -= minutes * seconds_in_minutes;

            return gregorian(year,  month_iter, day_count,
                             hours, minutes,    seconds_since_epoch,
                             day_of_year);
        }
    }

    ASSERT(0);
    return gregorian();
}

uint64_t gregorian::seconds_since_epoch(gregorian const &greg)
{
    // Determin the number of years and leap years since the epoch.
    uint16_t const years_since_epoch = greg.year - epoch_year;
    uint16_t const leap_year_count   = leap_years_since_epoch(greg.year);

    // Determine the number of days within the number of years since epoch.
    uint64_t days_since_epoch = 0u;
    days_since_epoch += years_since_epoch * days_in_non_leap_year;
    days_since_epoch += leap_year_count;
    days_since_epoch += get_day_of_year(greg.year, greg.month, greg.day_of_month);

    uint64_t const seconds = days_since_epoch * seconds_in_day +
                             greg.hours   * seconds_in_hour   +
                             greg.minutes * seconds_in_minutes +
                             greg.seconds;
    return seconds;
}

bool gregorian::is_leap_year(uint16_t year)
{
    // Years which are divisible by 400 are leap years.
    if ((year % 400u) == 0u) { return true; }

    // Years which are divisible by 100 and are not divisible by 400
    // (div 400 case is handled above) are not leap years.
    if ((year % 100u) == 0u) { return false; }

    // All 100, 400 year cases have been handled;
    // Years divisible by 4 are leap years.
    if ((year %   4u) == 0u) { return true; }

    // All other years are not leap years.
    return false;
}

uint8_t gregorian::get_days_in_month(uint8_t month, uint16_t year)
{
    ASSERT(month < months_in_year);

    if (month == February)
    {
        uint8_t days = days_in_month[month];
        days += is_leap_year(year) ? 1u : 0u;
        return days;
    }

    return days_in_month[month];
}

uint16_t gregorian::leap_years_since_epoch(uint16_t year)
{
    ASSERT(year >= gregorian::epoch_year);

    uint16_t const delta_years = year - gregorian::epoch_year;
    uint16_t const delta_400   = delta_years / 400u;
    uint16_t const delta_100   = delta_years / 100u;
    uint16_t const delta_4     = delta_years /   4u;
    uint16_t const leap_years  = delta_4 - delta_100 + delta_400;
    return leap_years;
}

uint8_t gregorian::get_day_of_year(uint16_t year, uint8_t month, uint8_t day_of_month)
{
    ASSERT(year         >= epoch_year);
    ASSERT(month        <  months_in_year);
    ASSERT(day_of_month <  get_days_in_month(month, year));

    uint8_t day_count = 0u;
    for (uint8_t month_iter = 0u; month_iter < month; ++month)
    {
        day_count += get_days_in_month(month, year);
    }

    day_count += day_of_month;
    return day_count;
}

} // namespace utility
