#include "if_type.hpp"

#include <corecrt.h>
#include <cstddef>
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

//////////////////////////////////////////////////////////////////////////////////////
// type list

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

//////////////////////////////////////////////////////////////////////////////////////
// front

template <typename LIST>
struct front;

template <typename T0, typename... T1toN>
struct front<type_list<T0, T1toN...>> : has_type<T0>
{
};

static_assert(std::is_same_v<front<type_list<int, bool, double>>::type, int>);

//////////////////////////////////////////////////////////////////////////////////////
// pop front

template <typename LIST>
struct pop_front
{
};

template <typename T0, typename... T1toN>
struct pop_front<type_list<T0, T1toN...>> : has_type<type_list<T1toN...>>
{
};

template <typename LIST>
using front_t = typename front<LIST>::type;

template <typename LIST>
using pop_front_t = typename pop_front<LIST>::type;

template <typename LIST>
static constexpr bool empty_v = empty<LIST>::value;

//////////////////////////////////////////////////////////////////////////////////////
// back

/*
    variadic templates can only be used as the last parameter, so we will have
    to use recursion
*/

template <typename LIST>
struct back : has_type<typename back<pop_front_t<LIST>>::type>
{
};

template <typename T0>
struct back<type_list<T0>> : has_type<T0>
{
};

template <typename LIST>
using back_t = typename back<LIST>::type;

static_assert(std::is_same_v<back_t<type_list<int, bool, double>>, double>);
static_assert(std::is_same_v<back_t<type_list<int, double, bool>>, bool>);

//////////////////////////////////////////////////////////////////////////////////////
// push back

template <typename LIST, typename T>
struct push_back
{
};

template <typename... T0toN, typename T>
struct push_back<type_list<T0toN...>, T> : has_type<type_list<T0toN..., T>>
{
};

template <typename LIST, typename T>
using push_back_t = typename push_back<LIST, T>::type;

static_assert(std::is_same_v<push_back_t<type_list<>, int>, type_list<int>>);
static_assert(std::is_same_v<
              push_back_t<type_list<int, bool>, double>,
              type_list<int, bool, double>>);

//////////////////////////////////////////////////////////////////////////////////////
// pop back

template <typename LIST, typename RET_LIST = type_list<>>
struct pop_back;

template <typename T0, typename RET_LIST>
struct pop_back<type_list<T0>, RET_LIST> : has_type<RET_LIST>
{
};

template <typename T0, typename T1, typename... T2toN, typename RET_LIST>
struct pop_back<type_list<T0, T1, T2toN...>, RET_LIST>
    : pop_back<type_list<T1, T2toN...>, push_back_t<RET_LIST, T0>>
{
};

template <typename LIST>
using pop_back_t = typename pop_back<LIST>::type;

static_assert(std::is_same_v<pop_back_t<type_list<int>>, type_list<>>);
static_assert(std::is_same_v<
              pop_back_t<type_list<int, bool, double>>,
              type_list<int, bool>>);

//////////////////////////////////////////////////////////////////////////////////////
// at

template <typename LIST, std::size_t index>
struct at;

template <typename LIST, std::size_t index>
struct at : has_type<typename at<pop_front_t<LIST>, index - 1>::type>
{
};

template <typename LIST>
struct at<LIST, 0> : has_type<front_t<LIST>>
{
};

template <typename LIST, std::size_t index>
using at_t = typename at<LIST, index>::type;

static_assert(
    std::is_same_v<typename at<type_list<int, bool, double>, 1>::type, bool>);

//////////////////////////////////////////////////////////////////////////////////////
// contains type

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

//////////////////////////////////////////////////////////////////////////////////////

type_list<int, bool, double> types;

static_assert(contains_type<int, decltype(types)>::value);
static_assert(contains_type<bool, decltype(types)>::value);
static_assert(!contains_type<float, decltype(types)>::value);
