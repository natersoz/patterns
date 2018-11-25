/**
 * @file observer.h
 */

#pragma once
#include <boost/intrusive/list.hpp>

/// Forward declare the Observable so that friendship can be established.
template <typename NotificationType>
class Observable;

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
 * list_base_hook: default tag, raw pointers stored, safe_link mode.
 */
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

private:
    using list_hook_type = boost::intrusive::list_member_hook<
        boost::intrusive::link_mode<boost::intrusive::auto_unlink>
        >;

    list_hook_type hook;

    friend class Observable<NotificationType>;
};
