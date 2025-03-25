#include "if_type.hpp"

#include <list>
#include <string>
#include <type_traits>

inline auto contain(const std::string& search, std::list<std::string>& l)
    -> bool
{
    if (l.empty()) { return false; }
    if (search == l.front()) { return true; }
    l.pop_front();
    return contain(search, l);
}

template <typename...>
struct type_list
{
};

template <typename LIST>
struct empty : std::false_type
{
};

template <>
struct empty<type_list<>> : std::true_type
{
};

static_assert(empty<type_list<>>::value);

template <typename LIST>
struct front;

template <typename T0, typename... T1toN>
struct front<type_list<T0, T1toN...>>
{
    using type = T0;
};

static_assert(std::is_same_v<front<type_list<int, bool, double>>::type, int>);

template <typename LIST>
struct pop_front
{
};

template <typename T0, typename... T1toN>
struct pop_front<type_list<T0, T1toN...>>
{
    using type = type_list<T1toN...>;
};

template <typename LIST>
using front_t = typename front<LIST>::type;

template <typename LIST>
using pop_front_t = typename pop_front<LIST>::type;

template <typename LIST>
static constexpr bool empty_v = empty<LIST>::value;

template <typename SEARCH, typename LIST>
struct contains_type;

template <typename SEARCH, typename LIST>
struct contains_type : if_type<
                           std::is_same_v<SEARCH, front_t<LIST>>,
                           std::true_type,
                           contains_type<SEARCH, pop_front_t<LIST>>>::type
{
};

template <typename SEARCH>
struct contains_type<SEARCH, type_list<>> : std::false_type
{
};

type_list<int, bool, double> types;

static_assert(contains_type<int, decltype(types)>::value);
static_assert(contains_type<bool, decltype(types)>::value);
static_assert(!contains_type<float, decltype(types)>::value);