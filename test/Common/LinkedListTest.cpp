#include <cassert>
#include <iostream>
#include <chrono>

#include <Common/LinkedList.h>

struct Value {
    explicit Value() : value(0) {}

    explicit Value(int value) : value(value) {}

    int value;

    bool operator==(Value const &rhs) {
        return (value == rhs.value);
    }
};

static void shouldReturnDefaultValueWhenEmpty() {
    LinkedList<int> linkedList{};

    assert(linkedList.get(0) == 0);
    assert(linkedList.get(1) == 0);
    assert(linkedList.get(-1) == 0);

    std::cout << "ok -> shouldReturnDefaultValueWhenEmpty\n";
}

static void shouldAddElementsToTheList() {
    LinkedList<int> linkedList{};

    linkedList.add(1);
    linkedList.add(2);
    linkedList.add(3);

    assert(linkedList.get(0) == 1);
    assert(linkedList.get(1) == 2);
    assert(linkedList.get(2) == 3);
    assert(linkedList.get(4) == 0);

    std::cout << "ok -> shouldAddElementsToTheList\n";
}

static void shouldGetElementsFromTheList() {
    LinkedList<int> linkedList{};

    linkedList.add(1);
    linkedList.add(2);
    linkedList.add(3);

    assert(linkedList.get(0) == 1);
    assert(linkedList.get(1) == 2);
    assert(linkedList.get(2) == 3);
    assert(linkedList.get(4) == 0);

    std::cout << "ok -> shouldGetElementsFromTheList\n";
}

static void shouldGetPointerElementsFromTheList() {
    LinkedList<int *> linkedList{};

    assert(linkedList.getOrElse(0, nullptr) == nullptr);

    linkedList.add(new int(21));
    linkedList.add(new int(331));
    linkedList.add(new int(74));

    int *element1 = linkedList.get(1);
    int *element2 = linkedList.get(2);

    assert(*element1 == 331);
    assert(*element2 == 74);

    std::cout << "ok -> shouldGetPointerElementsFromTheList\n";
}

static void shouldGetSpecifiedElseValueWhenElementNotInTheList() {

    LinkedList<int> linkedList{};
    assert(linkedList.getOrElse(4, 7) == 7);

    std::cout << "ok -> shouldGetSpecifiedElseValueWhenElementNotInTheList\n";
}

static void shouldGetElementsCountingFromThenEndOfTheListForNegativeIndexes() {
    LinkedList<int> linkedList{};

    linkedList.add(1);
    linkedList.add(2);
    linkedList.add(3);

    assert(linkedList.get(-1) == 3);
    assert(linkedList.get(-2) == 2);
    assert(linkedList.get(-3) == 1);
    assert(linkedList.get(-4) == 0);
    assert(linkedList.getOrElse(-4, -7) == -7);

    std::cout << "ok -> shouldGetElementsCountingFromThenEndOfTheListForNegativeIndexes\n";
}

static void shouldImplementForEachMethod() {
    LinkedList<int> linkedList{};

    linkedList.add(1);
    linkedList.add(2);
    linkedList.add(3);

    linkedList.forEach([](int i) { std::cout << "\ti = " << i << "\n"; });

    std::cout << "ok -> shouldImplementForEachMethod\n";
}

static void shouldImplementForEachMethodForPointers() {
    LinkedList<int *> linkedListOfPointers{};

    assert(linkedListOfPointers.getOrElse(0, nullptr) == nullptr);

    linkedListOfPointers.add(new int(21));
    linkedListOfPointers.add(new int(331));
    linkedListOfPointers.add(new int(74));

    linkedListOfPointers.forEach([](int *const i) { std::cout << "\ti = " << *i << "\n"; });
    linkedListOfPointers.clear();

    std::cout << "ok -> shouldImplementForEachMethodForPointers\n";
}

static void shouldRemoveExistingElementsCountingFromTheStartOfTheList() {
    LinkedList<int> linkedList{};

    linkedList.add(1);
    linkedList.add(2);
    linkedList.add(3);

    linkedList.remove(1);
    assert(linkedList.get(1) == 3);

    std::cout << "ok -> shouldRemoveExistingElementsCountingFromTheStartOfTheList\n";
}

static void shouldRemoveExistingElementsCountingFromTheEndOfTheList() {
    LinkedList<int> linkedList{};

    linkedList.add(1);
    linkedList.add(2);
    linkedList.add(3);

    linkedList.remove(-1);
    assert(linkedList.get(-1) == 2);

    std::cout << "ok -> shouldRemoveExistingElementsCountingFromTheEndOfTheList\n";
}

static void shouldNotRemoveElementsForIndexesOutOfTheListSize() {
    LinkedList<int> linkedList{};

    linkedList.add(1);
    linkedList.add(2);
    linkedList.add(3);

    assert(!linkedList.remove(5));
    assert(!linkedList.remove(-5));
    assert(linkedList.size() == 3);

    std::cout << "ok -> shouldNotRemoveElementsForIndexesOutOfTheListSize\n";
}

static void shouldUpdateElementForSpecifiedIndexInTheList() {
    LinkedList<int> linkedList{};

    linkedList.add(1);
    linkedList.add(8);
    linkedList.add(9);

    linkedList.set(1, 10);

    assert(linkedList.get(0) == 1);
    assert(linkedList.get(1) == 10);
    assert(linkedList.get(2) == 9);

    std::cout << "ok -> shouldUpdateElementForSpecifiedIndexInTheList\n";
}

static void shouldGetTheIndexOfTheFirstSearchedElement() {
    LinkedList<int> linkedList{};

    linkedList.add(1);
    linkedList.add(10);
    linkedList.add(9);
    linkedList.add(10);

    assert(linkedList.indexOf(10) == 1);

    std::cout << "ok -> shouldGetTheIndexOfTheFirstSearchedElement\n";
}

static void shouldRemoveElementAtSpecifiedIndexFromTheList() {
    LinkedList<float> linkedList{};

    linkedList.add(1.0f);
    linkedList.add(10.0f);
    linkedList.add(9.0f);

    linkedList.remove(1);

    assert(linkedList.size() == 2);
    assert(linkedList.get(0) == 1.0f);
    assert(linkedList.get(1) == 9.0f);

    std::cout << "ok -> shouldRemoveElementAtSpecifiedIndexFromTheList\n";
}

static void shouldRemoveElementWithSpecifiedValueFromTheList() {
    auto v1 = Value{1};
    auto v2 = Value{2};
    auto v3 = Value{3};

    LinkedList<Value> linkedList{};
    linkedList.add(v1);
    linkedList.add(v2);
    linkedList.add(v3);

    linkedList.remove(v3);

    assert(linkedList.size() == 2);
    assert(linkedList.get(0).value == v1.value);
    assert(linkedList.get(1).value == v2.value);

    std::cout << "ok -> shouldRemoveElementWithSpecifiedValueFromTheList\n";
}

static void shouldRemovePointerElementByValue() {
    LinkedList<int *> linkedList{};

    assert(linkedList.getOrElse(0, nullptr) == nullptr);

    linkedList.add(new int(21));
    linkedList.add(new int(331));
    linkedList.add(new int(74));

    int *element1 = linkedList.get(1);
    int *element2 = linkedList.get(2);

    linkedList.remove(element1);

    assert(linkedList.get(1) == element2);

    std::cout << "ok -> shouldRemovePointerElementByValue\n";
}

static void shouldNotRemoveElementAtSpecifiedIndexOutOfTheListBoundary() {
    LinkedList<float> linkedList{};
    linkedList.add(1.0f);
    linkedList.add(10.0f);
    linkedList.add(9.0f);

    linkedList.remove(3);

    assert(linkedList.size() == 3);
    assert(linkedList.get(0) == 1.0f);
    assert(linkedList.get(1) == 10.0f);
    assert(linkedList.get(2) == 9.0f);

    std::cout << "ok -> shouldNotRemoveElementAtSpecifiedIndexOutOfTheListBoundary\n";
}

static void shouldNotRemoveElementWithSpecifiedValueNotPresentInTheList() {
    auto v1 = Value{1};
    auto v2 = Value{2};
    auto v3 = Value{3};
    auto v4 = Value{4};

    LinkedList<Value> linkedList{};
    linkedList.add(v1);
    linkedList.add(v2);
    linkedList.add(v3);

    linkedList.remove(v4);

    assert(linkedList.size() == 3);
    assert(linkedList.get(0).value == v1.value);
    assert(linkedList.get(1).value == v2.value);
    assert(linkedList.get(2).value == v3.value);

    std::cout << "ok -> shouldNotRemoveElementWithSpecifiedValueNotPresentInTheList\n";
}

static void shouldRemoveAllElementsFromTheList() {
    LinkedList<float> linkedList{};
    linkedList.add(1.0f);
    linkedList.add(10.0f);
    linkedList.add(9.0f);

    assert(linkedList.removeAll());
    assert(linkedList.size() == 0);

    std::cout << "ok -> shouldRemoveAllElementsFromTheList\n";
}

static void shouldIterateTheListWithWhileLoop() {

    LinkedList<int> linkedList{};
    linkedList.add(1);
    linkedList.add(2);
    linkedList.add(3);

    /*
     * Warn:
     * ListIterator<int> iterator = linkedList.iterator(); // <- NOK, 'ListIterator<>' is an abstract class error
     * ListIterator<int> const &iterator = linkedList.iterator(); // must be const reference (both) to avoid 'ListIterator<>' is an abstract class error
     */

    ListIterator<int> const &iterator = linkedList.iterator();

    while (iterator.hasNext()) {
        std::cout << iterator.next() << "\n";
    }

    std::cout << "ok -> shouldIterateTheListWithWhileLoop\n";
}

static void shouldIterateTheListWithForLoop() {

    LinkedList<int> linkedList{};
    linkedList.add(1);
    linkedList.add(2);
    linkedList.add(3);

    /*
     * Warn:
     * ListIterator<int> iterator = linkedList.iterator(); // <- NOK, 'ListIterator<>' is an abstract class error
     * ListIterator<int> const &iterator = linkedList.iterator(); // must be const reference (both) to avoid 'ListIterator<>' is an abstract class error
     */

    for (ListIterator<int> const &iterator = linkedList.iterator(); iterator.hasNext(); iterator.forward()) {
        int value = iterator.getValue();
        std::cout << value << "\n";
    }

    std::cout << "ok -> shouldIterateTheListWithForLoop\n";
}

int main() {

    std::cout << "\n"
              << "------------------------------------------------------------" << "\n"
              << " >> TEST START" << "\n"
              << "------------------------------------------------------------" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    const int repeat = 1;

    for (int i = 0; i < repeat; ++i) {

        shouldReturnDefaultValueWhenEmpty();
        shouldAddElementsToTheList();
        shouldGetElementsFromTheList();
        shouldGetPointerElementsFromTheList();
        shouldGetSpecifiedElseValueWhenElementNotInTheList();
        shouldGetElementsCountingFromThenEndOfTheListForNegativeIndexes();

        shouldImplementForEachMethod();
        shouldImplementForEachMethodForPointers();

        shouldRemoveExistingElementsCountingFromTheStartOfTheList();
        shouldRemoveExistingElementsCountingFromTheEndOfTheList();
        shouldNotRemoveElementsForIndexesOutOfTheListSize();

        shouldUpdateElementForSpecifiedIndexInTheList();

        shouldGetTheIndexOfTheFirstSearchedElement();

        shouldRemoveElementAtSpecifiedIndexFromTheList();
        shouldRemoveElementWithSpecifiedValueFromTheList();
        shouldRemovePointerElementByValue();
        shouldNotRemoveElementAtSpecifiedIndexOutOfTheListBoundary();
        shouldNotRemoveElementWithSpecifiedValueNotPresentInTheList();
        shouldRemoveAllElementsFromTheList();

        shouldIterateTheListWithWhileLoop();
        shouldIterateTheListWithForLoop();
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
