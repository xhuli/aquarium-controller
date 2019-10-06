#ifndef _AQUARIUM_CONTROLLER_INCLUDE_COMMON_LINKED_FUNCTION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_COMMON_LINKED_FUNCTION_H_
#pragma once

class LinkedFunction : public StorableFunction {

public:

    LinkedFunction *next;

    LinkedFunction(
            void (*fun)(),
            LinkedFunction *head
    ) :
            StorableFunction(fun),
            next(head) {}

    LinkedFunction *getNext() const {
        return next;
    }
};

#endif