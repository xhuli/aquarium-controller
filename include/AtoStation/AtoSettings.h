#ifndef __ATO_SETTINGS_H__
#define __ATO_SETTINGS_H__

typedef struct {
    uint32_t maxDispensingDurationMillis = 30000;       // 30 sec * 1000 ms
    uint32_t minDispensingIntervalMillis = 1200000;     // 20 min * 60 sec * 1000 ms
} AtoSettings;

#endif