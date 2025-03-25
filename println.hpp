#include <iostream>
#include <utility>

namespace fire {
    inline void println() { std::cout << '\n'; }

    template <typename LAST>
    inline void println(LAST&& t)
    {
        std::cout << std::forward<LAST>(t);
        println();
    }

    template <typename T0, typename... T1toN>
    inline void println(T0&& t, T1toN&&... rest)
    {
        std::cout << std::forward<T0>(t) << ", ";
        println(std::forward<T1toN>(rest)...);
    }
}