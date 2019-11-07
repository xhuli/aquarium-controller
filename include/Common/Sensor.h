#ifndef _AQUARIUM_CONTROLLER_INCLUDE_COMMON_SENSOR_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_COMMON_SENSOR_H_
#pragma once

#include "Abstract/IForwarder.h"

/**
 * <br/>
 * Concrete class.<br/>
 * Call <tt>setReading(T const reading)</tt> to update sensor reading.
 *
 * @tparam T – sensor data type
 */
template<typename T>
class Sensor {

private:

    const IForwarder<T> *forwarder;
    T reading;

    void forwardReading() const {
        forwarder->forward(reading);
    }

public:

    Sensor(
            IForwarder<T> const *forwarder,
            T initialReading
    ) :
            forwarder(forwarder),
            reading(initialReading) {}

    explicit Sensor(
            T initialReading
    ) :
            forwarder(nullptr),
            reading(initialReading) {}

    ~Sensor() = default;

    T getReading() const {
        return reading;
    }

    void setReading(T const reading) {
        Sensor::reading = reading;
        if (forwarder != nullptr) {
            Sensor::forwardReading();
        }
    }

    bool isReading(T const value) const {
        return reading == value;
    }
};

#endif