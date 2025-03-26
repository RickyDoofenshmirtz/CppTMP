#pragma once

#include "type_list.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

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
        template <typename T, typename... Ts>
        explicit constexpr tuple(T&& e1, Ts&&... rest)
            : tuple<ELEM1toN...>(std::forward<Ts>(rest)...),
              data(std::forward<T>(e1))

        {
        }

        ELEM0 data;
    };

    template <typename T, typename... Ts>
    tuple(T e1, Ts... rest)
        -> tuple<std::unwrap_ref_decay_t<T>, std::unwrap_ref_decay_t<Ts>...>;

    template <typename... ELEMS>
    auto make_tuple(ELEMS&&... elems)
    {
        return tuple<std::unwrap_ref_decay_t<ELEMS>...>{ std::forward<ELEMS>(
            elems)... };
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
            static constexpr auto get(T&& t) -> decltype(auto)
            {
                static constexpr bool is_lvalue = std::is_lvalue_reference_v<T>;
                static constexpr bool is_const =
                    std::is_const_v<std::remove_reference_t<T>>;

                using data_t = front_t<TUPLE>;

                if constexpr (is_const && is_lvalue)
                {
                    return static_cast<const data_t&>(
                        static_cast<const TUPLE&>(std::forward<T>(t)).data);
                }

                if constexpr (!is_const && is_lvalue)
                {
                    return static_cast<data_t&>(
                        static_cast<TUPLE&>(std::forward<T>(t)).data);
                }

                if constexpr (!is_const && !is_lvalue)
                {
                    return static_cast<data_t&&>(
                        static_cast<TUPLE&&>(std::forward<T>(t)).data);
                }

                if constexpr (is_const && !is_lvalue)
                {
                    return static_cast<const data_t&&>(
                        static_cast<const TUPLE&&>(std::forward<T>(t)).data);
                }
            }
        };
    }

    template <std::size_t i, typename TUPLE>
    constexpr auto get(TUPLE&& tuple) -> decltype(auto)
    {
        return detail::get_impl<i, std::remove_cvref_t<TUPLE>>::get(
            std::forward<TUPLE>(tuple));
    }
} // namespace fire