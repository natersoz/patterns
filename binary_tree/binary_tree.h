/**
 * @file binary_tree.h
 *
 * @copyright Copyright (c) 2024 natersoz
 */

#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>
#include <utility>
#include <stdexcept>

/**
 * @class  binary_tree
 * @tparam Type
 * The data type to contain within the tree and its nodes.
 * @note Type must be movable.
 *
 *        <- less than ... greater than ->
 *                      A
 *                    /   \
 *                   B      C
 *                  / \    / \
 *                 D   E  F   G
 *                /   /  /     \
 *               H   K  I       L
 *                  /  /       /
 *                 T  J       N
 *                     \     / \
 *                      M   O   P
 *                               \
 *                                Q
 *                                 \
 *                                  R
 *
 * Iterate lowest -> highest
 * H, D, B, K, E, S, A, J, M, I, F, C, G, O, N, P, Q, R, L
 *
 * Iterate highest -> lowest
 * L, R, Q, P, N, O, G, C, F, I, M, J, A, S, E, K, B, D, H
 *
 * To make this tree:
 * insert: A(100), B( 50), C(200), D( 25), E( 75),
 *         H( 10), K( 60), F(150), I(140), J(130),
 *         M(135), G(300), L(400), N(350), O(275),
 *         P(375), Q(380), R(385), S( 80)
 * @note Node S is not shown in the tree, but is the right child of node E.
 */
template <typename Type>
class binary_tree
{
public:
    using value_type = Type;

    ~binary_tree() = default;

    // No copying allowed, move is ok.
    binary_tree(binary_tree const&)                = delete;
    binary_tree(binary_tree&&) noexcept            = default;
    binary_tree& operator=(binary_tree const&)     = delete;
    binary_tree& operator=(binary_tree&&) noexcept = default;

    binary_tree() noexcept :
        head_node(nullptr), node_count(0u) {}

    /**
     * @struct tree_node
     * This is intended to be an intrusive class. Therefore, the struct
     * tree_node data members are public and exposed for use by the client.
     */
    struct tree_node
    {
        using value_type = binary_tree::value_type;

        tree_node* parent;
        tree_node* left;
        tree_node* right;
        value_type data;

        ~tree_node()                               = default;
        tree_node(tree_node const&)                = delete;
        tree_node(tree_node&&) noexcept            = default;
        tree_node& operator=(tree_node const&)     = delete;
        tree_node& operator=(tree_node&&) noexcept = default;

        /// The default ctor requires that the data type have a default ctor.
        tree_node() noexcept :
            parent(nullptr), left(nullptr), right(nullptr), data() {};

        /// @param data_ Will be copied into the tree node.
        tree_node(value_type data_) noexcept :
            parent(nullptr), left(nullptr), right(nullptr), data(data_) {};

        /**
         * Get the left most node in the tree, starting with this node.
         * @return tree_node const* The resulting left most child in the tree.
         */
        tree_node const* get_leftest() const noexcept
        {
            tree_node const* node = this;
            for (; node->left; node = node->left);
            return node;
        }

        tree_node* get_leftest() noexcept
        {
            return const_cast<tree_node*>(std::as_const(*this).get_leftest());
        }

        /**
         * Get the right most node in the tree, starting with this node.
         * @return tree_node const* The resulting right most child in the tree.
         */
        tree_node const* get_rightest() const noexcept
        {
            tree_node const* node = this;
            for (; node->right; node = node->right);
            return node;
        }

        tree_node* get_rightest() noexcept
        {
            return const_cast<tree_node*>(std::as_const(*this).get_rightest());
        }

        /**
         * Iterate to the next highest value node in the tree;
         * i.e. the successor node to this.
         * @return tree_node const*  The next highest value node in the tree.
         */
        tree_node const* get_next() const noexcept
        {
            // If this has a right node, then the successor will be
            // the bottom of the left-hand side of the right node.
            // Example: In class comments: node B -> K, A -> J, G -> O.
            if (this->right)
            {
                return this->right->get_leftest();
            }

            // this has no right node: Iterate up the tree until a parent
            // is found in which we are not on its right hand side.
            // Example: In class comments: node E -> A, M -> I, R -> L.
            tree_node const* node = this;
            for (; node->parent and (node->parent->right == node);
                   node = node->parent);

            return node->parent;
        }

        tree_node* get_next() noexcept
        {
            return const_cast<tree_node*>(std::as_const(*this).get_next());
        }

        /**
         * Iterate to the next lowest value node in the tree.
         *
         * @param node               The starting position to begin iteration.
         * @return tree_node const*  The next lowest value node in the tree.
         */
        tree_node const* get_prev() const noexcept
        {
            // If this has a left node, then the predecessor will be
            // the bottom of the right-hand side of the left node.
            // Example: In class comments: node A -> E, C -> F, I -> M, L -> P
            if (this->left)
            {
                return this->left->get_rightest();
            }

            // this has no left node: Iterate up the tree until a parent
            // is found in which we are not on its left hand side.
            // Example: In class comments: node K -> B, J -> A, O -> G.
            tree_node const* node = this;
            for (; node->parent and (node->parent->left == node);
                   node = node->parent);

            return node->parent;
        }

        tree_node* get_prev() noexcept
        {
            return const_cast<tree_node*>(std::as_const(*this).get_prev());
        }

        /**
         * Insert a  node starting with this as the insertion point.
         * @param node The node to insert.
         */
        void insert(tree_node* node)
        {
            // Disconnect the node being inserted from other nodes.
            node->parent = nullptr;
            node->left   = nullptr;
            node->right  = nullptr;

            // Insert left if comparison is <, right if comparision is >.
            if (node->data < this->data)
            {
                // If this left child is occupied, insert after this left child.
                if (this->left)
                {
                    this->left->insert(node);
                }
                else
                {
                    // Left child is vacant; add to left child of this.
                    node->parent = this;
                    this->left   = node;
                }
            }
            else if (node->data > this->data)
            {
                // If this right child is occupied, insert after this right child.
                if (this->right)
                {
                    this->right->insert(node);
                }
                else
                {
                    // Right child is vacant; add to right child of this.
                    node->parent = this;
                    this->right  = node;
                }
            }
            else
            {
                // reject insertion
                throw std::runtime_error("duplicate node value");
            }
        }

        tree_node const* find(value_type const& value) const noexcept
        {
            tree_node const* node = this;
            while(node and value != node->data)
            {
                if (value < node->data)
                {
                    node = node->left;
                }
                else
                {
                    node = node->right;
                }
            }

            return node;
        }

        tree_node* find(value_type const& value) noexcept
        {
            return const_cast<tree_node*>(std::as_const(*this).find(value));
        }

        /**
         * Orphan this node by detaching it from its parent.
         */
        void detach_from_parent() noexcept
        {
            if (this->parent != nullptr)
            {
                if (this->parent->right == this)
                {
                    this->parent->right = nullptr;
                }
                else // (this->parent->left == this)
                {
                    this->parent->left = nullptr;
                }
            }
        }

        /**
         * Replace this node in the tree with a new node.
         *
         * If this has as parent, then point the the parent of this
         * at the replacement node. this becomes an orphan.
         *
         * @param node The replacement node.
         *             The replacement node's children are unchanged.
         */
        void replace(tree_node*& node) noexcept
        {
            if (this->parent)
            {
                if (this->parent->right == this)
                {
                    this->parent->right = node;
                }
                else // (this->parent->left == this)
                {
                    this->parent->left = node;
                }
            }

            node->parent = this->parent;

            this->parent = nullptr;
            this->left   = nullptr;
            this->right  = nullptr;
        }

        /**
         * Remove the this node from the tree.
         */
        void remove() noexcept
        {
            // Node being removed has no children:
            if (this->left == nullptr and this->right == nullptr)
            {
                this->detach_from_parent();
            }
            // Node being deleted has only a right child,
            // remove this node and the right child takes its place.
            else if (this->left == nullptr and this->right != nullptr)
            {
                this->replace(this->right);
            }
            // Node being deleted has only a left child,
            // remove this node and the left child takes its place.
            else if (this->left != nullptr and this->right == nullptr)
            {
                this->replace(this->left);
            }
            // Node being removed has 2 children; this node's successor
            // must be contained within the this node's right branch.
            else // (this->left != nullptr and this->right != nullptr)
            {
                tree_node* next_node = this->get_next();
                if (this->right == next_node)
                {
                    // If the next node (successor) is the right node of this
                    // node (being removed) then next_node->left must be null.
                    assert(next_node->left == nullptr);
                    this->replace(this->right);
                }
                else
                {
                    /**
                     * this node's (being removed) successor was not this's
                     * right node.
                     * Example: remove node B, B->next() = K
                     *
                     *         A                       A
                     *        /                       /
                     *       B                       K
                     *      / \         =>          / \
                     *     D   E                   D   E
                     *    /   / \                 /     \
                     *   H   K   S               H       S
                     *
                     *  * To make this tree:
                     * insert: A(100), B( 50), D( 25), E( 75),
                     *         H( 10), K( 60), S( 80)
                     */
                    std::swap(this->data, next_node->data);
                    next_node->detach_from_parent();
                }
            }
        }
    };

    template<typename TreeType, typename NodeType>
    class base_iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = Type;
        using difference_type   = Type;
        using pointer           = Type const*;  /// @todo TBD
        using reference         = Type const&;  /// @todo TBD

        using tree_type         = TreeType;
        using node_type         = NodeType;

        ~base_iterator()                                   = default;
        base_iterator()                                    = delete;
        base_iterator(base_iterator const&)                = default;
        base_iterator(base_iterator&&) noexcept            = default;
        base_iterator& operator=(base_iterator const&)     = default;
        base_iterator& operator=(base_iterator&&) noexcept = default;

        explicit base_iterator(tree_type* btree, node_type* node) noexcept:
            tree(btree), pos(node) {}

        base_iterator& operator++() noexcept
        {
            increment();
            return *this;
        }

        base_iterator operator++(int) noexcept
        {
            base_iterator previous = *this;
            increment();
            return previous;
        }

        base_iterator& operator--() noexcept
        {
            decrement();
            return *this;
        }

        base_iterator operator--(int) noexcept
        {
            base_iterator previous = *this;
            decrement();
            return previous;
        }

        template<typename Iterator>
        bool operator==(Iterator const& other) const noexcept
        {
            return (this->tree == other.tree) and (this->pos == other.pos);
        }

        template<typename Iterator>
        bool operator!=(Iterator const& other) const noexcept
        {
            return not (*this == other);
        }

        // Only const dereferencing makes sense, because if the value
        // is modified then the tree will no longer be sorted.
        value_type const& operator *() const noexcept
        {
            return this->pos->data;
        }

    private:
        tree_type* tree;
        node_type* pos;

        void increment() noexcept
        {
            if (this->pos == nullptr)
            {
                this->pos = this->tree->head_node->get_leftest();
            }
            else
            {
                this->pos = this->pos->get_next();
            }
        }

        void decrement() noexcept
        {
            if (this->pos == nullptr)
            {
                this->pos = this->tree->head_node->get_rightest();
            }
            else
            {
                this->pos = this->pos->get_prev();
            }
        }

        friend binary_tree;
    };

    using iterator       = base_iterator<binary_tree,       binary_tree::tree_node>;
    using const_iterator = base_iterator<binary_tree const, binary_tree::tree_node const>;

    /**
     * Insert a node into the binary tree.
     *
     * @param node The node to insert.
     * @throw std::runtime_error if the node.value is a duplicated.
     */
    void insert(tree_node& node)
    {
        node.parent = nullptr;
        node.left   = nullptr;
        node.right  = nullptr;

        if (this->head_node)
        {
            this->head_node->insert(&node);
        }
        else
        {
            this->head_node = &node;
        }
        this->node_count += 1u;
    }

    void erase(iterator iter) noexcept
    {
        iter.pos->remove();
        this->node_count -= 1u;
    }

    constexpr size_t size() const noexcept
    {
        return this->node_count;
    }

    constexpr bool empty() const noexcept
    {
        return (this->head_node == nullptr);
    }

    void clear() noexcept
    {
        this->node_count = 0u;
        this->head_node  = nullptr;
    }

    const_iterator find(const value_type& value) const noexcept
    {
        tree_node const* node = this->head_node->find(value);
        return const_iterator(this, node);
    }

    iterator find(const value_type& value) noexcept
    {
        tree_node* node = this->head_node->find(value);
        return iterator(this, node);
    }

    const_iterator begin() const noexcept
    {
        if (this->head_node)
        {
            tree_node const* node = this->head_node->get_leftest();
            return const_iterator(this, node);
        }
        else
        {
            return end();
        }
    }

    iterator begin() noexcept
    {
        if (this->head_node)
        {
            tree_node* node = this->head_node->get_leftest();
            return iterator(this, node);
        }
        else
        {
            return end();
        }
    }

    const_iterator end() const
    {
        return const_iterator(this, nullptr);
    }

    iterator end()
    {
        return iterator(this, nullptr);
    }

    std::reverse_iterator<binary_tree<value_type>::const_iterator> rbegin() const
    {
        return std::make_reverse_iterator(this->end());
    }

    std::reverse_iterator<binary_tree<value_type>::const_iterator> rend() const
    {
        return std::make_reverse_iterator(this->begin());
    }

private:
    tree_node*  head_node;
    size_t      node_count;
};
