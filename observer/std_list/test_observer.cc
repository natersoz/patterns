/**
 * @file test_observer.cc
 * Test application for classes Observer and Observable.
 */

#include <cstddef>
#include <iostream>
#include "observer.h"
#include "observable.h"

static Observable<int> test_observable;

/**
 * @class TestObserver
 * A simple observer which gets notifications of integers.
 */
class TestObserver: public Observer<int>
{
public:
    virtual ~TestObserver() {};
    TestObserver(int id): id_(id) {};

    TestObserver() = delete;
    TestObserver(TestObserver const &rhs) = delete;
    TestObserver& operator = (TestObserver const &rhs) = delete;

    virtual void Notify(int const &notification);

    virtual bool operator == (TestObserver const &rhs) const
    {
        return (this->id_ == rhs.id_);
    };

    virtual bool operator != (TestObserver const &rhs) const
    {
        return not(*this == rhs);
    }

    int GetId() const { return this->id_; }

private:
        int id_;
};

std::ostream& operator << (std::ostream &os, TestObserver const &observer)
{
     std::cout << "TestObserver[" << observer.GetId() << "]";
     return os;
};

void TestObserver::Notify(int const &notification)
{
    std::cout << *this << " received: " << notification << std::endl;

    // When the value of the notification matches our id, detach from the Observable.
    // This will test that the remove operation from the observer_list works properly.
    if (notification == id_)
    {
        test_observable.Detach(*this);
    }
};

static TestObserver test_observer_1(1);
static TestObserver test_observer_2(2);
static TestObserver test_observer_3(3);
static TestObserver test_observer_4(4);

int main(void)
{
    bool result = true;

    test_observable.Attach(test_observer_1);
    test_observable.Attach(test_observer_2);
    test_observable.Attach(test_observer_3);
    test_observable.Attach(test_observer_4);

    test_observable.NotifyAll(1);
    std::cout << "Obserable count: " << test_observable.GetObserverCount() << std::endl;
    test_observable.NotifyAll(2);
    std::cout << "Obserable count: " << test_observable.GetObserverCount() << std::endl;
    test_observable.NotifyAll(3);
    std::cout << "Obserable count: " << test_observable.GetObserverCount() << std::endl;
    test_observable.NotifyAll(4);
    std::cout << "Obserable count: " << test_observable.GetObserverCount() << std::endl;

    return result ? 0 : -1;
}
