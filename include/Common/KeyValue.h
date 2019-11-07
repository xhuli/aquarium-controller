#ifndef _AQUARIUM_CONTROLLER_INCLUDE_COMMON_KEY_VALUE_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_COMMON_KEY_VALUE_H_
#pragma once

/**
 * <br/>
 * Data structure for linked map <tt>key–value</tt> pairs.
 * @tparam K – key type
 * @tparam V – value type
 */
template<typename K, typename V>
class KeyValue {

private:

    K key;
    V value;

public:

    KeyValue(K const &key, V const &value) : key(key), value(value) {}

    K getKey() const {
        return key;
    }

    void setKey(K const &key) {
        KeyValue::key = key;
    }

    V getValue() const {
        return value;
    }

    void setValue(V const &value) {
        KeyValue::value = value;
    }
};

#endif