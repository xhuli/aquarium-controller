#ifndef _AQUARIUM_CONTROLLER_INCLUDE_COMMON_LINKED_HASH_MAP_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_COMMON_LINKED_HASH_MAP_H_
#pragma once

/**
 * <br/>
 * Data structure for linked map <tt>key–value</tt> pairs.
 * @tparam K – key type
 * @tparam V – value type
 */
template<typename K, typename V>
struct KeyValuePair {
    KeyValuePair *next;
    K key;
    V value;

    KeyValuePair(
            KeyValuePair *head,
            K const &key,
            V const &value
    ) :
            next(head),
            key(key),
            value(value) {}
};

/**
 * <br/>
 * Do not expect miracles, this class does not work for <tt>char[]</tt> or similar.
 *
 * @tparam K – map keys type
 * @tparam V – map values type
 */
template<typename K, typename V>
class LinkedHashMap {

    uint8_t pairCount = 0;
    KeyValuePair<K, V> *mapHead = nullptr;

public:

    explicit LinkedHashMap() = default;

    virtual ~LinkedHashMap() {
        LinkedHashMap::clear();
    }

    /**
     * <br/>
     * Returns the number of <tt>key</tt>–<tt>value</tt> mappings in this map.
     * @return the number of <tt>key</tt>–<tt>value</tt> mappings in this map
     */
    uint8_t size() const {
        return pairCount;
    }

    /**
     * <br/>
     * Returns true if this map contains no key-value mappings.
     *
     * @return true if this map contains no <tt>key</tt>–<tt>value</tt> mappings
     */
    bool isEmpty() const {
        return pairCount == 0;
    }

    /**
     * <br/>
     * Removes all of the mappings from this map (optional operation).<br/>
     * The map will be empty after this call returns.
     */
    void clear() {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            KeyValuePair<K, V> *pNodeToDelete = *tracer;
            *tracer = (*tracer)->next;
            delete pNodeToDelete;
            pairCount--;
        }
    }

    /**
     * <br/>
     * Associates the specified value with the specified key in this map. If the map previously contained a mapping for the key, the old value is replaced.
     *
     * @param key – key with which the specified <tt>value</tt> is to be associated
     * @param value – value to be associated with the specified <tt>key</tt>
     * @return true if the value was put into the map
     */
    bool put(K const &key, V const &value) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->key == key) {
                (*tracer)->value = value;
                return true;
            }
            tracer = &(*tracer)->next;
        }
        /* create new node */
        mapHead = new KeyValuePair<K, V>(mapHead, key, value);
        ++pairCount;
        return true;
    }

    /**
     * <br/>
     * Returns the value to which the specified key is mapped, or <tt>V{}</tt> if this map contains no mapping for the key.
     *
     * @param key – the key whose associated value is to be returned
     * @return the <tt>value</tt> to which the specified <tt>key</tt> is mapped, or <tt>V{}</tt> if this map contains no mapping for the <tt>key</tt>
     */
    V get(K const key) const {
        KeyValuePair<K, V> *const *tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->key == key) {
                return (*tracer)->value;
            }
            tracer = &(*tracer)->next;
        }
        return V{}; // <- door to hell
    }

    /**
     * <br/>
     * Updates provided <tt>value</tt> variable to which the specified key is mapped. Caller is responsible to verify if <tt>value</tt> was updated.
     *
     * @param key – the key whose associated value is to be returned
     * @param value – variable to be updated  with <tt>value</tt> to which the specified <tt>key</tt> is mapped
     */
    void get(K const key, V &value) {
        KeyValuePair<K, V> *const *tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->key == key) {
                value = (*tracer)->value;
            }
            tracer = &(*tracer)->next;
        }
    }

    /**
     * <br/>
     * Get a pointer to the first <tt>key</tt>-<tt>value</tt> pair.
     *
     * @return pointer to the first <tt>key</tt>-<tt>value</tt> pair
     */
    KeyValuePair<K, V> *getFirstPair() const {
        return mapHead;
    }

    /**
     * <br/>
     * Returns the value to which the specified <tt>key</tt> is mapped, or <tt>defaultValue</tt> if this map contains no mapping for the <tt>key</tt>.
     *
     * @param key – the <tt>key</tt> whose associated <tt>value</tt> is to be returned
     * @param defaultValue – the default mapping of the <tt>key</tt>
     * @return the <tt>value</tt> to which the specified <tt>key</tt> is mapped, or <tt>defaultValue</tt> if this map contains no mapping for the <tt>key</tt>
     */
    V getOrDefault(K const key, V const &defaultValue) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->key == key) {
                return (*tracer)->value;
            }
            tracer = &(*tracer)->next;
        }
        return defaultValue;
    }

    /**
     * <br/>
     * Returns the value to which the specified <tt>key</tt> is mapped, or <tt>nullptr</tt> if this map contains no mapping for the <tt>key</tt>.
     *
     * @param key – the <tt>key</tt> whose associated <tt>value</tt> is to be returned
     * @param defaultValue – the default mapping of the <tt>key</tt>
     * @return the <tt>value</tt> to which the specified <tt>key</tt> is mapped, or <tt>nullptr</tt> if this map contains no mapping for the <tt>key</tt>
     */
    V getOrNullPtr(K const key) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->key == key) {
                return (*tracer)->value;
            }
            tracer = &(*tracer)->next;
        }
        return nullptr;
    }

    /**
     * <br/>
     * Removes the mapping for the specified key from this map if present.
     *
     * @param key – key whose mapping is to be removed from the map
     * @return true if the value was removed
     */
    bool remove(K const key) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->key == key) {
                KeyValuePair<K, V> *pNodeToDelete = *tracer;
                *tracer = (*tracer)->next;
                delete pNodeToDelete;
                pairCount--;
                return true;
            }
            tracer = &(*tracer)->next;
        }
        return false;
    }

    /**
     * <br/>
     * Removes the entry for the specified key only if it is currently mapped to the specified value.
     *
     * @param key – key with which the specified value is associated
     * @param value – value expected to be associated with the specified <tt>key</tt>
     * @return true if the value was removed
     */
    bool remove(K const key, V const value) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->key == key && (*tracer)->value == value) {
                KeyValuePair<K, V> *pKeyValuePairToDelete = *tracer;
                *tracer = (*tracer)->next;
                delete pKeyValuePairToDelete;
                pairCount--;
                return true;
            }
            tracer = &(*tracer)->next;
        }
        return false;
    }

    /**
     * <br/>
     * Removes all <tt>key</tt>-<tt>value</tt> pairs from the map.
     *
     * @return <tt>true</tt> if map size is zero
     */
    bool removeAll() {
        LinkedHashMap<K, V>::clear();
        return (pairCount == 0);
    }

    /**
     * <br/>
     * Replaces the entry for the specified key only if it is currently mapped to some value.
     *
     * @param key – key with which the specified <tt>value</tt> is associated
     * @param value – value to be associated with the specified <tt>key</tt>
     * @return true if the value was replaced
     */
    bool replace(K const key, V const value) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->key == key) {
                (*tracer)->value = value;
                return true;
            }
            tracer = &(*tracer)->next;
        }
        return false;
    }

    /**
     * <br/>
     * Replaces the entry for the specified key only if currently mapped to the specified value.
     *
     * @param key – key with which the specified value is associated
     * @param oldValue – value expected to be associated with the specified key
     * @param newValue – value to be associated with the specified key
     * @return true if the value was replaced
     */
    bool replace(K const key, V const oldValue, V const newValue) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->key == key && (*tracer)->value == oldValue) {
                (*tracer)->value = newValue;
                return true;
            }
            tracer = &(*tracer)->next;
        }
        return false;
    }

    /**
     * <br/>
     * Returns <tt>true</tt> if this map contains a mapping for the specified key.
     *
     * @param key – the <tt>key</tt> whose presence in this map is to be tested
     * @return <tt>true</tt> if this map contains a mapping for the specified <tt>key</tt>.
     */
    bool containsKey(K const key) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->key == key) {
                return true;
            }
            tracer = &(*tracer)->next;
        }
        return false;
    }

    /**
     * <br/>
     * Returns <tt>true</tt> if this map maps one or more keys to the specified value.
     *
     * @param value – value whose presence in this map is to be tested
     * @return <tt>true</tt> if this map maps one or more keys to the specified <tt>value</tt>.
     */
    bool containsValue(V const value) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            if ((*tracer)->value == value) {
                return true;
            }
            tracer = &(*tracer)->next;
        }
        return false;
    }

    /**
     * <br/>
     * Performs the given action for each entry in this map until all entries have been processed.<br/>
     * Use this variant with pairs of primitive or pointer elements.<br/>
     * <pre>
     * map.forEach([](int k, int v){ println(k + v); });<br/>
     * map.forEach([](KeyType *k, ValueType *v){ println(k->toString() + v->toString()); });
     * </pre>
     * <br/><strong>Warning <tt>(╯︵╰,)</tt>:</strong> <tt>action(...)</tt> cannot be lambda with captures.
     *
     * @param action – the action to be performed for each entry
     */
    void forEach(void action(const K key, const V value)) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            KeyValuePair<K, V> *pNode = *tracer;
            action(pNode->key, pNode->value);
            tracer = &(*tracer)->next;
        }
    }

    /**
     * <br/>
     * Performs the given action for each entry in this map until all entries have been processed.<br/>
     * Use this variant with pairs of non-primitive or non-pointer elements.<br/>
     * <pre>
     * map.forEach([](KeyType k, ValueType v){ println(k.toString() + v.toString()); });
     * </pre>
     * <br/><strong>Warning <tt>(╯︵╰,)</tt>:</strong> <tt>action(...)</tt> cannot be lambda with captures.
     *
     * @param action – the action to be performed for each entry
     */
    void forEach(void action(K &key, V &value)) {
        KeyValuePair<K, V> **tracer = &mapHead;
        while (*tracer) {
            KeyValuePair<K, V> *pNode = *tracer;
            action(pNode->key, pNode->value);
            tracer = &(*tracer)->next;
        }
    }
};

#endif