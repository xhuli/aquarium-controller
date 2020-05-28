#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ATO_STATION_HIGH_LEVEL_SENSOR_CONNECTION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ATO_STATION_HIGH_LEVEL_SENSOR_CONNECTION_H_
#pragma once

#include <Abstract/AbstractConnection.h>

/**
 * <br/>
 * Concrete class.<br/>
 * <strong>Note:</strong> Consumer method must exist!<br/>
 * \code
 * ((S) consumer).setHighLevelState((T) data)
 * \endcode
 *
 * @tparam S – consumer type
 * @tparam T – forward data type
 */
template<typename S, typename T>
class HighLevelSensorConnection : public AbstractConnection<S, T> {

public:

    explicit HighLevelSensorConnection(S &consumer) : AbstractConnection<S, T>(consumer) {}

    void forward(T const &data) const override {
        AbstractConnection<S, T>::consumer.setHighLevelState(data);
    }
};

#endif