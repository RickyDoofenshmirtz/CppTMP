template <bool condition, typename THEN, typename ELSE>
struct if_type;

template <typename T, typename E>
struct if_type<true, T, E>
{
    using type = T;
};

template <typename T, typename E>
struct if_type<false, T, E>
{
    using type = E;
};

template <bool condition, typename T, typename E>
using if_t = typename if_type<condition, T, E>::type;