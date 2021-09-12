/**
 * @file observable.h
 */

#pragma once

#include <cstddef>
#include <list>

#include "observer.h"

/**
 * @class Observable
 * Use within a class as an aggregate implementation of the Observable 'Subject'
 * @see Design Patterns Gamma, Helm, et al.
 * 'Observer' pattern 'Subject' class. p. 294.
 *
 * @tparam NotificationType The type of data passed to observers as the
 * notification within the Notify() function.
 *
 * @note Attaching, Detaching, Notifying and storing observers is done with
 * non-const references since calling Observer::Notify() is expected
 * to have side effects (non-const).
 */
template <typename NotificationType>
class Observable
{
public:
    using value_type = NotificationType;

    virtual ~Observable()                       = default;
    Observable()                                = default;
    Observable(Observable const &)              = delete;
    Observable(Observable&&)                    = delete;
    Observable& operator = (Observable const&)  = delete;
    Observable& operator = (Observable&&)       = delete;

    /**
     * Attach an observer to this observable.
     * The observer will be notified with of events when this->NotifyAll is called.
     *
     * @param observer The observer to notify with events.
     */
    void Attach(Observer<value_type> &observer)
    {
        this->observer_list.push_back(&observer);
    }

    /**
     * Detach the observer from this observable.
     * The observer will no longer be notified.
     *
     * @param observer The observer to withdraw from receiving event notifications.
     */
    void Detach(Observer<value_type> &observer)
    {
        this->observer_list.remove(&observer);
    }

    /**
     * Notify all observers of an event.
     * @note This method cannot be const since the call to the observer may
     * result in a Detach() operation.
     *
     * @param notification The information to be sent as a notification event.
     */
    void NotifyAll(value_type const &notification)
    {
        // Note that the iteration happens prior to calling observer->Notify().
        // The observer may remove itself from the list rendering the current
        // iterator position invalid. Therefore: iterate first, operate next.
        for (auto observer_iter = this->observer_list.begin();
             observer_iter != this->observer_list.end();
            )
        {
            Observer<value_type> *observer = *observer_iter;
            ++observer_iter;
            observer->Notify(notification);
        }
    }

    /** @return size_t The number of Observers attached to this Observable. */
    constexpr std::size_t GetObserverCount(void) const
    {
        return this->observer_list.size();
    }

private:
    /**
     * Where the observers are stored.
     * @note observer_list cannot be const since calling observer->Notify() is
     * not const; event notifications likely have side effects.
     */
    std::list<Observer<value_type>*> observer_list;
};
