/**
 * @file observer.h
 */

#pragma once

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
class Observer
{
public:
    using value_type = NotificationType;

    virtual ~Observer()                         = default;
    Observer()                                  = default;
    Observer(Observer&)                         = delete;
    Observer(Observer&&)                        = delete;
    Observer& operator      = (Observer const&) = delete;
    Observer& operator      = (Observer&&)      = delete;

    /**
     * Notifications are called onto this interface
     * using typename NotificationType.
     *
     * @param notification The notification data passed to the observer from
     * the Observable.
     */
    virtual void Notify(value_type const &notification) = 0;
};
