#ifndef _AQUARIUM_CONTROLLER_INCLUDE_AMBIENT_STATION_AMBIENT_HUMIDITY_SENSOR_CONNECTION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_AMBIENT_STATION_AMBIENT_HUMIDITY_SENSOR_CONNECTION_H_
#pragma once

#include <Abstract/AbstractConnection.h>

/**
 * <br/>
 * Concrete class.<br/> 
 * <strong>Note:</strong> Consumer method must exist!<br/>
 * <pre><code>
 * ((S) consumer).setAmbientHumidity((T) data)
 * </code></pre>
 *
 * @tparam S – consumer type
 * @tparam T – forward data type
 */
template<typename S, typename T>
class AmbientHumiditySensorConnection : public AbstractConnection<S, T> {

public:

    explicit AmbientHumiditySensorConnection(S &consumer) : AbstractConnection<S, T>(consumer) {}

    void forward(T const &data) const override {
        AbstractConnection<S, T>::consumer.setAmbientHumidity(data);
    }
};

#endif