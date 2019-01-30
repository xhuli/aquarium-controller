#ifndef __MOCK_COMMON_H__
#define __MOCK_COMMON_H__

#include <stdint.h>

//#include <stdint.h>
//#include <chrono>  // C++ header
//
//uint32_t millis() {
//    // using namespace std::chrono;
//    // return (uint32_t) duration_cast<milliseconds>(system_clock::now().time_since_epoch());
//
//    // std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//
//    auto now = std::chrono::system_clock::now();
//    time_t tnow = std::chrono::system_clock::to_time_t(now);
//    tm *date = std::localtime(&tnow);
//    date->tm_hour = 0;
//    date->tm_min = 0;
//    date->tm_sec = 0;
//    auto midnight = std::chrono::system_clock::from_time_t(std::mktime(date));
//
//    uint32_t duration = (now - midnight).count();
//    return duration;
//}



void delay(uint32_t) {
    return;
}

#endif