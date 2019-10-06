#include <assert.h>
#include <chrono>

#include <iostream>

#include "../_Mocks/MockCommon.h"
#include <Abstract/AbstractRunnable.h>

#include "../_Mocks/Switchable.h"
#include "../_Mocks/MockPushButton.h"

static void setup() {
    AbstractRunnable::setupAll();
    ++currentMillis;
}

static void loop() {
    AbstractRunnable::loopAll();
    ++currentMillis;
}

static void loop(uint32_t forwardMs) {
    for (uint32_t ms = 0; ms < forwardMs; ++ms) {
        loop();
    }
}

static void shouldDebounceOnButtonPushShorterThanDebounceMs_DefaultConstructor() {
    /* given */
    currentMillis = getRandomUint32();
    MockPushButton pushButton{};
    loop(99);

    /* when */
    pushButton.push();
    loop(50);
    pushButton.release();

    /* then */
    assert(pushButton.getClick() == Click::NoClick);

    std::cout << "ok -> shouldDebounceOnButtonPushShorterThanDebounceMs_DefaultConstructor\n";
}

static void shouldShortClickOnButtonPushLongerThanDebounceMs_DefaultConstructor() {
    /* given */
    currentMillis = getRandomUint32();
    MockPushButton pushButton{};
    loop(99);

    /* when */
    pushButton.push();
    loop(51);
    pushButton.release();

    /* then */
    assert(pushButton.getClick() == Click::Short);

    std::cout << "ok -> shouldShortClickOnButtonPushLongerThanDebounceMs_DefaultConstructor\n";
}

static void shouldShortClickOnButtonPushShorterThanShortClickMs_DefaultConstructor() {
    /* given */
    currentMillis = getRandomUint32();
    MockPushButton pushButton{};
    loop(99);

    /* when */
    pushButton.push();
    loop(250);
    pushButton.release();

    /* then */
    assert(pushButton.getClick() == Click::Short);

    std::cout << "ok -> shouldShortClickOnButtonPushShorterThanShortClickMs_DefaultConstructor\n";
}

static void shouldLongClickOnButtonPushLongerThanShortClickMs_DefaultConstructor() {
    /* given */
    currentMillis = getRandomUint32();
    MockPushButton pushButton{};
    loop(99);

    /* when */
    pushButton.push();
    loop(251);
    pushButton.release();

    /* then */
    assert(pushButton.getClick() == Click::Long);

    std::cout << "ok -> shouldLongClickOnButtonPushLongerThanShortClickMs_DefaultConstructor\n";
}

static void shouldDebounceOnButtonPushShorterThanDebounceMs() {
    /* given */
    currentMillis = getRandomUint32();
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
    currentMillis = getRandomUint32();
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
    currentMillis = getRandomUint32();
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
    currentMillis = getRandomUint32();
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

        shouldDebounceOnButtonPushShorterThanDebounceMs_DefaultConstructor();
        shouldShortClickOnButtonPushLongerThanDebounceMs_DefaultConstructor();
        shouldShortClickOnButtonPushShorterThanShortClickMs_DefaultConstructor();
        shouldLongClickOnButtonPushLongerThanShortClickMs_DefaultConstructor();

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
