#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_STATION_CONNECTION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_STATION_CONNECTION_H_
#pragma once

#include "IForwarder.h"

/**
 * <br/>
 * Abstract class.<br/>
 * <strong>Implement:</strong>
 * <ul>
 * <li><tt>void IForwarder::forward(T const &data)</tt></li>
 * </ul>
 * 
 * @tparam S – consumer type
 * @tparam T – forward data type
 */
template<typename S, typename T>
class AbstractConnection : public IForwarder<T> {

protected:

    S &consumer;

public:

    explicit AbstractConnection(S &consumer) : consumer(consumer) {}

    virtual ~AbstractConnection() = default;
};

#endif