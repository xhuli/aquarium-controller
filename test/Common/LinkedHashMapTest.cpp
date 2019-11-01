#include <cassert>
#include <iostream>
#include <chrono>

#include <Common/LinkedHashMap.h>
#include <Common/LinkedList.h>

static void shouldCreateLinkedHashMap() {
    /* when */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();

    /* then */
    assert(linkedHashMap.isEmpty());

    std::cout << "ok -> shouldCreateLinkedHashMap\n";
}

static void shouldPutKeyValuePairInToTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();

    /* when */
    linkedHashMap.put(12, 13);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 1);

    std::cout << "ok -> shouldPutKeyValuePairInToTheHashMap\n";
}

static void shouldPutValueIfTheKeyExistsInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);

    /* when */
    int newValue = 29;
    bool putSuccessful = linkedHashMap.put(22, newValue);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 2);
    assert(putSuccessful);
    assert(linkedHashMap.get(22) == newValue);

    std::cout << "ok -> shouldPutValueIfTheKeyExistsInTheHashMap\n";
}

static void shouldGetTheFirstValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int value = linkedHashMap.get(11);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(value == 10);

    std::cout << "ok -> shouldGetTheFirstValueInTheHashMap\n";
}

static void shouldGetTheSecondValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int value = linkedHashMap.get(22);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(value == 20);

    std::cout << "ok -> shouldGetTheSecondValueInTheHashMap\n";
}

static void shouldGetTheLastValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int value = linkedHashMap.get(33);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(value == 30);

    std::cout << "ok -> shouldGetTheLastValueInTheHashMap\n";
}

static void shouldNotGetValueForKeyNotInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int value = linkedHashMap.get(44);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(value == 0);

    std::cout << "ok -> shouldNotGetValueForKeyNotInTheHashMap\n";
}

static void shouldGetTheValueInsteadOfDefaultForKeyInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int value = linkedHashMap.getOrDefault(11, 19);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(value == 10);

    std::cout << "ok -> shouldGetTheValueInsteadOfDefaultForKeyInTheHashMap\n";
}

static void shouldGetTheDefaultValueForKeyNotInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int value = linkedHashMap.getOrDefault(12, 19);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(value == 19);

    std::cout << "ok -> shouldGetTheDefaultValueForKeyNotInTheHashMap\n";
}

static void shouldGetFloatValueForEnumKeyInTheHashMap() {
    /* given */

    enum class TestEnum : uint8_t {
        First,
        Second,
        Third,
    };

    LinkedHashMap<TestEnum, float> linkedHashMap = LinkedHashMap<TestEnum, float>();
    linkedHashMap.put(TestEnum::First, 1.1f);
    linkedHashMap.put(TestEnum::Second, 2.2f);
    linkedHashMap.put(TestEnum::Third, 3.3f);

    /* when */
    float value = linkedHashMap.get(TestEnum::Second);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(value == 2.2f);

    std::cout << "ok -> shouldGetFloatValueForEnumKeyInTheHashMap\n";
}

static void shouldGetEnumValueForFloatKeyInTheHashMap() {
    /* given */

    enum class TestEnum : uint8_t {
        First,
        Second,
        Third,
    };

    LinkedHashMap<float, TestEnum> linkedHashMap = LinkedHashMap<float, TestEnum>();
    linkedHashMap.put(1.1f, TestEnum::First);
    linkedHashMap.put(2.2f, TestEnum::Second);
    linkedHashMap.put(3.3f, TestEnum::Third);

    /* when */
    TestEnum value = linkedHashMap.get(1.1f);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(value == TestEnum::First);

    std::cout << "ok -> shouldGetEnumValueForFloatKeyInTheHashMap\n";
}

static void shouldTestMapDoesContainKey() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when & then */
    assert(linkedHashMap.containsKey(33));

    std::cout << "ok -> shouldTestMapDoesContainKey\n";
}

static void shouldTestMapDoesNotContainKey() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when & then */
    assert(!linkedHashMap.containsKey(10));

    std::cout << "ok -> shouldTestMapDoesNotContainKey\n";
}

static void shouldTestMapDoesContainValue() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);
    linkedHashMap.put(44, 20);

    /* when & then */
    assert(linkedHashMap.containsValue(20));

    std::cout << "ok -> shouldTestMapDoesContainValue\n";
}

static void shouldTestMapDoesNotContainValue() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when & then */
    assert(!linkedHashMap.containsValue(11));

    std::cout << "ok -> shouldTestMapDoesNotContainValue\n";
}

static void shouldReplaceTheFirstValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int newValue = 19;
    bool replaceSuccessful = linkedHashMap.replace(11, newValue);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(replaceSuccessful);
    assert(linkedHashMap.get(11) == newValue);

    std::cout << "ok -> shouldReplaceTheFirstValueInTheHashMap\n";
}

static void shouldReplaceTheSecondValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int newValue = 29;
    bool replaceSuccessful = linkedHashMap.replace(22, newValue);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(replaceSuccessful);
    assert(linkedHashMap.get(22) == newValue);

    std::cout << "ok -> shouldReplaceTheSecondValueInTheHashMap\n";
}

static void shouldReplaceTheLastValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int newValue = 39;
    bool replaceSuccessful = linkedHashMap.replace(33, newValue);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(replaceSuccessful);
    assert(linkedHashMap.get(33) == newValue);

    std::cout << "ok -> shouldReplaceTheLastValueInTheHashMap\n";
}

static void shouldNotReplaceAnyValueForKeyNotInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int newValue = 49;
    bool replaceSuccessful = linkedHashMap.replace(44, newValue);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(!replaceSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldNotReplaceAnyValueForKeyNotInTheHashMap\n";
}

static void shouldReplaceExactOldValueWithNewValueForKeyInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int newValue = 49;
    bool replaceSuccessful = linkedHashMap.replace(33, 30, newValue);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(replaceSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == newValue);

    std::cout << "ok -> shouldReplaceExactOldValueWithNewValueForKeyInTheHashMap\n";
}

static void shouldNotReplaceMismatchOldValueWithNewValueForKeyInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int newValue = 49;
    bool replaceSuccessful = linkedHashMap.replace(33, 31, newValue);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(!replaceSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldNotReplaceMismatchOldValueWithNewValueForKeyInTheHashMap\n";
}

static void shouldNotReplaceExactOldValueWithNewValueForKeyNotInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int newValue = 49;
    bool replaceSuccessful = linkedHashMap.replace(32, 30, newValue);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(!replaceSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldNotReplaceExactOldValueWithNewValueForKeyNotInTheHashMap\n";
}

static void shouldRemoveTheFirstPairInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedHashMap.remove(11);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldRemoveTheFirstPairInTheHashMap\n";
}

static void shouldRemoveTheSecondPairInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedHashMap.remove(22);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldRemoveTheSecondPairInTheHashMap\n";
}

static void shouldRemoveTheLastPairInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedHashMap.remove(33);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(22) == 20);

    std::cout << "ok -> shouldRemoveTheLastPairInTheHashMap\n";
}

static void shouldNotRemoveAnyPairsWhenKeyNotInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedHashMap.remove(44);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(!removeSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldNotRemoveAnyPairsWhenKeyNotInTheHashMap\n";
}

static void shouldRemoveTheFirstPairWithExactKeyValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedHashMap.remove(11, 10);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedHashMap.get(11) == int());
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldRemoveTheFirstPairWithExactKeyValueInTheHashMap\n";
}

static void shouldRemoveTheSecondPairWithExactKeyValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedHashMap.remove(22, 20);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(22) == int());
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldRemoveTheSecondPairWithExactKeyValueInTheHashMap\n";
}

static void shouldRemoveTheLastPairWithExactKeyValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedHashMap.remove(33, 30);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 2);
    assert(removeSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == int());

    std::cout << "ok -> shouldRemoveTheLastPairWithExactKeyValueInTheHashMap\n";
}

static void shouldRemoveAllPairsInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    bool removeAllSuccessful = linkedHashMap.removeAll();

    /* then */
    assert(linkedHashMap.isEmpty());
    assert(removeAllSuccessful);

    std::cout << "ok -> shouldRemoveAllPairsInTheHashMap\n";
}

static void shouldNotRemoveAnyPairsWithMismatchKeyAndExactValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedHashMap.remove(43, 30);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(!removeSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldNotRemoveAnyPairsWithMismatchKeyAndExactValueInTheHashMap\n";
}

static void shouldNotRemoveAnyPairsWithExactKeyAndMismatchValueInTheHashMap() {
    /* given */
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    bool removeSuccessful = linkedHashMap.remove(33, 31);

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(!removeSuccessful);
    assert(linkedHashMap.get(11) == 10);
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldDebounceOnButtonPushShorterThanDebounceMs_DefaultConstructor\n";
}

static void actionAddKeyToValue(int key, int value) {
    std::cout << "\tkey(" << key << ") + value(" << value << ") = " << key + value << "\n";
}

static void shouldExecuteActionForEachPairInTheHashMap() {
    /* given */
    std::cout << "start -> shouldExecuteActionForEachPairInTheHashMap\n";
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    linkedHashMap.forEach(actionAddKeyToValue);

    linkedHashMap.forEach([](int key, int value) {
        std::cout << "\tkey(" << key << ") + value(" << value << ") = " << key + value << "\n";
    });

    /* then */
    assert(!linkedHashMap.isEmpty());
    assert(linkedHashMap.size() == 3);
    assert(linkedHashMap.get(11) == 10); // actual keys and values remain unchanged
    assert(linkedHashMap.get(22) == 20);
    assert(linkedHashMap.get(33) == 30);

    std::cout << "ok -> shouldExecuteActionForEachPairInTheHashMap\n";
}

static void shouldGetEachPairInTheHashMap() {
    /* given */
    std::cout << "start -> shouldExecuteActionForEachPairInTheHashMap\n";
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int key = 33;
    int value = 30;

    for (KeyValuePair<int, int> *pKeyValuePair = linkedHashMap.getFirstPair();
         pKeyValuePair;
         pKeyValuePair = pKeyValuePair->next) {

        /* then */
        assert(linkedHashMap.get(key) == pKeyValuePair->value);
        assert(linkedHashMap.get(pKeyValuePair->key) == value);

        key -= 11;
        value -= 10;
    }

    std::cout << "ok -> shouldGetEachPairInTheHashMap\n";
}

static void shouldPutArrayPointerToKey() {
    /* given */
    uint8_t keyCandidate1[8] = {0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF1};
    uint8_t keyCandidate2[8] = {0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF2};

    LinkedHashMap<uint8_t *, int> linkedHashMap;
    linkedHashMap.put(*(&keyCandidate1), 1);
    linkedHashMap.put(*(&keyCandidate2), 2);

    /* when */
    uint8_t value1 = linkedHashMap.get(*(&keyCandidate1));
    uint8_t value2 = linkedHashMap.get(*(&keyCandidate2));

    /* then */
    assert(value1 == 1);
    assert(value2 == 2);

    std::cout << "ok -> shouldPutArrayPointerToKey\n";
}

static void shouldPutArrayPointerToValue() {
    /* given */
    uint8_t valueCandidate1[8] = {0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF1};
    uint8_t valueCandidate2[8] = {0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF2};

    LinkedHashMap<int, uint8_t *> linkedHashMap;
    linkedHashMap.put(1, *(&valueCandidate1));
    linkedHashMap.put(2, *(&valueCandidate2));

    /* when */
    uint8_t *value1 = linkedHashMap.get(1);
    uint8_t *value2 = linkedHashMap.get(2);

    /* then */
    for (int i = 0; i < 8; ++i) {
        assert(static_cast<int>(valueCandidate1[i]) == static_cast<int>(value1[i]));
        assert(static_cast<int>(valueCandidate2[i]) == static_cast<int>(value2[i]));
    }

    std::cout << "ok -> shouldPutArrayPointerToValue\n";
}

static void shouldUpdateProvidedVariableWithMappedValueForTheProvidedKey() {
    /* given */
    std::cout << "start -> shouldExecuteActionForEachPairInTheHashMap\n";
    LinkedHashMap<int, int> linkedHashMap = LinkedHashMap<int, int>();
    linkedHashMap.put(11, 10);
    linkedHashMap.put(22, 20);
    linkedHashMap.put(33, 30);

    /* when */
    int key = 33;
    int value = 30;

    for (KeyValuePair<int, int> *pKeyValuePair = linkedHashMap.getFirstPair();
         pKeyValuePair;
         pKeyValuePair = pKeyValuePair->next) {

        /* then */
        int getValue;
        linkedHashMap.get(key, getValue);

        assert(getValue == value);

        key -= 11;
        value -= 10;
    }

    std::cout << "ok -> shouldUpdateProvidedVariableWithMappedValueForTheProvidedKey\n";
}

static void shouldTestLinkedList() {
    LinkedList<int> linkedList{};

    assert(linkedList.get(0) == 0);
    assert(linkedList.get(1) == 0);
    assert(linkedList.get(-1) == 0);

    linkedList.add(1);
    linkedList.add(2);
    linkedList.add(3);

    assert(linkedList.get(0) == 1);
    assert(linkedList.get(1) == 2);
    assert(linkedList.get(2) == 3);
    assert(linkedList.get(4) == 0);
    assert(linkedList.getOrElse(4, 7) == 7);

    assert(linkedList.get(-1) == 3);
    assert(linkedList.get(-2) == 2);
    assert(linkedList.get(-3) == 1);
    assert(linkedList.get(-4) == 0);
    assert(linkedList.getOrElse(-4, -7) == -7);

    linkedList.forEach([](int i) { std::cout << "\ti = " << i << "\n"; });

    linkedList.remove(1);
    assert(linkedList.get(1) == 3);

    linkedList.remove(-1);
    assert(linkedList.get(-1) == 1);

    assert(!linkedList.remove(5));
    assert(!linkedList.remove(-5));

    linkedList.add(8);
    linkedList.add(9); // linkedList = {1, 8, 9}
    assert(linkedList.size() == 3);
    assert(linkedList.get(0) == 1);
    linkedList.set(1, 10);
    assert(linkedList.get(0) == 1);
    assert(linkedList.get(1) == 10);
    assert(linkedList.get(2) == 9);

    assert(linkedList.indexOf(10) == 1);

    assert(linkedList.removeAll());

    std::cout << "------------------------------------\n";

    LinkedList<int *> pL{};
    assert(pL.getOrElse(0, nullptr) == nullptr);
    pL.add(new int(21));
    pL.add(new int(331));
    pL.add(new int(74));

    pL.forEach([](int *const i) { std::cout << "\ti = " << *i << "\n"; });
    pL.clear();

    std::cout << "------------------------------------\n";

    LinkedList<int *> linkedList1RemoveTest{};
    assert(linkedList1RemoveTest.getOrElse(0, nullptr) == nullptr);
    linkedList1RemoveTest.add(new int(21));
    linkedList1RemoveTest.add(new int(331));
    linkedList1RemoveTest.add(new int(74));

    int *element1 = linkedList1RemoveTest.get(1);
    int *element2 = linkedList1RemoveTest.get(2);

    linkedList1RemoveTest.remove(element1);
    assert(linkedList1RemoveTest.get(1) == element2);

    std::cout << "ok -> shouldTestLinkedList\n";
}

int main() {

    std::cout << "\n"
              << "------------------------------------------------------------" << "\n"
              << " >> TEST START" << "\n"
              << "------------------------------------------------------------" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    const int repeat = 1;

    for (int i = 0; i < repeat; ++i) {

        shouldCreateLinkedHashMap();

        shouldPutKeyValuePairInToTheHashMap();
        shouldPutValueIfTheKeyExistsInTheHashMap();

        shouldGetTheFirstValueInTheHashMap();
        shouldGetTheSecondValueInTheHashMap();
        shouldGetTheLastValueInTheHashMap();
        shouldNotGetValueForKeyNotInTheHashMap();

        shouldGetTheValueInsteadOfDefaultForKeyInTheHashMap();
        shouldGetTheDefaultValueForKeyNotInTheHashMap();

        shouldGetFloatValueForEnumKeyInTheHashMap();
        shouldGetEnumValueForFloatKeyInTheHashMap();

        shouldTestMapDoesContainKey();
        shouldTestMapDoesNotContainKey();

        shouldTestMapDoesContainValue();
        shouldTestMapDoesNotContainValue();

        shouldReplaceTheFirstValueInTheHashMap();
        shouldReplaceTheSecondValueInTheHashMap();
        shouldReplaceTheLastValueInTheHashMap();
        shouldNotReplaceAnyValueForKeyNotInTheHashMap();

        shouldReplaceExactOldValueWithNewValueForKeyInTheHashMap();
        shouldNotReplaceMismatchOldValueWithNewValueForKeyInTheHashMap();
        shouldNotReplaceExactOldValueWithNewValueForKeyNotInTheHashMap();

        shouldRemoveTheFirstPairInTheHashMap();
        shouldRemoveTheSecondPairInTheHashMap();
        shouldRemoveTheLastPairInTheHashMap();
        shouldNotRemoveAnyPairsWhenKeyNotInTheHashMap();

        shouldRemoveTheFirstPairWithExactKeyValueInTheHashMap();
        shouldRemoveTheSecondPairWithExactKeyValueInTheHashMap();
        shouldRemoveTheLastPairWithExactKeyValueInTheHashMap();
        shouldRemoveAllPairsInTheHashMap();

        shouldNotRemoveAnyPairsWithMismatchKeyAndExactValueInTheHashMap();
        shouldNotRemoveAnyPairsWithExactKeyAndMismatchValueInTheHashMap();

        shouldExecuteActionForEachPairInTheHashMap();
        shouldGetEachPairInTheHashMap();

        shouldPutArrayPointerToKey();
        shouldPutArrayPointerToValue();

        shouldUpdateProvidedVariableWithMappedValueForTheProvidedKey();

        shouldTestLinkedList();
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
