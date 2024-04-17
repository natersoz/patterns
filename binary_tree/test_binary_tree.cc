/**
 * @file test_binary_tree.cc
 */

// #include "binary_tree.h"
// #include "utility/make_array.h"

#include <array>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <cassert>

#include "gtest/gtest.h"

#include "binary_tree/binary_tree.h"

using namespace testing;


// print the binary tree test_values
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

template<typename Type>
void check_ascending_forward(binary_tree<Type> &tree)
{
    auto last_iter = tree.end();
    --last_iter;
    for (auto tree_iter = tree.begin(); tree_iter != last_iter; )
    {
        int const value_1 = *tree_iter++;
        int const value_2 = *tree_iter;
        EXPECT_GT(value_2, value_1);
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
        EXPECT_LT(value_2, value_1);
    }
}

class TestBinaryTree : public Test
{
public:
    // Test data:
    static constexpr std::array<int, 19u> const test_values = {
    //    1    2    3    4    5    6    7    8    9   10
        100,  50, 200,  25,  75,  10,  60, 150, 140, 130,
        135, 300, 400, 350, 275, 375, 380, 385,  80
    };

    // Values that are not to be found within the test_data.
    // Used to check that these cannot be found within the tree.
    static constexpr std::array<int, 3u> const invalid_values = {-1, 1001, 401};

    std::array<binary_tree<int>::tree_node, test_values.size()> tree_nodes;

    binary_tree<int> test_tree;

    TestBinaryTree() : Test(), test_tree()
    {
        EXPECT_TRUE(test_tree.empty());

        // Check that no values in the invalid_values array are found in the test_values array.
        for (auto invalid_value : invalid_values)
        {
            EXPECT_EQ(
                std::find(test_values.begin(), test_values.end(), invalid_value),
                test_values.end());
        }
    }

    void fill_test_data()
    {
        auto value_iter = test_values.begin();
        for (binary_tree<int>::tree_node& node : tree_nodes)
        {
            node.data = *value_iter++;
            this->test_tree.insert(node);
        }

        // std::cout << "--- tree 1 created:" << std::endl;
        // std::cout << test_tree << std::endl;
    }
};

TEST_F(TestBinaryTree, check_filled_test_data)
{
    EXPECT_TRUE(test_tree.empty());
    EXPECT_EQ(test_tree.size(), 0u);

    fill_test_data();

    check_ascending_forward(test_tree);
    check_descending_reverse(test_tree);

    EXPECT_EQ(test_tree.size(), test_values.size());
    EXPECT_FALSE(test_tree.empty());

    // Check that all test_values can be found in the tree and
    // that the returned iterator dereferences correctly.
    for (int const& value : test_values)
    {
        auto iter = test_tree.find(value);
        EXPECT_EQ(value, *iter);
    }

    // Check that invalid values are not found within the binary tree.
    for (int const value : invalid_values)
    {
        auto iter = test_tree.find(value);
        EXPECT_EQ(test_tree.end(), iter);
    }
}

TEST_F(TestBinaryTree, check_test_data_erasures)
{
    EXPECT_TRUE(test_tree.empty());
    EXPECT_EQ(test_tree.size(), 0u);

    fill_test_data();

    check_ascending_forward(test_tree);
    check_descending_reverse(test_tree);

    EXPECT_EQ(test_tree.size(), test_values.size());
    EXPECT_FALSE(test_tree.empty());

    // Check that all test_values can be found in the tree and
    // that the returned iterator dereferences correctly.
    for (int const& value : test_values)
    {
        auto iter = test_tree.find(value);
        EXPECT_EQ(value, *iter);
    }

    auto iter_50 = test_tree.find(50);
    test_tree.erase(iter_50);
    auto iter_50_bad = test_tree.find(50);
    EXPECT_EQ(iter_50_bad, test_tree.end());
    check_ascending_forward(test_tree);
    check_descending_reverse(test_tree);

    auto iter_80 = test_tree.find(80);
    test_tree.erase(iter_80);
    auto iter_80_bad = test_tree.find(80);
    EXPECT_EQ(iter_80_bad, test_tree.end());
    check_ascending_forward(test_tree);
    check_descending_reverse(test_tree);

    while(test_tree.size() > 0u)
    {
        EXPECT_FALSE(test_tree.empty());
        auto iter = test_tree.begin();
        test_tree.erase(iter);
        auto iter_bad = test_tree.find(80);
        EXPECT_EQ(iter_bad, test_tree.end());
        check_ascending_forward(test_tree);
        check_descending_reverse(test_tree);
    }
}

TEST_F(TestBinaryTree, check_test_data_clear)
{
    EXPECT_TRUE(test_tree.empty());
    EXPECT_EQ(test_tree.size(), 0u);

    fill_test_data();

    check_ascending_forward(test_tree);
    check_descending_reverse(test_tree);

    EXPECT_EQ(test_tree.size(), test_values.size());
    EXPECT_FALSE(test_tree.empty());

    // Check that all test_values can be found in the tree and
    // that the returned iterator dereferences correctly.
    for (int const& value : test_values)
    {
        auto iter = test_tree.find(value);
        EXPECT_EQ(value, *iter);
    }

    test_tree.clear();
    EXPECT_TRUE(test_tree.empty());
    EXPECT_EQ(test_tree.size(), 0u);
}
