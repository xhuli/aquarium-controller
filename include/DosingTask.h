#ifndef __DOSING_TASK_H__
#define __DOSING_TASK_H__

typedef struct {
    uint8_t startHour;
    uint8_t startMinute;
    uint8_t doseMiliLiters;
} DosingTask;

#endif