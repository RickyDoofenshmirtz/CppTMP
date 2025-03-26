#include "tuple.hpp"

#include <iostream>

auto main() -> int
{
    using namespace fire;

    auto t = fire::make_tuple(8, true, 5.6);
    std::cout << t.data() << '\n';
    fire::get<2>(t) = 4.2;

    constexpr fire::tuple t2{ 1, false, 5 };
    std::cout << t2.data() << '\n';
    std::cout << fire::get<2>(t2) << '\n';

    [[maybe_unused]] static constexpr auto x = fire::get<2>(t2);

    std::cout << fire::get<2>(fire::tuple(2, 5, 6)) << '\n';

    return 0;
}