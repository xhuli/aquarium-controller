#include <cassert>
#include <iostream>
#include <chrono>

#include <Common/RingBuffer.h>
#include <Enums/Level.h>

struct TestStruct {
    uint8_t x = 0;
    uint8_t y = 0;

    TestStruct() : x(0), y(0) {}

    TestStruct(uint8_t x, uint8_t y) : x(x), y(y) {}

    TestStruct(TestStruct const &rhs) {
        *this = rhs;
    }

    explicit operator int() const { return sqrt(x * x + y * y); }

    TestStruct &operator=(const TestStruct &rhs) {
        if (this != &rhs) {
            x = rhs.x;
            y = rhs.y;
        }
        return *this;
    }

    TestStruct &operator/(uint8_t rhs) &{
        x /= rhs;
        y /= rhs;
        return *this;
    }

    TestStruct &operator/(TestStruct rhs) &{
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    TestStruct &operator/=(TestStruct rhs) &{
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    friend TestStruct &operator/(TestStruct &lhs, TestStruct rhs) {
        lhs /= rhs;
        return lhs;
    }

    TestStruct &operator+(TestStruct const &rhs) &{
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    TestStruct &operator+=(TestStruct const &rhs) &{
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    friend TestStruct &operator+(TestStruct &lhs, TestStruct const &rhs) {
        lhs += rhs;
        return lhs;
    }

    bool operator==(TestStruct const &rhs) {
        return (x == rhs.x && y == rhs.y);
    }

    friend bool operator==(TestStruct const &lhs, TestStruct const &rhs) {
        return (lhs.x == rhs.x && lhs.y == rhs.y);
    }
};

static void shouldConstructRingBuffer() {
    /* when */
    RingBuffer<int, 3> buffer{};

    /* then */
    assert(buffer.size() == 3);
    assert(buffer.getAverage() == 0);

    std::cout << "ok -> shouldConstructRingBuffer\n";
}

static void shouldAssignRingBuffer() {
    /* given */
    RingBuffer<int, 3> oldBuffer{};

    /* when */
    RingBuffer<int, 3> newBuffer;
    newBuffer = oldBuffer;
    newBuffer.add(2);

    /* then */
    assert(newBuffer.size() == 3);
    assert(newBuffer.getAverage() == 2);
    assert(oldBuffer.getAverage() == 0); // !!!

    std::cout << "ok -> shouldAssignRingBuffer\n";
}

static void shouldCopyRingBuffer() {
    /* given */
    RingBuffer<int, 3> oldBuffer{};

    /* when */
    RingBuffer<int, 3> newBuffer{oldBuffer};
    newBuffer.add(2);

    /* then */
    assert(newBuffer.size() == 3);
    assert(newBuffer.getAverage() == 2);
    assert(oldBuffer.getAverage() == 0); // !!!

    std::cout << "ok -> shouldCopyRingBuffer\n";
}

static void shouldAddValuesToTheRingBuffer() {
    /* given */
    RingBuffer<int, 3> buffer{};

    /* when */
    buffer.add(3);
    buffer.add(7);

    /* then */
    assert(buffer.size() == 3);
    assert(buffer.getAverage() == 5);

    std::cout << "ok -> shouldAddValuesToTheRingBuffer\n";
}

static void shouldDropOldValuesFromTheRingBuffer() {
    /* given */
    RingBuffer<int, 3> buffer{};

    /* when */
    buffer.add(1000);
    buffer.add(3);
    buffer.add(3);
    buffer.add(3);

    /* then */
    assert(buffer.size() == 3);
    assert(buffer.getAverage() == 3);

    std::cout << "ok -> shouldDropOldValuesFromTheRingBuffer\n";
}

static void shouldGetAverageForAllZeroValues() {
    /* given */
    RingBuffer<int, 3> buffer{};

    /* when */
    buffer.add(1000);
    buffer.add(0);
    buffer.add(0);
    buffer.add(0);

    /* then */
    assert(buffer.size() == 3);
    assert(buffer.getAverage() == 0);

    std::cout << "ok -> shouldGetAverageForAllZeroValues\n";
}

static void shouldGetAverageForNegativeValues() {
    /* given */
    RingBuffer<int, 3> buffer{};

    /* when */
    buffer.add(1000);
    buffer.add(-8);
    buffer.add(1);
    buffer.add(1);

    /* then */
    assert(buffer.size() == 3);
    assert(buffer.getAverage() == -2);

    std::cout << "ok -> shouldGetAverageForNegativeValues\n";
}

static void shouldGetRoundedAverageForFloatValues() {
    /* given */
    RingBuffer<float, 3> buffer{};

    /* when */
    buffer.add(1000);
    buffer.add(-8.0f);
    buffer.add(1.1);
    buffer.add(1.1);

    /* then */
    assert(buffer.size() == 3);
    assert(buffer.getAverage() == -2.0);

    std::cout << "ok -> shouldGetRoundedAverageForFloatValues\n";
}

//static void shouldSupportCustomTypes() {
//    /* given */
//    RingBuffer<Level, 3> buffer{};
//
//    /* when */
//    buffer.add(Level::High);
//    buffer.add(Level::High);
//
//    /* then */
//    assert(buffer.size() == 3);
//    assert(buffer.getAverage() == Level::High);
//
//    std::cout << "ok -> shouldSupportCustomTypes\n";
//}
//
//static void shouldCalculateAverageForCustomTypes1() {
//    /* given */
//    RingBuffer<Level, 3> buffer{};
//
//    /* when */
//    buffer.add(Level::High);
//    buffer.add(Level::High);
//    buffer.add(Level::Low);
//
//    /* then */
//    assert(buffer.size() == 3);
//    assert(buffer.getAverage() == Level::High);
//
//    std::cout << "ok -> shouldCalculateAverageForCustomTypes1\n";
//}
//
//static void shouldCalculateAverageForCustomTypes2() {
//    /* given */
//    RingBuffer<Level, 3> buffer{};
//
//    /* when */
//    buffer.add(Level::Low);
//    buffer.add(Level::High);
//    buffer.add(Level::Low);
//
//    /* then */
//    assert(buffer.size() == 3);
//    assert(buffer.getAverage() == Level::Low);
//
//    std::cout << "ok -> shouldCalculateAverageForCustomTypes2\n";
//}
//
//static void shouldCalculateAverageForCustomTypes3() {
//    /* given */
//    RingBuffer<Level, 3> buffer{};
//
//    /* when */
//    buffer.add(Level::Low);        // 0
//    buffer.add(Level::High);       // 1
//    buffer.add(Level::Low);        // 0
//    buffer.add(Level::Unknown);    // 2
//    buffer.add(Level::Unknown);    // 2
//
//    /* then */
//    assert(buffer.size() == 3);
//    assert(buffer.getAverage() == Level::High);
//
//    std::cout << "ok -> shouldCalculateAverageForCustomTypes3\n";
//}
//
//static void shouldCalculateAverageForCustomTypes4() {
//    /* given */
//    RingBuffer<TestStruct, 3> buffer{};
//
//    /* when */
//    buffer.add(TestStruct{});
//    buffer.add(TestStruct{2, 1});
//    buffer.add(TestStruct{1, 2});
//
//    /* then */
//    assert(buffer.size() == 3);
//    const TestStruct &average = buffer.getAverage();
//    assert(average.x == 1);
//    assert(average.y == 1);
//
//    std::cout << "ok -> shouldCalculateAverageForCustomTypes4\n";
//}

int main() {

    std::cout << "\n"
              << "------------------------------------------------------------" << "\n"
              << " >> TEST START" << "\n"
              << "------------------------------------------------------------" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    const int repeat = 1;

    for (int i = 0; i < repeat; ++i) {

        shouldConstructRingBuffer();
        shouldAssignRingBuffer();
        shouldCopyRingBuffer();
        shouldAddValuesToTheRingBuffer();
        shouldDropOldValuesFromTheRingBuffer();
        shouldGetAverageForAllZeroValues();
        shouldGetAverageForNegativeValues();
        shouldGetRoundedAverageForFloatValues();

//        shouldSupportCustomTypes();
//        shouldCalculateAverageForCustomTypes1();
//        shouldCalculateAverageForCustomTypes2();
//        shouldCalculateAverageForCustomTypes3();
//        shouldCalculateAverageForCustomTypes4();
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
