#ifndef _AQUARIUM_CONTROLLER_INCLUDE_COMMON_STORABLE_FUNCTION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_COMMON_STORABLE_FUNCTION_H_
#pragma once

class StorableFunction {
private:

    void (*f)();

public:

    explicit StorableFunction(
            void (*fun)()
    ) :
            f(fun) {}

    virtual ~StorableFunction() = default;

    void invoke() const {
        f();
    }
};

#endif