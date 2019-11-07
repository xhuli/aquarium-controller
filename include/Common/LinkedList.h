#ifndef _AQUARIUM_CONTROLLER_INCLUDE_COMMON_LINKED_LIST_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_COMMON_LINKED_LIST_H_
#pragma once

#include "Abstract/ListIterator.h"

template<typename V>
struct Element {

    Element<V> *next = nullptr;
    V value;

    explicit Element() : value(V{}) {}

    explicit Element(
            V value
    ) :
            value(value) {}

    Element<V> *getNext() const {
        return next;
    }
};

template<typename V>
class LinkedListIterator : public ListIterator<V> {

private:

    mutable Element<V> *pElement;

public:

    explicit LinkedListIterator(Element<V> *firstElement) : pElement(firstElement) {}

    virtual ~LinkedListIterator() = default;

    V next() const override {
        V value = pElement->value;
        pElement = pElement->next;
        return value;
    }

    bool hasNext() const override {
        return pElement != nullptr;
    }

    V getValue() const override {
        return pElement->value;
    }

    void forward() const override {
        pElement = pElement->next;
    }
};

template<typename V>
class LinkedList {

protected:
    Element<V> *head = nullptr;
    uint8_t count = 0;

public:
    explicit LinkedList() = default;

    virtual ~LinkedList() {
        LinkedList<V>::clear();
    }

    Element<V> *getFirstElement() {
        return head;
    }

    /**
     * <br/>
     * Returns the number of elements in this list.
     * @return the number of elements in this list
     */
    uint8_t size() const {
        return count;
    }

    /**
     * <br/>
     * Returns <tt>true</tt> if this list contains no elements.
     *
     * @return <tt>true</tt> if this list contains no elements
     */
    bool isEmpty() const {
        return count == 0;
    }

    /**
     * <br/>
     * Removes all the elements from this list (optional operation).<br/>
     * The list will be empty after this call returns.
     */
    void clear() {
        Element<V> **tracer = &head;
        while (*tracer) {
            Element<V> *pElementToDelete = *tracer;
            *tracer = (*tracer)->next;
            delete pElementToDelete;
            count--;
        }
    }

    /**
     * Appends the specified element to the end of this list.
     *
     * @param value – value to be appended to this list
     * @return <tt>true</tt> if the element was put into the list
     */
    bool add(V const &value) {
        Element<V> **tracer = &head;
        while (*tracer) {
            tracer = &(*tracer)->next;
        }
        if (*tracer == nullptr) {
            *tracer = new Element<V>(value);
        } else {
            head = new Element<V>(value);
        }
        ++count;
        return true;
    }

    /**
     * <br/>
     * Replaces the value at the specified position in this list with the specified value (optional operation).<br/>
     *
     * @param index – index of the value to replace
     * @param value – value to be stored at the specified position
     * @return <tt>true</tt> if the value was set
     */
    bool set(int8_t const index, V const &value) {
        Element<V> **tracer = &head;

        if (index >= 0) {
            /* count from the beginning of the list */
            if (index > LinkedList<V>::size()) { return false; }
            uint8_t i = 0;
            while (i < index) {
                tracer = &(*tracer)->next;
                ++i;
            }
        } else {
            /* count from the end of the list */
            /* warn: index value is negative! */
            if (index + LinkedList<V>::size() < 0) { return false; }
            uint8_t i = 0;
            while (i < LinkedList<V>::size() + index) {
                tracer = &(*tracer)->next;
                ++i;
            }
        }

        if (*tracer != nullptr) {
            (*tracer)->value = value;
            return true;
        }

        return false;
    }

    /**
     * <br/>
     * Returns the index of the first occurrence of the specified value in this list, or -1 if this list does not contain the value.
     *
     * @param value – value to search for
     * @return the index of the first occurrence of the specified value in this list, or -1 if this list does not contain the value
     */
    int8_t indexOf(V const &value) {
        Element<V> **tracer = &head;
        uint8_t i = 0;

        while (*tracer && i < count) {
            if ((*tracer)->value == value) {
                return i;
            }
            tracer = &(*tracer)->next;
            ++i;
        }

        return -1;
    }

    /**
     * <br/>
     * Returns the element at the specified position in this list.<br/>
     * Positive <tt>index</tt> values are counted from the beginning of the list.<br/>
     * Negative <tt>index</tt> values are counted from the end of the list.
     * <pre>
     * linkedList = {1, 2, 3, 4, 5};<br/>
     * linkedList.getOrElse(2, 13);     // -> 3<br/>
     * linkedList.getOrElse(-1, 13);    // -> 5<br/>
     * linkedList.getOrElse(5, 13);     // -> 13<br/>
     * linkedList.getOrElse(-6, 13);    // -> 13<br/>
     * </pre>
     *
     * @param index – index of the element to return
     * @return the element at the specified position in this list
     */
    V getOrElse(int8_t const index, V const &orElse) {
        Element<V> **tracer = &head;

        if (index >= 0) {
            /* count from the beginning of the list */
            if (index > LinkedList<V>::size()) { return orElse; } // <- return provided `orElse` value
            uint8_t i = 0;
            while (i < index) {
                tracer = &(*tracer)->next;
                ++i;
            }
        } else {
            /* count from the end of the list */
            /* note: index has negative value */
            if (index + LinkedList<V>::size() < 0) { return orElse; } // <- return provided `orElse` value
            uint8_t i = 0;
            while (i < LinkedList<V>::size() + index) {
                tracer = &(*tracer)->next;
                ++i;
            }
        }

        if (*tracer != nullptr) {
            return (*tracer)->value;
        }

        return orElse; // <- return provided `orElse` value
    }

    /**
     * <br/>
     * Returns the element at the specified position in this list.<br/>
     * Positive <tt>index</tt> values are counted from the beginning of the list.<br/>
     * Negative <tt>index</tt> values are counted from the end of the list.
     * <pre>
     * linkedList = {1, 2, 3, 4, 5};<br/>
     * linkedList.get(2);   // -> 3<br/>
     * linkedList.get(-1);  // -> 5<br/>
     * linkedList.get(6);   // -> 0<br/>
     * linkedList.get(-5);  // -> 0<br/>
     * </pre>
     *
     * @param index – index of the element to return
     * @return the element at the specified position in this list
     */
    V get(int8_t const index) {
        return LinkedList<V>::getOrElse(index, V{});
    }

    /**
     * <br/>
     * Returns <tt>true</tt> if this list contains the specified element.
     *
     * @param value – value which presence in this list is to be tested
     * @return <tt>true</tt> if this list contains the specified element
     */
    bool contains(V const &value) {
        Element<V> **tracer = &head;
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
     * Removes the element at the specified position in this list.<br/>
     * Positive <tt>index</tt> values are counted from the beginning of the list.<br/>
     * Negative <tt>index</tt> values are counted from the end of the list.
     * <pre>
     * linkedList = {0, 1, 2, 3, 4, 5};<br/>
     * linkedList.remove(0);    // {1, 2, 3, 4, 5}<br/>
     * linkedList.remove(-1);   // {1, 2, 3, 4}<br/>
     * </pre>
     *
     * @param index – index of the element to remove
     * @return <tt>true</tt> if the element was removed
     */
    bool remove(int8_t const index) {
        Element<V> **tracer = &head;
        uint8_t i = 0;

        if (index >= 0) {
            /* count from the beginning of the list */
            if (index > LinkedList<V>::size()) { return false; }
            while (*tracer && i < index) {
                tracer = &(*tracer)->next;
                ++i;
            }
        } else {
            /* count from the end of the list */
            /* note: index has negative value */
            if (index + LinkedList<V>::size() < 0) { return false; }
            while (i < LinkedList<V>::size() + index) {
                tracer = &(*tracer)->next;
                ++i;
            }
        }
        if (*tracer != nullptr) {
            Element<V> *pElementToDelete = *tracer;
            *tracer = (*tracer)->next;
            delete pElementToDelete;
            --count;
            return true;
        }
        return false;
    }

    /**
     * <br/>
     * Removes the first occurrence of the specified element from this list.<br/>
     * Does not work for primitive types.
     *
     * @param value – value to be removed from this list, if present
     * @return true if the value was removed
     */
    bool remove(V &value) {
        Element<V> **tracer = &head;
        while (*tracer) {
            if ((*tracer)->value == value) {
                Element<V> *elementToDelete = *tracer;
                *tracer = (*tracer)->next;
                delete elementToDelete;
                --count;
                return true;
            }
            tracer = &(*tracer)->next;
        }
        return false;
    }

    /**
     * <br/>
     * Removes all elements from the list.
     *
     * @return <tt>true</tt> if list size is zero
     */
    bool removeAll() {
        LinkedList<V>::clear();
        return (count == 0);
    }

    /**
     * <br/>
     * Performs the given action for each element until all elements have been processed.<br/>
     * Use this variant with primitive or pointer elements.<br/>
     * <pre>
     * map.forEach([](int value){ println(value); });<br/>
     * map.forEach([](V *pValue){ println(pValue->toString()); });
     * </pre>
     * <br/><strong>Warning <tt>(╯︵╰,)</tt>:</strong> <tt>action(...)</tt> cannot be lambda with captures.
     *
     * @param action – the action to be performed for each element
     */
    void forEach(void action(const V value)) {
        Element<V> **tracer = &head;
        while (*tracer) {
            action((*tracer)->value);
            tracer = &(*tracer)->next;
        }
    }

    /**
     * <br/>
     * Performs the given action for each element until all elements have been processed.<br/>
     * Use this variant with non-pointer elements.<br/>
     * <pre>
     * map.forEach([](V value){ println(value.toString()); });
     * </pre>
     * <br/><strong>Warning <tt>(╯︵╰,)</tt>:</strong> <tt>action(...)</tt> cannot be lambda with captures.
     *
     * @param action – the action to be performed for each element
     */
    void forEach(void action(const V &value)) {
        Element<V> **tracer = &head;
        while (*tracer) {
            action((*tracer)->value);
            tracer = &(*tracer)->next;
        }
    }

    LinkedListIterator<V> iterator() {
        return LinkedListIterator<V>(LinkedList::getFirstElement());
    };
};

#endif