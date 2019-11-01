#ifndef __DOSING_TASK_H__
#define __DOSING_TASK_H__

typedef struct {
    // Use one byte to store two values
    // dayOfWeek: (0 - 7) <= (2)^3 -> it can be stored in 3 bits
    // startHour: (0 - 23) <= (2)^5 -> it can be stored in 5 bits
    // 3 + 5 bits = 8 bits = 1 byte
    uint8_t dayOfWeek : 3, startHour : 5;

    uint8_t startMinute : 6, doseMilliLitersFraction : 2;   // (0 - 59), (0, 1, 2, 3) * (0.25)
    uint8_t doseMilliLiters;                                // (1 - 255)
} DosingTask;

#endif