/**
 * @file observer.h
 */

#pragma once
#include "linked_list.h"

/**
 * @class Observer.
 * Observer implementation using boost::inclusive::list.
 *
 * This interface defines the requirements of an Observer
 * that can be attached and detached to the Observable class.
 * Attached Observers receive event notifications from the Observable.
 *
 * @see Design Patterns Gamma, Helm, et al.
 * 'Observer' pattern 'Subject' class. pp. 294 - 303.
 *
 * @tparam NotifyType is the data type that will be passed as a notification.
 */
template <typename NotificationType>
class Observer
{
public:
    virtual ~Observer()                         = default;
    Observer(Observer const &rhs)               = delete;
    Observer& operator = (Observer const &rhs)  = delete;

    Observer(): list_node_(this) {}

    /**
     * Notifications are called onto this interface
     * using typename NotificationType.
     *
     * @param notification The notification data passed to the observer from
     * the Observable.
     */
    virtual void Notify(NotificationType const &notification) = 0;

    /**
     * @return bool To determine whether this observer is attached to an Observable.
     * @retval true The Observer is attached to the Observable.
     * @retval false The Observer is not attached to the Observable.
     */
    bool IsAttached() const { return this->is_linked(); }

    /**
     * An observer is equal to another if their pointers are the same.
     * Equality meaning that the class being notified from the observable
     * is the same class.
     *
     * @return bool If the observers are the same receivers of notifications.
     */
    bool operator==(Observer const& rhs) const { return (this == &rhs); }

    typename linked_list<Observer<NotificationType>*>::list_node list_node_;
};


