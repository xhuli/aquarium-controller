#include <iostream>
#include <set>

// interfaces

template<typename T>
class AbstractObserver {
public:
    ~AbstractObserver() = default;

    virtual void update(T const &state) = 0;
};

template<typename T>
class AbstractObservable {
public:
    ~AbstractObservable() = default;

    virtual void registerObserver(AbstractObserver<T> &observer) = 0;

    virtual void unregisterObserver(AbstractObserver<T> &observer) = 0;

    virtual void notifyObservers() const = 0;
};

// implementations

template<typename T>
class ConcreteObserver : public AbstractObserver<T> {
    AbstractObservable<T> &observable;
    T state;
public:
    explicit ConcreteObserver(AbstractObservable<T> &observableToAttachTo) : observable(observableToAttachTo) {
        std::cout << "Concrete observer " << this << " created to observe " << &observableToAttachTo << "\n";
        observable.registerObserver(*this);
    }

    ~ConcreteObserver() {
        observable.unregisterObserver(*this);
    }

    void update(T const &newSubjectState) override {
        state = newSubjectState;
        std::cout << "Observer " << this << " notified of observable " << &observable << " value change to " << state << "\n";
    }
};

static constexpr uint8_t MAX_OBSERVERS = 8;

template<typename T>
class ConcreteObservable : public AbstractObservable<T> {
    std::set<AbstractObserver<T> *> observers;
    T state;

    short observerIndex = -1;
    AbstractObserver<T> *m_observers[MAX_OBSERVERS];

public:
    explicit ConcreteObservable(T state) : state(state) {
        std::cout << "Concrete observable " << this << " created with value " << state << "\n";
    }

    void registerObserver(AbstractObserver<T> &observer) override {
        observers.insert(&observer);
        std::cout << "Subject " << this << " registered observer " << &observer << "\n";
    }

    void unregisterObserver(AbstractObserver<T> &observer) override {
        observers.erase(&observer);
        std::cout << "Subject " << this << " unregistered observer " << &observer << "\n";
    }

    void notifyObservers() const override {
        for (auto iter = observers.begin(); iter != observers.end(); ++iter) {
            (*iter)->update(state);
        }
    }

    void stateChanged() const {
        notifyObservers();
    }

    T const &getState() {
        return &state;
    }

    void setState(T const &newState) {
        state = newState;
        std::cout << "State of observable " << this << " changed to " << newState << "\n";
        stateChanged();
    }
};

int main() {
    std::cout << "Testing Subject-Observer implementation:" << "\n";

    ConcreteObservable<int> subject(42);

    ConcreteObserver<int> observer_1(subject);
    ConcreteObserver<int> observer_2(subject);
    ConcreteObserver<int> observer_3(subject);

    subject.setState(19);
    subject.unregisterObserver(observer_3);
    subject.setState(13);
}