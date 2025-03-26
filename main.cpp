#include "tuple.hpp"

#include <iostream>

auto main() -> int
{
    using namespace fire;

    auto t = make_tuple(8, true, 5.6);
    std::cout << t.data << '\n';

    tuple t2{ 1, false, 5 };
    std::cout << t2.data << '\n';
    std::cout << get<2>(t2) << '\n';

    return 0;
}