/**
 * @file array_test.cc
 * @copyright (c) 2018, natersoz. Distributed under the Apache 2.0 license.
 */

#include <array>
#include <iostream>
#include <cassert>

class thing_1
{
public:
    ~thing_1()
    {
        std::cout << "thing_1 dtor @ " << this << std::endl;
    }

    thing_1(): data_1(1), data_2(2)
    {
        std::cout << "thing_1 ctor @ " << this << std::endl;
    }

    int data_1;
    int data_2;
};

int main()
{
    thing_1 thing_array[3u];

    std::cout << "data_1: " << thing_array[0u].data_1 << std::endl;
    std::cout << "data_2: " << thing_array[0u].data_2 << std::endl;

    return 0;
}
