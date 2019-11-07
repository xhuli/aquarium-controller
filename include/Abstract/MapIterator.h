#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_MAP_ITERATOR_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_MAP_ITERATOR_H_
#pragma once

#include <Common/KeyValue.h>

template<typename K, typename V>
struct MapIterator {

    virtual ~MapIterator() = default;

    virtual KeyValue<K, V> *next() const = 0;

    virtual bool hasNext() const = 0;

    virtual KeyValue<K, V> *getKeyValue() const = 0;

    virtual void forward() const = 0;
};

#endif