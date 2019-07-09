#ifndef __AQUARIUM_CONTROLLER_MOCK_COMMON_H__
#define __AQUARIUM_CONTROLLER_MOCK_COMMON_H__

#include <cstdint>
#include <random>

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
uint8_t currentMinute = 0;
uint8_t currentHour = 0;
uint8_t currentDayOfWeek = 0;

time_t now() {
    return currentTime;
};

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

/**
 * https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
 *
 * @return random unsigned 32 bit integer
 */
uint32_t getRandomUint32() {
    //

    std::random_device rd;                                       // only used once to initialise (seed) engine
    std::mt19937 rng(rd());                                      // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<uint32_t> uni(0, 4294967295);  // guaranteed unbiased

    return uni(rng);
}

#endif
