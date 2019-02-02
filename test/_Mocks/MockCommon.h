#ifndef AQUARIUMCONTROLLER_MOCK_COMMON_H
#define AQUARIUMCONTROLLER_MOCK_COMMON_H

#include <cstdint>

//#include <time.h>
//
//time_t theTime = time(nullptr);
//struct tm* aTime = localtime(&theTime);
//
//uint8_t minute() {
//    return (uint8_t) aTime->tm_min;
//};
//
//uint8_t hour() {
//    return (uint8_t) aTime->tm_hour;
//};
//
//uint8_t weekday() {
//    return (uint8_t) aTime->tm_wday;
//};

uint32_t currentMillis = 0;
uint8_t currentMinute = 0;
uint8_t currentHour = 0;
uint8_t currentDayOfWeek = 0;

uint32_t millis() {
    return currentMillis;
};

uint8_t minute() {
    return currentMinute;
};

uint8_t hour() {
    return currentHour;
};

uint8_t weekday() {
    return currentDayOfWeek;
};

void delay(uint32_t) {
    return;
}

#endif
