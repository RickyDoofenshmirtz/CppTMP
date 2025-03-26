#include "type_list.hpp"

#include <tuple>
#include <type_traits>

auto main() -> int
{
    type_list<int, bool, double> l;
    static_assert(std::is_same_v<front_t<decltype(l)>, int>);

    std::tuple<int, bool, double> t;
    static_assert(std::is_same_v<front_t<decltype(t)>, int>);

    return 0;
}