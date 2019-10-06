#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ATO_STATION_NORMAL_LEVEL_SENSOR_CONNECTION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ATO_STATION_NORMAL_LEVEL_SENSOR_CONNECTION_H_
#pragma once

#include <Abstract/AbstractConnection.h>

/**
 * <br/>
 * Concrete class.<br/>
 * <strong>Note:</strong> Consumer method must exist!<br/>
 * <pre><code>
 * ((S) consumer).setNormalLevelState((T) data)
 * </code></pre>
 *
 * @tparam S – consumer type
 * @tparam T – forward data type
 */
template<typename S, typename T>
class NormalLevelSensorConnection : public AbstractConnection<S, T> {

public:

    explicit NormalLevelSensorConnection(S &consumer) : AbstractConnection<S, T>(consumer) {}

    void forward(T const &data) const override {
        AbstractConnection<S, T>::consumer.setNormalLevelState(data);
    }
};

#endif