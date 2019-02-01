#define __TEST_MODE__

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <random>


#include "DosingStation/DosingPort.h"

#include "../_Mocks/MockStorage.h"

uint32_t getRandomUint32() {
    // https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range

    std::random_device rd;                                       // only used once to initialise (seed) engine
    std::mt19937 rng(rd());                                      // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<uint32_t> uni(0, 4294967295);  // guaranteed unbiased

    return uni(rng);
}

MockStorage *mockStoragePointer = new MockStorage();

DosingPort dosingPump = DosingPort(1, mockStoragePointer);

int main() {
    dosingPump.setup();

    return 0;
};
