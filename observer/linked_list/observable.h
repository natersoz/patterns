/**
 * @file observable.h
 */

#pragma once

#include <cstddef>
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
    ~Observable()                                   = default;
    Observable()                                    = default;
    Observable(Observable const &rhs)               = delete;
    Observable& operator = (Observable const &rhs)  = delete;

    /**
     * Attach an observer to this observable.
     * The observer will be notified with type NotificationType const reference
     * when needed.
     *
     * @param observer The observer to notify with events.
     */
    void Attach(Observer<NotificationType> &observer);

    /**
     * Detach the observer from this observable.
     * The observer will no longer be notified.
     *
     * @param observer The observer to withdraw from receiving event notifications.
     */
    void Detach(Observer<NotificationType> &observer);

    /**
     * Notify all observers of an event.
     * @note This method cannot be const since the call to the observer may
     * result in a Detach() operation.
     *
     * @param notification The information to be sent as a notification event.
     */
    void NotifyAll(NotificationType const &notification);

    /** @return size_t The number of Observers attached to this Observable. */
    std::size_t GetObserverCount(void) const;

private:
    linked_list<Observer<NotificationType>*> observer_list;
};

template <typename NotificationType>
void Observable<NotificationType>::Attach(Observer<NotificationType> &observer)
{
    observer_list.push_back(observer.list_node_);
}

template <typename NotificationType>
void Observable<NotificationType>::Detach(Observer<NotificationType> &observer)
{
    observer.list_node_.remove();
}

template <typename NotificationType>
void Observable<NotificationType>::NotifyAll(NotificationType const &notification)
{
    // Note that the iteration happens prior to calling observer->Notify().
    // The observer may remove itself from the list rendering the current
    // iterator position invalid. Therefore: iterate first, operate next.
    for (auto observer_iter = observer_list.begin();
         observer_iter != observer_list.end();
        )
    {
        Observer<NotificationType> *observer = *observer_iter;
        ++observer_iter;
        observer->Notify(notification);
    }
}

template <typename NotificationType>
std::size_t Observable<NotificationType>::GetObserverCount() const
{
    return observer_list.size();
}

