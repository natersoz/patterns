/**
 * @file binary_tree.h
 *
 * @copyright Copyright (c) 2024 natersoz
 */

#pragma once

#include <cstddef>
#include <stdexcept>

template <typename Type>
class binary_tree
{
public:
    ~binary_tree() = default;

    // No copying allowed, move is ok.
    binary_tree(binary_tree const&)                = delete;
    binary_tree(binary_tree&&) noexcept            = default;
    binary_tree& operator=(binary_tree const&)     = delete;
    binary_tree& operator=(binary_tree&&) noexcept = default;

    binary_tree() noexcept : head_node(nullptr), node_count(0u) {}

    using value_type = Type;

    struct tree_node
    {
        ~tree_node()                               = default;
        tree_node(tree_node const&)                = delete;
        tree_node(tree_node&&) noexcept            = default;
        tree_node& operator=(tree_node const&)     = delete;
        tree_node& operator=(tree_node&&) noexcept = default;

        /// @param data_ Will be copied into the tree node.
        tree_node():
            parent(nullptr), left(nullptr), right(nullptr), data() {};

        tree_node(value_type data_):
            parent(nullptr), left(nullptr), right(nullptr), data(data_) {};

        tree_node* parent;
        tree_node* left;
        tree_node* right;
        value_type data;
    };

    void node_insert(tree_node* node)
    {
        this->node_insert(this->head_node, node);
        this->node_count += 1u;
    }

public:
    /**
     * Iterate to the next highest value node in the tree.
     *
     * @param node               The starting position to begin iteration.
     * @return tree_node const*  The next highest value node in the tree.
     */
    tree_node const* get_node_next(tree_node const* node) const
    {
        if (not node)
        {
            return nullptr;
        }

        if (node->right)
        {
            return node->right;
        }
        else if (node->parent)
        {
            if (node->parent->left == node)
            {
                return node->parent;
            }
            if (node->parent->right == node)
            {
                return node->parent->parent;
            }
        }
        else
        {
            return nullptr;
        }
    }

    /**
     * Get the left most node in the tree, starting with the specified node.
     *
     * @param node              The node to begin the iteration from.
     * @return tree_node const* The resulting left most child in the tree.
     */
    tree_node const* get_node_left_most(tree_node const* node) const
    {
        if (not node)
        {
            return nullptr;
        }

        while (node->left)
        {
            node = node->left;
        }

        return node;
    }

    /**
     * Get the right most node in the tree, starting with the specified node.
     *
     * @param node              The node to begin the iteration from.
     * @return tree_node const* The resulting right most child in the tree.
     */
    tree_node const* get_node_right_most(tree_node const* node) const
    {
        if (not node)
        {
            return nullptr;
        }

        while (node->right)
        {
            node = node->right;
        }

        return node;
    }

    /**
     * Add a node, inserting the node as a possible child of the position.
     *
     * @param pos  The position in the tree to insert the new node after.
     * @param node The node to insert.
     */
    void node_insert(tree_node *pos, tree_node* node)
    {
        // Make sure the added node is clean:
        node->parent  = nullptr;
        node->left    = nullptr;
        node->right   = nullptr;

        // If pos is null, then the new node goes there.
        if(not pos)
        {
            pos = node;
            return;
        }

        // Insert left if comparison is <, right if comparision is >.
        if (node->data < pos->data)
        {
            // If the left child is occupied, insert after the left child.
            if (pos->left)
            {
                this->node_insert(pos->left, node);
            }
            else
            {
                // Left child is vacant; add to left child of pos.
                node->parent = pos;
                pos->left    = node;
            }
        }
        else if (node->data > pos->data)
        {
            if (pos->right)
            {
                this->node_insert(pos->right, node);
            }
            else
            {
                node->parent = pos;
                pos->right   = node;
            }
        }
        else
        {
            // reject insertion
            throw std::runtime_error("duplicate node value");
        }
    }

    constexpr size_t size() const
    {
        return this->node_count;
    }

// private:
    tree_node*  head_node;
    size_t      node_count;
};
