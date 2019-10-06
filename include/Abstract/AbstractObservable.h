#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_OBSERVABLE_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_OBSERVABLE_H_
#pragma once

#include <Abstract/AbstractObserver.h>

template<typename T>
class AbstractObservable {
public:
    ~AbstractObservable() = default;

    virtual void registerObserver(AbstractObserver<T> &observer) = 0;

    virtual void unregisterObserver(AbstractObserver<T> &observer) = 0;

    virtual void notifyObservers() const = 0;
};

#endif