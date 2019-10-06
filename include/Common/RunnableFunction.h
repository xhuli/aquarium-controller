#ifndef _AQUARIUM_CONTROLLER_INCLUDE_RUNNABLE_FUNCTION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_RUNNABLE_FUNCTION_H_
#pragma once

#include <Common/StorableFunction.h>
#include <Abstract/AbstractRunnable.h>

/**
 * <br/>
 * Can only reference global scope variables.<br/>
 * • void return type.<br/>
 * • no input arguments.<br/>
 * • no lambda closures.<br/>
 */
class RunnableFunction :
        public StorableFunction,
        public AbstractRunnable {

public:

    explicit RunnableFunction(
            void (*fun)()
    ) :
            StorableFunction(fun) {}

    void setup() override {};

    void loop() override {
        StorableFunction::invoke();
    }
};

#endif