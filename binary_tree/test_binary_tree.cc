/**
 * @file test_binary_tree.cc
 */

#include "binary_tree.h"
#include "utility/make_array.h"

#include <array>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <cassert>

#if 1   // Just print the integers

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

// template<typename Type>
std::ostream& operator<<(std::ostream &os, typename binary_tree<int>::tree_node const& node)
{
    os << " node: " << std::hex << std::setw(8) << std::setfill('0')
       << &node;
    os << " parent: " << std::hex << std::setw(8) << std::setfill('0')
       << node.parent;
    os << " left:   " << std::hex << std::setw(8) << std::setfill('0')
       << node.left;
    os << " right:  " << std::hex << std::setw(8) << std::setfill('0')
       << node.right;
    os << " data:   " << std::dec << std::setw(4) << std::setfill(' ')
       << node.data;
    return os;
}

// template<typename Type>
std::ostream& operator<<(std::ostream &os, binary_tree<int> const &tree)
{
    os << "head: " ;
    os << *tree.head_node << std::endl;

    for (auto iter = tree.begin(); iter != tree.end(); ++iter)
    {
        os << "iter: " << *iter.pos << std::endl;
    }

    return os;
}
#endif


template<typename Type>
void check_ascending_forward(binary_tree<Type> &tree)
{
    auto last_iter = tree.end();
    --last_iter;
    for (auto tree_iter = tree.begin(); tree_iter != last_iter; )
    {
        int const value_1 = *tree_iter++;
        int const value_2 = *tree_iter;
        assert(value_2 > value_1);
    }
}

template<typename Type>
void check_descending_reverse(binary_tree<Type> &tree)
{
    auto last_iter = tree.rend();
    --last_iter;
    for (auto tree_iter = tree.rbegin(); tree_iter != last_iter; )
    {
        int const value_1 = *tree_iter++;
        int const value_2 = *tree_iter;
        // std::cout << std::dec << std::setw(4) << std::setfill(' ') << value_1 << ", ";
        // std::cout << std::dec << std::setw(4) << std::setfill(' ') << value_2 << std::endl;
        assert(value_2 < value_1);
    }
}

int main()
{
    int result = 0;

    binary_tree<int> tree_1;
    assert(tree_1.empty());

    // Test data:
    std::array<int, 19u> values = {
    //    1    2    3    4    5    6    7    8    9   10
        100,  50, 200,  25,  75,  10,  60, 150, 140, 130,
        135, 300, 400, 350, 275, 375, 380, 385,  80
    };

    std::array<binary_tree<int>::tree_node, values.size()> tree_nodes =
        make_initialized_array<binary_tree<int>::tree_node>(values);

    static_assert(tree_nodes.size() == values.size());

    for (binary_tree<int>::tree_node& node : tree_nodes)
    {
        tree_1.insert(node);
    }

    std::cout << "--- tree 1 created:" << std::endl;
    std::cout << tree_1 << std::endl;

    check_ascending_forward(tree_1);
    check_descending_reverse(tree_1);

    assert(tree_1.size() == values.size());
    assert(not tree_1.empty());

    // Check that all values can be found in the tree and
    // that the returned iterator dereferences correctly.
    for (int const& value : values)
    {
        auto iter = tree_1.find(value);
        assert(value == *iter);
    }

    // Searching for a value that does not exist should return end().
    {
        auto const iter = tree_1.find(10001);
        assert(iter == tree_1.end());
    }

    {
        auto iter_50 = tree_1.find(50);
        tree_1.erase(iter_50);
        check_ascending_forward(tree_1);
        check_descending_reverse(tree_1);

        auto iter_80 = tree_1.find(80);
        tree_1.erase(iter_80);
        check_ascending_forward(tree_1);
        check_descending_reverse(tree_1);

        while(tree_1.size() > 0u)
        {
            assert(not tree_1.empty());
            auto iter = tree_1.begin();
            tree_1.erase(iter);
            check_ascending_forward(tree_1);
            check_descending_reverse(tree_1);
        }
    }

    tree_1.clear();
    assert(tree_1.empty());
    assert(tree_1.size() == 0u);

    return result;
}
