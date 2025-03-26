namespace fire {
    template <typename T>
    struct has_type
    {
        using type = T;
    };

    template <bool condition, typename THEN, typename ELSE>
    struct if_type;

    template <typename T, typename E>
    struct if_type<true, T, E> : has_type<T>
    {
    };

    template <typename T, typename E>
    struct if_type<false, T, E> : has_type<E>
    {
    };

    template <bool condition, typename T, typename E>
    using if_t = typename if_type<condition, T, E>::type;
}