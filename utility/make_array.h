/**
 * @file make_array.h
 * See
 * https://stackoverflow.com/questions/78130353/c-template-container-instantiation
 */

#pragma once

#include <array>

/**
 * Instantiate a std::array of objects using an array of values required
 * for the objects' instantiation.
 *
 * @tparam N         The number of elements in the instantiated std::array.
 * @tparam NodeType  The instantiated node type.
 * @tparam ParamType The parameter type that NodeType requires for its ctor.
 * @param  params    The parameters used to for NodeType initialization.
 * @return constexpr std::array<NodeType, N> The initialized array.
 */
template <typename NodeType, std::size_t N, typename ParamType>
constexpr std::array<NodeType, N> make_initialized_array(std::array<ParamType, N>& params)
{
    return [&]<std::size_t...Is>(std::index_sequence<Is...>){
        return std::array<NodeType, N>{{ NodeType(params[Is])... }};
    }(std::make_index_sequence<N>());
}
