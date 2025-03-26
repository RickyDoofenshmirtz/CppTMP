#include "if_type.hpp"

#include <cstddef>
#include <ios>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace fire {
    template <typename SEARCH, typename TUPLE, std::size_t start_from = 0>
    struct contains_type
        : if_type<
              std::is_same_v<
                  std::tuple_element_t<start_from, std::remove_cv_t<TUPLE>>,
                  SEARCH>,
              std::true_type,
              typename if_type<
                  start_from ==
                      std::tuple_size<std::remove_cv_t<TUPLE>>::value - 1,
                  std::false_type,
                  contains_type<
                      SEARCH,
                      std::remove_cv_t<TUPLE>,
                      start_from + 1>>::type>::type
    {
    };

    template <typename SEARCH>
    struct contains_type<SEARCH, std::tuple<>, 0> : std::false_type
    {
    };
}

inline auto constins(
    const std::string& search,
    const std::vector<std::string>& v,
    std::size_t start_from = 0) -> bool
{
    if (v[start_from] == search) { return true; }
    if (start_from == v.size() - 1) { return false; }
    return constins(search, v, start_from + 1);
}

inline void test_contains_type()
{
    const std::vector<std::string> v{ "int", "bool", "float" };
    std::cout << std::boolalpha << constins("bool", v) << '\n';
    std::cout << std::boolalpha << constins("double", v) << '\n';

    const std::tuple<int, bool, float> t;

    [[maybe_unused]]
    static constexpr auto val = fire::contains_type<bool, decltype(t)>::value;

    std::cout << std::boolalpha << fire::contains_type<bool, decltype(t)>::value
              << '\n';
    std::cout << std::boolalpha
              << fire::contains_type<double, decltype(t)>::value << '\n';
}