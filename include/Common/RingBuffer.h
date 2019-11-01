#ifndef _AQUARIUM_CONTROLLER_INCLUDE_COMMON_RING_BUFFER_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_COMMON_RING_BUFFER_H_
#pragma once

#include <math.h>

/**
 * <br/>
 * <a href="https://pragprog.com/magazines/2011-04/advanced-arduino-hacking">Advanced Arduino Hacking by Maik Schmidt</a><br/>
 * You might be tempted to read the sensor’s signal from analog pin using <tt>analogRead()</tt> and immediately use this value.<br/>
 * Unfortunately, the real world is a bit more complicated, because sensor signals are often subject to jitter and distortion.<br/>
 * It is a much better to continuously append sensor data to a small buffer and calculate their average value.<br/>
 * If the buffer is full and a new sensor value arrives, the oldest value will be removed from the buffer.
 *
 * @tparam T – type of data stored in the buffer
 * @tparam N – buffer size
 */
template<typename T, uint8_t N>
class RingBuffer {
private:
    T *values;
    uint8_t index = 0;
    uint8_t actualSize = 0;

public:

    RingBuffer() {
        values = new T[N];
    };

    RingBuffer(RingBuffer const &rhs) {
        *this = rhs;
    }

    RingBuffer &operator=(const RingBuffer &rhs) {
        if (this != &rhs) {
            index = rhs.index;
            values = new T[N];
            for (uint8_t i = 0; i < N; i++) { values[i] = rhs.values[i]; }
        }
        return *this;
    }

    ~RingBuffer() {
        delete[] values;
    }

    uint8_t size() const { return N; }

    void add(const T value) {
        values[index] = value;
        index = (index + 1) % N;
        if (actualSize < N) {
            ++actualSize;
        }
    }

    T getAverage() const {
        if (actualSize == 0) { return T{}; }

        uint8_t size = (actualSize < N) ? actualSize: N;
        float sum = 0.0;

        for (uint8_t i = 0; i < size; ++i)
            sum += values[i];
        return round(sum / size);
    }
};

#endif