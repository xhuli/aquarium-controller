#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_OBSERVER_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_OBSERVER_H_

template<typename T>
class AbstractObserver {
public:
    ~AbstractObserver() = default;

    virtual void update(T const &state) = 0;
};

#endif