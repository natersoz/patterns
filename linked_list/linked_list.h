/**
 * @file linked_list.h
 */

#pragma once

#include <iterator>
#include <type_traits>

/**
 * @class linked_list
 *
 * A double-linked list data structure. Nodes are intended to be owned
 * by a specific client and are never kept within more than one list
 * at a time. Node ownership is held by the client.
 *
 * @tparam Type The data type contained within the list and its nodes.
 */
template<typename Type>
class linked_list
{
public:
    /**
     * @class list_node
     * Doubly linked list node elements which are inserted into a linked_list.
     */
    class list_node
    {
    public:
        ~list_node() = default;

        /** @{
         * Duplicate list nodes which point to the same node within
         * a list is potential trouble. Disallow this situation.
         */
        list_node(list_node const &)            = delete;
        list_node& operator=(list_node const &) = delete;
        /** @} */

        list_node(Type const &data): data_(data), next_(this), prev_(this) {}

        /// @note Uninitialized this->data_ is expected for sentinal nodes.
        list_node(): next_(this), prev_(this) {}

        Type      data_;
        list_node *next_;
        list_node *prev_;

        /**
         * Insert node after this.
         * @param node The node to insert after this node.
         */
        void insert_after(list_node& node)
        {
            node.unlink();
            node.next_          = this->next_;
            node.prev_          = this;
            this->next_->prev_  = &node;
            this->next_         = &node;
        }

        /**
         * Insert node before this.
         * @param node The node to insert before this node.
         */
        void insert_before(list_node& node)
        {
            node.unlink();
            node.next_          = this;
            node.prev_          = this->prev_;
            this->prev_->next_  = &node;
            this->prev_         = &node;
        }

        /**
         * Determine the number of nodes whcih are linked between [first, last).
         *
         * @param first The node to begin counting with.
         *              Counting includes this node.
         * @param last  The last node to terminate couting with.
         *              Counting excludes this node.
         *
         * @return std::size_t The number of lined nodes [first, last).
         */
        static std::size_t count(list_node const& first, list_node const& last)
        {
            std::size_t node_count = 0u;
            for (list_node const* iter = &first; iter != &last; iter = iter->next_)
            {
                node_count += 1u;
            }
            return node_count;
        }

        /**
         * Insert the range of nodes [first, last) before this.
         * @note This disconnects the nodes from the previous list.
         *
         * @param first The first node to insert.
         * @param last The terminating forward node in the list to insert.
         */
        void insert_before(list_node &first, list_node &last)
        {
            // last is the node beyond the nodes being inserted.
            // last_prev will be the last node inserted.
            list_node* last_prev    = last.prev_;

            // Point the neighbors of first, last_prev around them so that
            // the neighobrs are now connected.
            first.prev_->next_      = last_prev->next_;
            last_prev->next_->prev_ = first.prev_;

            this->prev_->next_  = &first;
            first.prev_         = this->prev_;
            last_prev->next_    = this;
            this->prev_         = last_prev;
        }

        /**
         * Fully remove this node from list containment and safely point
         * next_, prev_ at this. Node this is not part of the original list
         * and does not reference the original list.
         */
        void remove()
        {
            this->unlink();
            this->next_ = this;
            this->prev_ = this;
        }

        /**
         * Remove the node from the list in the manner a forward iterator
         * remove opperation would perform.
         *
         * @return list_node* The list node in back of the node removed.
         */
        list_node* remove_forward()
        {
            list_node *next = this->next_;
            remove();
            return next;
        }

        /**
         * Remove the node from the list in the manner a reverse iterator
         * remove opperation would perform.
         *
         * @return list_node* The list node in front of the node removed.
         */
        list_node* remove_reverse()
        {
            list_node *prev = this->prev_;
            remove();
            return prev;
        }

        /**
         * @return bool true if the node points to something other than itself.
         */
        bool is_linked() const { return not(this->next == this); }

        bool operator==(list_node const& other) const
        {
            return (this == &other);
        }

    private:
        /**
         * Unlink a list node from its neighbors,
         * thereby removing it from the list.
         * @warning this->next_, this->prev_ still point to the original list.
         * @see remove().
         */
        void unlink()
        {
            this->prev_->next_ = this->next_;
            this->next_->prev_ = this->prev_;
        }
    };

    template<bool IsConst>
    class _iterator_generic
        : public std::iterator<std::bidirectional_iterator_tag, Type>
    {
    public:
        using value_type = std::remove_cv_t<Type>;
        using reference  = std::conditional_t<IsConst, const Type, Type>&;
        using pointer    = std::add_pointer_t<reference>;

        using node_reference = std::conditional_t<
            IsConst,
            const linked_list::list_node,
            linked_list::list_node>&;

        using node_pointer = std::add_pointer_t<node_reference>;

        ~_iterator_generic()                                       = default;
        _iterator_generic(_iterator_generic const& other)          = default;
        _iterator_generic& operator=(_iterator_generic const& rhs) = default;

        _iterator_generic(node_reference node) : node_(&node) {}

        // Pre-increment operator: ++iterator.
        _iterator_generic& operator++()
        {
            this->node_ = this->node_->next_;
            return *this;
        }

        // Post-increment operator: iterator++.
        _iterator_generic operator++(int)
        {
            _iterator_generic ret_val = *this;
            ++(*this);
            return ret_val;
        }

        // Pre-increment operator: --iterator.
        _iterator_generic& operator--()
        {
            this->node_ = this->node_->prev_;
            return *this;
        }

        // Pos-increment operator: iterator--.
        _iterator_generic operator--(int)
        {
            _iterator_generic ret_val = *this;
            --(*this);
            return ret_val;
        }

        /**
         * @return bool true if the nodes pointed to be the iterator are the
         * same; false if they are different.
         * @note const_iterator and iterator pointing a the same node will
         * return true. This is the intended behavior.
         */
        bool operator==(_iterator_generic const& other) const
        {
            return (this->node_ == other.node_);
        }

        bool operator!=(_iterator_generic const& other) const
        {
            return not(*this == other);
        }

        reference operator*() const { return this->node_->data_; }
        reference operator*()       { return this->node_->data_; }

        operator node_reference() const { return *this->node_; }
        operator node_reference()       { return *this->node_; }

        node_pointer node_;
    };

    using iterator       = _iterator_generic<false>;
    using const_iterator = _iterator_generic<true>;

    /** Create an empty linked_list; contains a single sentinal node. */
    linked_list() = default;

    // @todo these might be useful:
    // linked_list(std::initializer_list<T> init);
    // list& operator=( list&& other );
    // list& operator=( std::initializer_list<T> ilist );
    // splice()

    iterator       begin()        { return       iterator(*this->sentinal_.next_); }
    const_iterator begin()  const { return const_iterator(*this->sentinal_.next_); }

    iterator       end()          { return       iterator(this->sentinal_); }
    const_iterator end()    const { return const_iterator(this->sentinal_); }

    iterator       rbegin()       { return       iterator(*this->sentinal_.prev_); }
    const_iterator rbegin() const { return const_iterator(*this->sentinal_.prev_); }

    iterator       rend()         { return this->end(); }
    const_iterator rend()   const { return this->end(); }

    const_iterator cbegin() const  { return this->begin(); }
    const_iterator cend()   const  { return this->end(); }

    const_iterator crbegin() const { return this->rbegin(); }
    const_iterator crend()   const { return this->end(); }

    /**
     * @return bool true if the list contains no non-sentinal nodes.
     * false if there are active nodes within the list.
     */
    bool empty() const { return this->sentinal_.next_ == &this->sentinal_; }

    /**
     * @return std::size_t The number of nodes contained in the list.
     * @note Unlike std::list this function is O(n), not O(1).
     */
    std::size_t size() const
    {
        return list_node::count(*this->sentinal_.next_, this->sentinal_);
    }

    /** @return The maximum number of nodes that a list can contain. */
    std::size_t max_size() const { return ~std::size_t(0u); }

    /**
     * Insert a list node into a list before pos.
     *
     * @param pos  The iterator in the destination list marking the insertion
     *             point. The user supplied node is inserted before pos.
     * @param node The user supplied node to insert into this list.
     *             If the node is attached to a list it is removed from the
     *             list prior to insertion.
     *
     * @return iterator The iterator pointing the inserted node.
     */
    template<bool IsConst>
    _iterator_generic<IsConst> insert(_iterator_generic<IsConst> pos, list_node &node)
    {
        pos.node_->insert_before(node);
        return _iterator_generic<IsConst>(node);
    }

    /**
     * Insert a list node into a list before pos.
     *
     * @param pos The iterator in the destination list marking the insertion
     *            point. The user supplied node is inserted before pos.
     * @param node_iter The user supplied node to insert into this list.
     *             If the node is attached to a list it is removed from the
     *             list prior to insertion.
     *
     * @return iterator The iterator pointing the inserted node.
     */
    template<bool IsConst>
    _iterator_generic<IsConst> insert(_iterator_generic<IsConst> pos,
                                      _iterator_generic<IsConst>& node_iter)
    {
        pos.node_->insert_before(node_iter.node_);
        return _iterator_generic<IsConst>(node_iter.node_);
    }

    /**
     * Insert a forward sequence of list nodes [first, last) into a list
     * before pos.
     * @example To insert the entirety of list_1 into the front if list_2
     * list_2.insert(list_2.begin(), list_1.begin(), list_1.end());
     * @note This could be a static function but since the std::list version
     * is tied to a specific list instance it was chosen to be similar.
     *
     * @param pos The iterator in the destination list marking the insertion
     *            point. The user supplied nodes are inserted before pos.
     * @param first The first node in a list to be moved from one list to this.
     * @param last  The terminating list node, which is one past the last node
     *              to be added to this list.
     *
     * @return iterator If first != last: The iterator pointing the first
     *                  inserted node.
     *                  IF first == last: The iterator pos.
     */
    template<bool IsConst>
    _iterator_generic<IsConst> insert(_iterator_generic<IsConst> pos,
                                      _iterator_generic<IsConst> first,
                                      _iterator_generic<IsConst> last)
    {
        if (first == last)
        {
            return pos;
        }
        else
        {
            pos.node_->insert_before(*first.node_, *last.node_);
            return first;
        }
    }

    /** @{ Access the node data from the front and back of the list. */
    Type&       front()       { return this->sentinal_.next_->data_; }
    Type const& front() const { return this->sentinal_.next_->data_; }

    Type&       back()        { return this->sentinal_.prev_->data_; }
    Type const& back()  const { return this->sentinal_.prev_->data_; }
    /** @} */

    /**
     * Prepend the node to the front of the list
     * @param node The node to append.
     */
    void push_front(list_node& node)
    {
        this->sentinal_.insert_after(node);
    }

    /**
     * Append the node to the end of the list
     * @param node The node to append.
     */
    void push_back(list_node& node)
    {
        this->sentinal_.insert_before(node);
    }

    /**
     * Remove the node from the front of the list.
     * @note The node is not destroyed.
     */
    void pop_front()
    {
        if (not this->empty())
        {
            this->sentinal_.next_->remove();
        }
    }

    /**
     * Remove the node from the back of the list.
     * @note The node is not destroyed.
     */
    void pop_back()
    {
        if (not this->empty())
        {
            this->sentinal_.prev_->remove();
        }
    }

    /**
     * Remove the node from the list pointed to by pos.
     *
     * @param pos The iterator which points to the node to remove from the list.
     * @note The node is removed but not destroyed.
     *
     * @return iterator The iterator following the last removed element.
     * If pos refers to the last element then end() is returned.
     */
    template<bool IsConst>
    _iterator_generic<IsConst> erase(_iterator_generic<IsConst> pos)
    {
        return _iterator_generic<IsConst>(*pos.node_->remove_forward());
    }

    list_node sentinal_;
};
