/**
 * @file test_observer.cc
 * Test application for classes Observer and Observable.
 *
 * Compile with:
 * g++ -g
 * -Wall -Wmissing-field-initializers -Wpointer-arith -Wuninitialized
 * -Winit-self -Wstrict-overflow -Wundef -Wshadow
 * -std=c++17 -I . -I /opt/boost/include  test_observer.cc -o test_observer
 */

#include <boost/intrusive/list.hpp>
#include <iostream>

/** @class Observer. */
template <typename NotificationType>
class Observer
{
public:
    virtual ~Observer()                     = default;
    Observer()                              = default;
    Observer(Observer const&)               = delete;
    Observer(Observer &&)                   = delete;
    Observer& operator = (Observer const&)  = delete;
    Observer& operator=(Observer&&)         = delete;

    virtual void Notify(NotificationType const &notification) = 0;

    using list_hook_type = boost::intrusive::list_member_hook<
        boost::intrusive::link_mode<boost::intrusive::auto_unlink>
        >;

    list_hook_type hook;
};

template <typename NotificationType>
class Observable
{
public:
    ~Observable()                               = default;
    Observable()                                = default;
    Observable(Observable const&)               = delete;
    Observable(Observable&&)                    = delete;
    Observable& operator = (Observable const&)  = delete;
    Observable& operator=(Observable&&)         = delete;

    /**  Notify all observers of an event. */
    void NotifyAll(NotificationType const &notification)
    {
        for (auto observer_iter = observer_list.begin();
             observer_iter != observer_list.end();
            )
        {
            Observer<NotificationType> &observer = *observer_iter;
            ++observer_iter;
            observer.Notify(notification);
        }
    }

    using list_type =
    boost::intrusive::list<
        Observer<NotificationType>,
        boost::intrusive::constant_time_size<false>,
        boost::intrusive::member_hook<
            Observer<NotificationType>,
            typename Observer<NotificationType>::list_hook_type,
            &Observer<NotificationType>::hook>
    >;

    list_type observer_list;
};

static Observable<int> test_observable;

/**
 * @class TestObserver
 * A simple observer which gets notifications of integers.
 */
class TestObserver: public Observer<int>
{
public:
    virtual void Notify(int const &notification)
    {
        std::cout << "notified: " << notification << std::endl;
        if (notification == 3)
        {
            this->hook.unlink();
        }
    };
};

static TestObserver test_observer_1;

int main(void)
{
    test_observable.observer_list.push_back(test_observer_1);

    test_observable.NotifyAll(1);
    test_observable.NotifyAll(2);
    test_observable.NotifyAll(3);
    test_observable.NotifyAll(4);

    return 0;
}
