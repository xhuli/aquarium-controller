#ifndef __MOCK_MILLIS__
#define __MOCK_MILLIS__

uint32_t currentMillis = 0;

uint32_t millis() {
    return currentMillis;
};

#endif