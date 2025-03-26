#include "type_list.hpp"

#include <type_traits>

auto main() -> int
{
    type_list<bool, int, double> l;
    static_assert(std::is_same_v<front_t<decltype(l)>, bool>);
    return 0;
}