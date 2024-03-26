/**
 * @file test_binary_tree.cc
 */

#include "binary_tree.h"
#include <array>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <cassert>

#if 0   // Just print the integers

template<typename Type>
std::ostream& operator<<(std::ostream &os, binary_tree<Type> const &tree)
{
    for (auto const &tree_iter : tree)
    {
        os << std::dec  << std::setw(4) << std::setfill(' ')
           << tree_iter << std::endl;
    }

    return os;
}

#else   // Full debug printing

template<typename Type>
std::ostream& operator<<(std::ostream &os, binary_tree<Type> const &tree)
{
    os << " head_node: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << &tree.head_node;
    os << " parent: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << tree.head_node->parent;
    os << " left: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << tree.head_node->left;
    os << " right: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << tree.head_node->right;
    os << " data: " << std::dec << std::setw(4) << std::setfill(' ')
       << tree.head_node->data;
    os << std::endl;
#if 0
    for (auto tree_iter = tree.cbegin(); tree_iter != tree.cend(); ++tree_iter)
    {
        os << " node: 0x" << std::hex << std::setw(8) << std::setfill('0')
           << tree_iter.node_;
        os << " left: 0x" << std::hex << std::setw(8) << std::setfill('0')
           << tree_iter.node_->next_;
        os << " right: 0x" << std::hex << std::setw(8) << std::setfill('0')
           << tree_iter.node_->prev_;
        os << " data: " << std::dec << std::setw(4) << std::setfill(' ')
           << tree_iter.node_->data;
        os << std::endl;
    }
#endif
    return os;
}
#endif

#if 0
template<typename Type>
void check_ascending_forward(binary_tree<Type> &tree)
{
    auto last_iter = tree.end();
    --last_iter;
    for (auto tree_iter = tree.begin(); tree_iter != last_iter; )
    {
        int const value_1 = *tree_iter++;
        int const value_2 = *tree_iter;
        assert(value_2 == value_1 + 1);
    }
}

template<typename Type>
void check_descending_reverse(binary_tree<Type> &tree)
{
    auto last_iter = tree.end();
    ++last_iter;
    for (auto tree_iter = tree.rbegin(); tree_iter != last_iter; )
    {
        int const value_1 = *tree_iter--;
        int const value_2 = *tree_iter;
        assert(value_2 == value_1 - 1);
    }
}

template<typename Type>
void check_nodes(binary_tree<Type> &tree)
{
    for (auto tree_iter = tree.begin(); tree_iter != tree.end(); ++tree_iter)
    {
        assert (tree_iter.node_->next_->prev_ == tree_iter.node_);
        assert (tree_iter.node_->prev_->next_ == tree_iter.node_);
    }
}
#endif

int main()
{
    int result = 0;

    binary_tree<int> tree_1;
    std::array<binary_tree<int>::tree_node, 10u> nodes_fwd;

    for (binary_tree<int>::tree_node& tree_node : nodes_fwd)
    {
        // Push tree nodes to the back of the tree with ascending integer data.
        // Result in tree data: {1 ... 10}
        tree_node.data = &tree_node - nodes_fwd.begin() + 1;
        tree_1.node_insert(&tree_node);
    }

    std::cout << "--- tree 1 created:" << std::endl;
    std::cout << tree_1 << std::endl;
#if 0
    check_ascending_forward(tree_1);
    check_descending_reverse(tree_1);
    check_nodes(tree_1);

    assert(tree_1.front() ==  1);
    assert(tree_1.back()  == 10);
    assert(tree_1.size()  == 10);

    binary_tree<int> tree_2;
    binary_tree<int>::tree_node nodes_rev[10];

    for (binary_tree<int>::tree_node& tree_node : nodes_rev)
    {
        // Push tree nodes to the front of the tree with ascending values
        // which are <= 0. The tree data added: { -9 ... 0 }
        tree_node.data = (std::begin(nodes_rev) - &tree_node);
        tree_2.push_front(tree_node);
    }

    std::cout << "--- tree 2 created:" << std::endl;
    std::cout << tree_2 << std::endl;

    check_ascending_forward(tree_2);
    check_descending_reverse(tree_2);
    check_nodes(tree_2);

    assert(tree_2.front() == -9);
    assert(tree_2.back()  ==  0);
    assert(tree_2.size()  == 10);

    std::cout << "--- insert tree 2 -> tree 1:" << std::endl;
    tree_1.insert(tree_1.begin(), tree_2.begin(), tree_2.end());

    assert(tree_1.front() == -9);
    assert(tree_1.back()  == 10);
    assert(tree_1.size()  == 20);
    assert(tree_2.size()  ==  0);

    std::cout << "--- tree 1:" << std::endl;
    std::cout << tree_1 << std::endl;

    std::cout << "--- tree 2:" << std::endl;
    std::cout << tree_2 << std::endl;

    check_nodes(tree_1);
    check_nodes(tree_2);

    std::cout << "move tree 1 [-5, 5) -> tree 2" << std::endl;

    auto iter_find_neg5 = std::find(tree_1.begin(), tree_1.end(), -5);
    auto iter_find_pos5 = std::find(tree_1.begin(), tree_1.end(),  5);

    tree_2.insert(tree_2.begin(), iter_find_neg5, iter_find_pos5);

    std::cout << "--- tree 1:" << std::endl;
    std::cout << tree_1 << std::endl;

    std::cout << "--- tree 2:" << std::endl;
    std::cout << tree_2 << std::endl;

    check_nodes(tree_1);
    check_nodes(tree_2);

    assert(tree_1.front() == -9);
    assert(tree_1.back()  == 10);

    assert(tree_2.front() == -5);
    assert(tree_2.back()  ==  4);

    // Test the front and back functions.
    tree_1.pop_front();
    assert(tree_1.front() == -8);
    tree_1.pop_back();
    assert(tree_1.back()  ==  9);

    // Test the begin and end functions.
    assert(*tree_1.begin() == *tree_1.cbegin());
    assert(*tree_1.end()   == *tree_1.cend());

    // Test erase as well and push_front().
    {
        auto iter_begin = tree_1.begin();
        tree_1.erase(iter_begin);
        assert(tree_1.front() == -7);
        check_nodes(tree_1);
        tree_1.push_front(iter_begin);
        assert(tree_1.front() == -8);
        check_nodes(tree_1);
    }

    // Test erase and push_back().
    {
        auto iter_rbegin = tree_1.rbegin();
        tree_1.erase(iter_rbegin);
        assert(tree_1.back() == 8);
        check_nodes(tree_1);
        tree_1.push_back(iter_rbegin);
        assert(tree_1.back() == 9);
        check_nodes(tree_1);
    }

    std::cout << "append tree_2 to the end of tree 1" << std::endl;
    tree_1.insert(tree_1.end(), tree_2.begin(), tree_2.end());

    std::cout << "--- tree 1:" << std::endl;
    std::cout << tree_1 << std::endl;

    std::cout << "--- tree 2:" << std::endl;
    std::cout << tree_2 << std::endl;

    check_nodes(tree_1);
    check_nodes(tree_2);

    std::cout << "--- Tests Pass" << std::endl;
#endif
    return result;
}
