#pragma once

#include "type_list.hpp"

#include <cstddef>
#include <type_traits>

// tuple can be defined using two ways, recursive inheritance or multiple
// inheritance, here recursive inheritance is used

namespace fire {
    template <typename... ELEMS>
    struct tuple
    {
        constexpr tuple() = default;
    };

    template <typename ELEM0, typename... ELEM1toN>
    struct tuple<ELEM0, ELEM1toN...> : tuple<ELEM1toN...>
    {
        explicit constexpr tuple(ELEM0 e1, ELEM1toN... rest)
            : tuple<ELEM1toN...>(rest...), data(e1)
        {
        }

        ELEM0 data;
    };

    template <typename T, typename... Ts>
    tuple(T e1, Ts... rest)
        -> tuple<std::unwrap_ref_decay_t<T>, std::unwrap_ref_decay_t<Ts>...>;

    template <typename... ELEMS>
    auto make_tuple(ELEMS... elems)
    {
        return tuple<std::unwrap_ref_decay_t<ELEMS>...>{ elems... };
    }

    namespace detail {
        template <std::size_t i, typename TUPLE>
        struct get_impl : get_impl<i - 1, pop_front_t<TUPLE>>
        {
        };

        template <typename TUPLE>
        struct get_impl<0, TUPLE>
        {
            template <typename T>
            static constexpr auto get(T& t) -> decltype(auto)
            {
                // if constexpr (std::is_const_v<T>)
                // {
                //     return static_cast<const TUPLE&>(t).data;
                // }
                // return static_cast<TUPLE&>(t).data;
                return t.data;
            }
        };
    }

    template <std::size_t i, typename TUPLE>
    constexpr auto get(TUPLE& tuple) -> decltype(auto)
    {
        return detail::get_impl<i, std::remove_cvref_t<TUPLE>>::get(tuple);
    }
}