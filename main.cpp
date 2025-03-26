#include "tuple.hpp"

#include <iostream>

auto main() -> int
{
    using namespace fire;

    auto t = fire::make_tuple(8, true, 5.6);
    std::cout << t.data << '\n';

    fire::tuple t2{ 1, false, 5 };
    std::cout << t2.data << '\n';
    std::cout << fire::get<2>(t2) << '\n';

    std::cout << fire::get<2>(fire::tuple(2, 5, 6)) << '\n';

    fire::get<2>(t) = 4.2;

    return 0;
}