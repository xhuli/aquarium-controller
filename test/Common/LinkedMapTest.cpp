#include <cassert>
#include <iostream>
#include <chrono>

#include <Common/LinkedMap.h>

static void shouldCreateLinkedMap() {
    /* when */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();

    /* then */
    assert(linkedMap.isEmpty());

    std::cout << "ok -> shouldCreateLinkedMap\n";
}

static void shouldPutKeyValuePairInToTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();

    /* when */
    linkedMap.put(12, 13);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 1);

    std::cout << "ok -> shouldPutKeyValuePairInToTheMap\n";
}

static void shouldPutValueIfTheKeyExistsInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);

    /* when */
    int newValue = 29;
    bool putSuccessful = linkedMap.put(22, newValue);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 2);
    assert(putSuccessful);
    assert(linkedMap.get(22) == newValue);

    std::cout << "ok -> shouldPutValueIfTheKeyExistsInTheMap\n";
}

static void shouldGetTheFirstValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int value = linkedMap.get(11);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(value == 10);

    std::cout << "ok -> shouldGetTheFirstValueInTheMap\n";
}

static void shouldGetTheSecondValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int value = linkedMap.get(22);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(value == 20);

    std::cout << "ok -> shouldGetTheSecondValueInTheMap\n";
}

static void shouldGetTheLastValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int value = linkedMap.get(33);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(value == 30);

    std::cout << "ok -> shouldGetTheLastValueInTheMap\n";
}

static void shouldNotGetValueForKeyNotInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int value = linkedMap.get(44);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(value == 0);

    std::cout << "ok -> shouldNotGetValueForKeyNotInTheMap\n";
}

static void shouldGetTheValueInsteadOfDefaultForKeyInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int value = linkedMap.getOrDefault(11, 19);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(value == 10);

    std::cout << "ok -> shouldGetTheValueInsteadOfDefaultForKeyInTheMap\n";
}

static void shouldGetTheDefaultValueForKeyNotInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int value = linkedMap.getOrDefault(12, 19);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(value == 19);

    std::cout << "ok -> shouldGetTheDefaultValueForKeyNotInTheMap\n";
}

static void shouldGetFloatValueForEnumKeyInTheMap() {
    /* given */

    enum class TestEnum : uint8_t {
        First,
        Second,
        Third,
    };

    LinkedMap<TestEnum, float> linkedMap = LinkedMap<TestEnum, float>();
    linkedMap.put(TestEnum::First, 1.1f);
    linkedMap.put(TestEnum::Second, 2.2f);
    linkedMap.put(TestEnum::Third, 3.3f);

    /* when */
    float value = linkedMap.get(TestEnum::Second);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(value == 2.2f);

    std::cout << "ok -> shouldGetFloatValueForEnumKeyInTheMap\n";
}

static void shouldGetEnumValueForFloatKeyInTheMap() {
    /* given */

    enum class TestEnum : uint8_t {
        First,
        Second,
        Third,
    };

    LinkedMap<float, TestEnum> linkedMap = LinkedMap<float, TestEnum>();
    linkedMap.put(1.1f, TestEnum::First);
    linkedMap.put(2.2f, TestEnum::Second);
    linkedMap.put(3.3f, TestEnum::Third);

    /* when */
    TestEnum value = linkedMap.get(1.1f);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(value == TestEnum::First);

    std::cout << "ok -> shouldGetEnumValueForFloatKeyInTheMap\n";
}

static void shouldTestMapDoesContainKey() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when & then */
    assert(linkedMap.containsKey(33));

    std::cout << "ok -> shouldTestMapDoesContainKey\n";
}

static void shouldTestMapDoesNotContainKey() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when & then */
    assert(!linkedMap.containsKey(10));

    std::cout << "ok -> shouldTestMapDoesNotContainKey\n";
}

static void shouldTestMapDoesContainValue() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);
    linkedMap.put(44, 20);

    /* when & then */
    assert(linkedMap.containsValue(20));

    std::cout << "ok -> shouldTestMapDoesContainValue\n";
}

static void shouldTestMapDoesNotContainValue() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when & then */
    assert(!linkedMap.containsValue(11));

    std::cout << "ok -> shouldTestMapDoesNotContainValue\n";
}

static void shouldReplaceTheFirstValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int newValue = 19;
    bool replaceSuccessful = linkedMap.replace(11, newValue);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(replaceSuccessful);
    assert(linkedMap.get(11) == newValue);

    std::cout << "ok -> shouldReplaceTheFirstValueInTheMap\n";
}

static void shouldReplaceTheSecondValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int newValue = 29;
    bool replaceSuccessful = linkedMap.replace(22, newValue);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(replaceSuccessful);
    assert(linkedMap.get(22) == newValue);

    std::cout << "ok -> shouldReplaceTheSecondValueInTheMap\n";
}

static void shouldReplaceTheLastValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int newValue = 39;
    bool replaceSuccessful = linkedMap.replace(33, newValue);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(replaceSuccessful);
    assert(linkedMap.get(33) == newValue);

    std::cout << "ok -> shouldReplaceTheLastValueInTheMap\n";
}

static void shouldNotReplaceAnyValueForKeyNotInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int newValue = 49;
    bool replaceSuccessful = linkedMap.replace(44, newValue);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(!replaceSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldNotReplaceAnyValueForKeyNotInTheMap\n";
}

static void shouldReplaceExactOldValueWithNewValueForKeyInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int newValue = 49;
    bool replaceSuccessful = linkedMap.replace(33, 30, newValue);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(replaceSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == newValue);

    std::cout << "ok -> shouldReplaceExactOldValueWithNewValueForKeyInTheMap\n";
}

static void shouldNotReplaceMismatchOldValueWithNewValueForKeyInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int newValue = 49;
    bool replaceSuccessful = linkedMap.replace(33, 31, newValue);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(!replaceSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldNotReplaceMismatchOldValueWithNewValueForKeyInTheMap\n";
}

static void shouldNotReplaceExactOldValueWithNewValueForKeyNotInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int newValue = 49;
    bool replaceSuccessful = linkedMap.replace(32, 30, newValue);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(!replaceSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldNotReplaceExactOldValueWithNewValueForKeyNotInTheMap\n";
}

static void shouldRemoveTheFirstPairInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedMap.remove(11);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldRemoveTheFirstPairInTheMap\n";
}

static void shouldRemoveTheSecondPairInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedMap.remove(22);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldRemoveTheSecondPairInTheMap\n";
}

static void shouldRemoveTheLastPairInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedMap.remove(33);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(22) == 20);

    std::cout << "ok -> shouldRemoveTheLastPairInTheMap\n";
}

static void shouldNotRemoveAnyPairsWhenKeyNotInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedMap.remove(44);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(!removeSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldNotRemoveAnyPairsWhenKeyNotInTheMap\n";
}

static void shouldRemoveTheFirstPairWithExactKeyValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedMap.remove(11, 10);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedMap.get(11) == int());
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldRemoveTheFirstPairWithExactKeyValueInTheMap\n";
}

static void shouldRemoveTheSecondPairWithExactKeyValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedMap.remove(22, 20);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(22) == int());
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldRemoveTheSecondPairWithExactKeyValueInTheMap\n";
}

static void shouldRemoveTheLastPairWithExactKeyValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedMap.remove(33, 30);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == int());

    std::cout << "ok -> shouldRemoveTheLastPairWithExactKeyValueInTheMap\n";
}

static void shouldRemoveAllPairsInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    bool removeAllSuccessful = linkedMap.removeAll();

    /* then */
    assert(linkedMap.isEmpty());
    assert(removeAllSuccessful);

    std::cout << "ok -> shouldRemoveAllPairsInTheMap\n";
}

static void shouldNotRemoveAnyPairsWithMismatchKeyAndExactValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedMap.remove(43, 30);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(!removeSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldNotRemoveAnyPairsWithMismatchKeyAndExactValueInTheMap\n";
}

static void shouldNotRemoveAnyPairsWithExactKeyAndMismatchValueInTheMap() {
    /* given */
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedMap.remove(33, 31);

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(!removeSuccessful);
    assert(linkedMap.get(11) == 10);
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldDebounceOnButtonPushShorterThanDebounceMs_DefaultConstructor\n";
}

static void actionAddKeyToValue(int key, int value) {
    std::cout << "\tkey(" << key << ") + value(" << value << ") = " << key + value << "\n";
}

static void shouldExecuteActionForEachPairInTheMap() {
    /* given */
    std::cout << "start -> shouldExecuteActionForEachPairInTheMap\n";
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    linkedMap.forEach(actionAddKeyToValue);

    linkedMap.forEach([](int key, int value) {
        std::cout << "\tkey(" << key << ") + value(" << value << ") = " << key + value << "\n";
    });

    /* then */
    assert(!linkedMap.isEmpty());
    assert(linkedMap.size() == 3);
    assert(linkedMap.get(11) == 10); // actual keys and values remain unchanged
    assert(linkedMap.get(22) == 20);
    assert(linkedMap.get(33) == 30);

    std::cout << "ok -> shouldExecuteActionForEachPairInTheMap\n";
}

static void shouldGetEachPairInTheMap() {
    /* given */
    std::cout << "start -> shouldExecuteActionForEachPairInTheMap\n";
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int key = 33;
    int value = 30;

    for (LinkedKeyValue<int, int> *pKeyValuePair = linkedMap.getFirstPair();
         pKeyValuePair;
         pKeyValuePair = pKeyValuePair->next) {

        /* then */
        assert(linkedMap.get(key) == pKeyValuePair->getValue());
        assert(linkedMap.get(pKeyValuePair->getKey()) == value);

        key -= 11;
        value -= 10;
    }

    std::cout << "ok -> shouldGetEachPairInTheMap\n";
}

static void shouldPutArrayPointerToKey() {
    /* given */
    uint8_t keyCandidate1[8] = {0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF1};
    uint8_t keyCandidate2[8] = {0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF2};

    LinkedMap<uint8_t *, int> linkedMap;
    linkedMap.put(*(&keyCandidate1), 1);
    linkedMap.put(*(&keyCandidate2), 2);

    /* when */
    uint8_t value1 = linkedMap.get(*(&keyCandidate1));
    uint8_t value2 = linkedMap.get(*(&keyCandidate2));

    /* then */
    assert(value1 == 1);
    assert(value2 == 2);

    std::cout << "ok -> shouldPutArrayPointerToKey\n";
}

static void shouldPutArrayPointerToValue() {
    /* given */
    uint8_t valueCandidate1[8] = {0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF1};
    uint8_t valueCandidate2[8] = {0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF2};

    LinkedMap<int, uint8_t *> linkedMap;
    linkedMap.put(1, *(&valueCandidate1));
    linkedMap.put(2, *(&valueCandidate2));

    /* when */
    uint8_t *value1 = linkedMap.get(1);
    uint8_t *value2 = linkedMap.get(2);

    /* then */
    for (int i = 0; i < 8; ++i) {
        assert(static_cast<int>(valueCandidate1[i]) == static_cast<int>(value1[i]));
        assert(static_cast<int>(valueCandidate2[i]) == static_cast<int>(value2[i]));
    }

    std::cout << "ok -> shouldPutArrayPointerToValue\n";
}

static void shouldUpdateProvidedVariableWithMappedValueForTheProvidedKey() {
    /* given */
    std::cout << "start -> shouldExecuteActionForEachPairInTheMap\n";
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    /* when */
    int key = 33;
    int value = 30;

    for (LinkedKeyValue<int, int> *pKeyValuePair = linkedMap.getFirstPair();
         pKeyValuePair;
         pKeyValuePair = pKeyValuePair->next) {

        /* then */
        int getValue;
        linkedMap.get(key, getValue);

        assert(getValue == value);

        key -= 11;
        value -= 10;
    }

    std::cout << "ok -> shouldUpdateProvidedVariableWithMappedValueForTheProvidedKey\n";
}

static void shouldIterateLinkedMapWithWhileLoop() {
    /* given */
    std::cout << "start -> shouldIterateLinkedMapWithWhileLoop\n";
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    MapIterator<int, int> const &mapIterator = linkedMap.iterator();


    while (mapIterator.hasNext()) {
        KeyValue<int, int> *keyValue = mapIterator.next();
        std::cout << "\tmap[" << keyValue->getKey() << "] = " << keyValue->getValue() << "\n";
    }

    std::cout << "ok -> shouldIterateLinkedMapWithWhileLoop\n";
}

static void shouldIterateLinkedMapWithForLoop() {
    /* given */
    std::cout << "start -> shouldIterateLinkedMapWithForLoop\n";
    LinkedMap<int, int> linkedMap = LinkedMap<int, int>();
    linkedMap.put(11, 10);
    linkedMap.put(22, 20);
    linkedMap.put(33, 30);

    for (MapIterator<int, int> const &mapIterator = linkedMap.iterator(); mapIterator.hasNext(); mapIterator.forward()) {
        KeyValue<int, int> *pKeyValue = mapIterator.getKeyValue();
        std::cout << "\tmap[" << pKeyValue->getKey() << "] = " << pKeyValue->getValue() << "\n";
    }

    std::cout << "ok -> shouldIterateLinkedMapWithForLoop\n";
}

int main() {

    std::cout << "\n"
              << "------------------------------------------------------------" << "\n"
              << " >> TEST START" << "\n"
              << "------------------------------------------------------------" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    const int repeat = 1;

    for (int i = 0; i < repeat; ++i) {

        shouldCreateLinkedMap();

        shouldPutKeyValuePairInToTheMap();
        shouldPutValueIfTheKeyExistsInTheMap();

        shouldGetTheFirstValueInTheMap();
        shouldGetTheSecondValueInTheMap();
        shouldGetTheLastValueInTheMap();
        shouldNotGetValueForKeyNotInTheMap();

        shouldGetTheValueInsteadOfDefaultForKeyInTheMap();
        shouldGetTheDefaultValueForKeyNotInTheMap();

        shouldGetFloatValueForEnumKeyInTheMap();
        shouldGetEnumValueForFloatKeyInTheMap();

        shouldTestMapDoesContainKey();
        shouldTestMapDoesNotContainKey();

        shouldTestMapDoesContainValue();
        shouldTestMapDoesNotContainValue();

        shouldReplaceTheFirstValueInTheMap();
        shouldReplaceTheSecondValueInTheMap();
        shouldReplaceTheLastValueInTheMap();
        shouldNotReplaceAnyValueForKeyNotInTheMap();

        shouldReplaceExactOldValueWithNewValueForKeyInTheMap();
        shouldNotReplaceMismatchOldValueWithNewValueForKeyInTheMap();
        shouldNotReplaceExactOldValueWithNewValueForKeyNotInTheMap();

        shouldRemoveTheFirstPairInTheMap();
        shouldRemoveTheSecondPairInTheMap();
        shouldRemoveTheLastPairInTheMap();
        shouldNotRemoveAnyPairsWhenKeyNotInTheMap();

        shouldRemoveTheFirstPairWithExactKeyValueInTheMap();
        shouldRemoveTheSecondPairWithExactKeyValueInTheMap();
        shouldRemoveTheLastPairWithExactKeyValueInTheMap();
        shouldRemoveAllPairsInTheMap();

        shouldNotRemoveAnyPairsWithMismatchKeyAndExactValueInTheMap();
        shouldNotRemoveAnyPairsWithExactKeyAndMismatchValueInTheMap();

        shouldExecuteActionForEachPairInTheMap();
        shouldGetEachPairInTheMap();

        shouldPutArrayPointerToKey();
        shouldPutArrayPointerToValue();

        shouldUpdateProvidedVariableWithMappedValueForTheProvidedKey();

        shouldIterateLinkedMapWithWhileLoop();
        shouldIterateLinkedMapWithForLoop();
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
