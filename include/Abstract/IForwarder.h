#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_I_FORWARDER_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_I_FORWARDER_H_
#pragma once

/**
 * <br/>
 * Interface between sensor objects and connection objects.<br/>
 * <strong>Implement:</strong>
 * <ul>
 * <li><tt>void forward(T const &data)</tt></li>
 * </ul>
 *
 * @tparam T – forward data type
 */
template<typename T>
class IForwarder {

public:
    
    virtual ~IForwarder() = default;
    
    virtual void forward(T const &data) const = 0;
};

#endif