#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ENUMS_DAY_OF_WEEK_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ENUMS_DAY_OF_WEEK_H_
#pragma once

/**
 * <br/>
 * In <tt>Time.h</tt> from <em>Paul Stoffregen</em>, for <tt>weekday</tt> we have: Sunday is day 1
 */
enum class DayOfWeek : uint8_t {
    EveryDay = 0,
    Sunday = 1,
    Monday = 2,
    Tuesday = 3,
    Wednesday = 4,
    Thursday = 5,
    Friday = 6,
    Saturday = 7,

    MondayToFriday = 8,
    SaturdayAndSunday = 9,

    MoTuWeThFr = 10,
    SaSu = 11,
    MoWeFrSu = 12,
    TuThSa = 13,
};

#endif