#ifndef _AQUARIUM_CONTROLLER_INCLUDE_COMMON_FUNCTION_LIST_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_COMMON_FUNCTION_LIST_H_
#pragma once

#include <Common/StorableFunction.h>
#include <Common/LinkedFunction.h>

class FunctionList {

private:

    LinkedFunction *head = nullptr;
    uint8_t count = 0;

public:

    ~FunctionList() {
        removeAll();
    }

    uint8_t getCount() const {
        return count;
    }

    uint8_t size() {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }

    LinkedFunction *getFirst() const {
        return head;
    }

    void add(void (*fun)()) {
        auto *pLinkedGlobalFunction = new LinkedFunction{fun, head};
        head = pLinkedGlobalFunction;
        ++count;
    }

    void removeAll() {
        LinkedFunction **tracer = &head;
        while (*tracer) {
            LinkedFunction *pLinkedGlobalFunctionToDelete = *tracer;
            *tracer = (*tracer)->next;
            count--;
            delete pLinkedGlobalFunctionToDelete;
        }
    }

    void invokeAll() {
        LinkedFunction **tracer = &head;
        while (*tracer) {
            (*tracer)->invoke();
            tracer = &(*tracer)->next;
        }
    }
};

#endif