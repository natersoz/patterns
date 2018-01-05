/**
 * @file observer.h
 */

#pragma once
#include <boost/intrusive/list.hpp>

/**
 * @class Observer.
 * Observer implementation using std::list.
 *
 * This interface defines the requirements of an Observer
 * that can be attached and detached to the Observable class.
 * Attached Observers receive event notifications from the Observable.
 *
 * @see Design Patterns Gamma, Helm, et al.
 * 'Observer' pattern 'Subject' class. pp. 294 - 303.
 *
 * @tparam NotifyType is the data type that will be passed as a notification.
 * list_base_hook: default tag, raw pointers stored, safe_link mode.
 */
template <typename NotificationType>
class Observer: public boost::intrusive::list_base_hook<>
{
public:
    virtual ~Observer() = default;
    Observer() = default;
    Observer(Observer const &rhs) = delete;
    Observer& operator = (Observer const &rhs) = delete;

    /**
     * Notifications are called onto this interface
     * using typename NotificationType.
     *
     * @param notification The notification data passed to the observer from
     * the Observable.
     */
    virtual void Notify(NotificationType const &notification) = 0;
};

