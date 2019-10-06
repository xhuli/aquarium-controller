#ifndef _AQUARIUM_CONTROLLER_MOCK_COMMON_H_
#define _AQUARIUM_CONTROLLER_MOCK_COMMON_H_
#pragma once

#include <stdint.h>
#include <random>
#include <time.h>
#include <functional>

//#include <time.h>

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

time_t currentTime = 0;
uint32_t currentMillis = 0;
uint8_t currentSecond = 0;
uint8_t currentMinute = 0;
uint8_t currentHour = 0;
uint8_t currentDayOfWeek = 0;

time_t now() {
    return currentTime;
};

uint32_t millis() {
    return currentMillis;
};

uint8_t second() {
    return currentSecond;
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

uint32_t getRandomUint32() {
//    static const int shift = static_cast<int>(std::log2(RAND_MAX));
//    uint32_t value;
//    if ((rand() >> shift) & 1) {
//        value = rand() * rand();
//    } else {
//        value = rand();
//    }
//    // std::cout << "rand: " << value << "\n";
//    return value;

    return rand();
}

#endif
