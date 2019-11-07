#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_LIST_ITERATOR_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_LIST_ITERATOR_H_
#pragma once

template<typename V>
struct ListIterator {

    virtual ~ListIterator() = default;

    virtual V next() const = 0;

    virtual bool hasNext() const = 0;

    virtual V getValue() const = 0;

    virtual void forward() const = 0;
};

#endif