/**
 * @file test_list.cc
 */

#include "linked_list.h"
#include <iostream>
#include <iomanip>
#include <cassert>

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
       << &list.sentinal_;
    os << " next: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << list.sentinal_.next_;
    os << " prev: 0x" << std::hex << std::setw(8) << std::setfill('0')
       << list.sentinal_.prev_;
//    os << " data: " << std::dec << std::setw(4) << std::setfill(' ')
//       << list.sentinal_.data_;
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
        assert(value_2 == value_1 + 1);
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
        assert(value_2 == value_1 - 1);
    }
}

template<typename Type>
void check_nodes(linked_list<Type> &list)
{
    for (auto list_iter = list.begin(); list_iter != list.end(); ++list_iter)
    {
        assert (list_iter.node_->next_->prev_ == list_iter.node_);
        assert (list_iter.node_->prev_->next_ == list_iter.node_);
    }
}

int main()
{
    int result = 0;

    linked_list<int> list_1;
    linked_list<int>::list_node nodes_fwd[10];

    for (linked_list<int>::list_node& list_node : nodes_fwd)
    {
        // Push list nodes to the back of the list with ascending integer data.
        // Result in list data: {1 ... 10}
        list_node.data_ = (&list_node - std::begin(nodes_fwd)) + 1;
        list_1.push_back(list_node);
    }

    std::cout << "--- list 1 created:" << std::endl;
    std::cout << list_1 << std::endl;

    check_ascending_forward(list_1);
    check_descending_reverse(list_1);
    check_nodes(list_1);

    assert(list_1.front() ==  1);
    assert(list_1.back()  == 10);
    assert(list_1.size()  == 10);

    linked_list<int> list_2;
    linked_list<int>::list_node nodes_rev[10];

    for (linked_list<int>::list_node& list_node : nodes_rev)
    {
        // Push list nodes to the front of the list with ascending values
        // which are <= 0. The list data added: { -9 ... 0 }
        list_node.data_ = (std::begin(nodes_rev) - &list_node);
        list_2.push_front(list_node);
    }

    std::cout << "--- list 2 created:" << std::endl;
    std::cout << list_2 << std::endl;

    check_ascending_forward(list_2);
    check_descending_reverse(list_2);
    check_nodes(list_2);

    assert(list_2.front() == -9);
    assert(list_2.back()  ==  0);
    assert(list_2.size()  == 10);

    std::cout << "--- insert list 2 -> list 1:" << std::endl;
    list_1.insert(list_1.begin(), list_2.begin(), list_2.end());

    assert(list_1.front() == -9);
    assert(list_1.back()  == 10);
    assert(list_1.size()  == 20);
    assert(list_2.size()  ==  0);

    std::cout << "--- list 1:" << std::endl;
    std::cout << list_1 << std::endl;

    std::cout << "--- list 2:" << std::endl;
    std::cout << list_2 << std::endl;

    check_nodes(list_1);
    check_nodes(list_2);

    std::cout << "move list 1 [-5, 5) -> list 2" << std::endl;

    auto iter_find_neg5 = std::find(list_1.begin(), list_1.end(), -5);
    auto iter_find_pos5 = std::find(list_1.begin(), list_1.end(),  5);

    list_2.insert(list_2.begin(), iter_find_neg5, iter_find_pos5);

    std::cout << "--- list 1:" << std::endl;
    std::cout << list_1 << std::endl;

    std::cout << "--- list 2:" << std::endl;
    std::cout << list_2 << std::endl;

    check_nodes(list_1);
    check_nodes(list_2);

    assert(list_1.front() == -9);
    assert(list_1.back()  == 10);

    assert(list_2.front() == -5);
    assert(list_2.back()  ==  4);

    // Test the front and back functions.
    list_1.pop_front();
    assert(list_1.front() == -8);
    list_1.pop_back();
    assert(list_1.back()  ==  9);

    // Test the begin and end functions.
    assert(*list_1.begin() == *list_1.cbegin());
    assert(*list_1.end()   == *list_1.cend());

    // Test erase as well and push_front().
    {
        auto iter_begin = list_1.begin();
        list_1.erase(iter_begin);
        assert(list_1.front() == -7);
        check_nodes(list_1);
        list_1.push_front(iter_begin);
        assert(list_1.front() == -8);
        check_nodes(list_1);
    }

    // Test erase and push_back().
    {
        auto iter_rbegin = list_1.rbegin();
        list_1.erase(iter_rbegin);
        assert(list_1.back() == 8);
        check_nodes(list_1);
        list_1.push_back(iter_rbegin);
        assert(list_1.back() == 9);
        check_nodes(list_1);
    }

    std::cout << "append list_2 to the end of list 1" << std::endl;
    list_1.insert(list_1.end(), list_2.begin(), list_2.end());

    std::cout << "--- list 1:" << std::endl;
    std::cout << list_1 << std::endl;

    std::cout << "--- list 2:" << std::endl;
    std::cout << list_2 << std::endl;

    check_nodes(list_1);
    check_nodes(list_2);

    std::cout << "--- Tests Pass" << std::endl;

    return result;
}
