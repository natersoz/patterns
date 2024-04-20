/**
 * @file test_linked_list.cc
 */

#include "gtest/gtest.h"

#include "linked_list.h"

#include <array>
#include <iostream>
#include <iomanip>
#include <cassert>

using namespace testing;


#if 0   // Just print the integers

template<typename Type>
std::ostream& operator<<(std::ostream &os, linked_list<Type> const &list)
{
    for (auto const &list_iter : list)
    {
        os << std::dec  << std::setw(4) << std::setfill(' ')
           << list_iter << std::endl;
    }

    return os;
}

#else   // Full debug printing

template<typename Type>
std::ostream& operator<<(std::ostream &os, linked_list<Type> const &list)
{
    os << " sntl: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << &list.sentinel_;
    os << " next: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << list.sentinel_.next_;
    os << " prev: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << list.sentinel_.prev_;
//    os << " data: " << std::dec << std::setw(4) << std::setfill(' ')
//       << list.sentinel_.data_;
    os << std::endl;

    for (auto list_iter = list.cbegin(); list_iter != list.cend(); ++list_iter)
    {
        os << " node: 0x" << std::hex << std::setw(8) << std::setfill('0')
           << list_iter.node_;
        os << " next: 0x" << std::hex << std::setw(8) << std::setfill('0')
           << list_iter.node_->next_;
        os << " prev: 0x" << std::hex << std::setw(8) << std::setfill('0')
           << list_iter.node_->prev_;
        os << " data: " << std::dec << std::setw(4) << std::setfill(' ')
           << list_iter.node_->data_;
        os << std::endl;
    }

    return os;
}
#endif

template<typename Type>
void check_ascending_forward(linked_list<Type> &list)
{
    auto last_iter = list.end();
    --last_iter;
    for (auto list_iter = list.begin(); list_iter != last_iter; )
    {
        int const value_1 = *list_iter++;
        int const value_2 = *list_iter;
        EXPECT_EQ(value_2, value_1 + 1);
    }
}

template<typename Type>
void check_descending_reverse(linked_list<Type> &list)
{
    auto last_iter = list.end();
    ++last_iter;
    for (auto list_iter = list.rbegin(); list_iter != last_iter; )
    {
        int const value_1 = *list_iter--;
        int const value_2 = *list_iter;
        EXPECT_EQ(value_2, value_1 - 1);
    }
}

template<typename Type>
void check_nodes(linked_list<Type> &list)
{
    for (auto list_iter = list.begin(); list_iter != list.end(); ++list_iter)
    {
        EXPECT_EQ(list_iter.node_->next_->prev_, list_iter.node_);
        EXPECT_EQ(list_iter.node_->prev_->next_, list_iter.node_);
    }
}

class TestLinkedList : public Test
{
public:
    linked_list<int>                             test_list_forward;
    linked_list<int>                             test_list_reverse;
    std::array<linked_list<int>::list_node, 10u> list_nodes_forward;
    std::array<linked_list<int>::list_node, 10u> list_nodes_reverse;

    TestLinkedList() : Test(), test_list_forward(), test_list_reverse()
    {
        EXPECT_TRUE(test_list_forward.empty());
    }

    void fill_test_data_forward()
    {
        for (linked_list<int>::list_node& list_node : list_nodes_forward)
        {
            // Push list nodes to the back of the list with ascending integer data.
            // Result in list data: {1 ... 10}
            list_node.data_ = (&list_node - std::begin(list_nodes_forward)) + 1;
            test_list_forward.push_back(list_node);
        }

        // std::cout << "--- list forward created:" << std::endl;
        // std::cout << test_list_forward << std::endl;
    }

    void fill_test_data_reverse()
    {
        for (linked_list<int>::list_node& list_node : list_nodes_reverse)
        {
            // Push list nodes to the front of the list with ascending values
            // which are <= 0. The list data added: { -9 ... 0 }
            list_node.data_ = (std::begin(list_nodes_reverse) - &list_node);
            test_list_reverse.push_front(list_node);
        }

        // std::cout << "--- list reverse created:" << std::endl;
        // std::cout << test_list_reverse << std::endl;
    }
};

TEST_F(TestLinkedList, check_filled_test_data)
{
    fill_test_data_forward();
    fill_test_data_reverse();

    check_ascending_forward(test_list_forward);
    check_descending_reverse(test_list_forward);
    check_nodes(test_list_forward);

    EXPECT_EQ(test_list_forward.front(), 1);
    EXPECT_EQ(test_list_forward.back(), 10);
    EXPECT_EQ(test_list_forward.size(), list_nodes_forward.size());

    check_ascending_forward(test_list_reverse);
    check_descending_reverse(test_list_reverse);
    check_nodes(test_list_reverse);

    EXPECT_EQ(test_list_reverse.front(), -9);
    EXPECT_EQ(test_list_reverse.back(),   0);
    EXPECT_EQ(test_list_reverse.size(),  10);

    // std::cout << "--- insert list 2 -> list 1:" << std::endl;
    test_list_forward.insert(test_list_forward.begin(),
                             test_list_reverse.begin(),
                             test_list_reverse.end());

    EXPECT_EQ(test_list_forward.front(), -9);
    EXPECT_EQ(test_list_forward.back(),  10);
    EXPECT_EQ(test_list_forward.size(),  20);
    EXPECT_EQ(test_list_reverse.size(),   0);

    // std::cout << "--- list 1:" << std::endl;
    // std::cout << test_list_forward << std::endl;

    // std::cout << "--- list 2:" << std::endl;
    // std::cout << test_list_reverse << std::endl;

    check_nodes(test_list_forward);
    check_nodes(test_list_reverse);

    // std::cout << "move list 1 [-5, 5) -> list 2" << std::endl;

    auto iter_find_neg5 = std::find(test_list_forward.begin(), test_list_forward.end(), -5);
    auto iter_find_pos5 = std::find(test_list_forward.begin(), test_list_forward.end(),  5);

    test_list_reverse.insert(test_list_reverse.begin(), iter_find_neg5, iter_find_pos5);

    // std::cout << "--- list 1:" << std::endl;
    // std::cout << test_list_forward << std::endl;

    // std::cout << "--- list 2:" << std::endl;
    // std::cout << test_list_reverse << std::endl;

    check_nodes(test_list_forward);
    check_nodes(test_list_reverse);

    EXPECT_EQ(test_list_forward.front(), -9);
    EXPECT_EQ(test_list_forward.back(),  10);

    EXPECT_EQ(test_list_reverse.front(), -5);
    EXPECT_EQ(test_list_reverse.back(),   4);

    // Test the front and back functions.
    test_list_forward.pop_front();
    EXPECT_EQ(test_list_forward.front(), -8);
    test_list_forward.pop_back();
    EXPECT_EQ(test_list_forward.back(), 9);

    // Test the begin and end functions.
    EXPECT_EQ(*test_list_forward.begin(), *test_list_forward.cbegin());
    EXPECT_EQ(*test_list_forward.end(),   *test_list_forward.cend());

    // Test erase as well and push_front().
    {
        auto iter_begin = test_list_forward.begin();
        test_list_forward.erase(iter_begin);
        EXPECT_EQ(test_list_forward.front(), -7);
        check_nodes(test_list_forward);
        test_list_forward.push_front(iter_begin);
        EXPECT_EQ(test_list_forward.front(), -8);
        check_nodes(test_list_forward);
    }

    // Test erase and push_back().
    {
        auto iter_rbegin = test_list_forward.rbegin();
        test_list_forward.erase(iter_rbegin);
        EXPECT_EQ(test_list_forward.back(), 8);
        check_nodes(test_list_forward);
        test_list_forward.push_back(iter_rbegin);
        EXPECT_EQ(test_list_forward.back(), 9);
        check_nodes(test_list_forward);
    }

    // std::cout << "append test_list_reverse to the end of list 1" << std::endl;
    test_list_forward.insert(test_list_forward.end(),
                             test_list_reverse.begin(),
                             test_list_reverse.end());

    // std::cout << "--- list 1:" << std::endl;
    // std::cout << test_list_forward << std::endl;

    // std::cout << "--- list 2:" << std::endl;
    // std::cout << test_list_reverse << std::endl;

    check_nodes(test_list_forward);
    check_nodes(test_list_reverse);

    // std::cout << "--- Tests Pass" << std::endl;
}
