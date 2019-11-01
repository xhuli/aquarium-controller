#include <assert.h>
#include <chrono>

#include <iostream>

#include <Abstract/AbstractRunnable.h>
#include "Common/Switchable.h"

#include "../_Mocks/MockCommon.h"
#include "../_Mocks/MockPushButton.h"

static void setup() {
    AbstractRunnable::setupAll();
}

static void loop() {
    AbstractRunnable::loopAll();
}

static void loop(uint32_t forwardMs) {
    for (uint32_t ms = 0; ms < forwardMs; ++ms) {
        loop();
    }
}

static void shouldDebounceOnButtonPushShorterThanDebounceMs() {
    /* given */
    MockPushButton pushButton{Switched::Off, 750, 1250};
    loop(99);

    /* when */
    pushButton.push();
    loop(750);
    pushButton.release();

    /* then */
    assert(pushButton.getClick() == Click::NoClick);

    std::cout << "ok -> shouldDebounceOnButtonPushShorterThanDebounceMs\n";
}

static void shouldShortClickOnButtonPushLongerThanDebounceMs() {
    /* given */
    MockPushButton pushButton{Switched::Off, 750, 1250};
    loop(99);

    /* when */
    pushButton.push();
    loop(751);
    pushButton.release();

    /* then */
    assert(pushButton.getClick() == Click::Short);

    std::cout << "ok -> shouldShortClickOnButtonPushLongerThanDebounceMs\n";
}

static void shouldShortClickOnButtonPushShorterThanShortClickMs() {
    /* given */
    MockPushButton pushButton{Switched::Off, 750, 1250};
    loop(99);

    /* when */
    pushButton.push();
    loop(1250);
    pushButton.release();

    /* then */
    assert(pushButton.getClick() == Click::Short);

    std::cout << "ok -> shouldShortClickOnButtonPushShorterThanShortClickMs\n";
}

static void shouldLongClickOnButtonPushLongerThanShortClickMs() {
    /* given */
    MockPushButton pushButton{Switched::Off, 750, 1250};
    loop(99);

    /* when */
    pushButton.push();
    loop(1251);
    pushButton.release();

    /* then */
    assert(pushButton.getClick() == Click::Long);

    std::cout << "ok -> shouldLongClickOnButtonPushLongerThanShortClickMs\n";
}

int main() {

    std::cout << "\n"
              << "------------------------------------------------------------" << "\n"
              << " >> TEST START" << "\n"
              << "------------------------------------------------------------" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    const int repeat = 1;

    for (int i = 0; i < repeat; ++i) {

        shouldDebounceOnButtonPushShorterThanDebounceMs();
        shouldShortClickOnButtonPushLongerThanDebounceMs();
        shouldShortClickOnButtonPushShorterThanShortClickMs();
        shouldLongClickOnButtonPushLongerThanShortClickMs();
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "\n"
                 "------------------------------------------------------------" << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    std::cout << "------------------------------------------------------------" << "\n"
              << " >> TEST END" << "\n"
              << "------------------------------------------------------------" << "\n"
              << "\n";

    return 0;
}
