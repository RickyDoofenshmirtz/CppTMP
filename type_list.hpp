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

//////////////////////////////////////////////////////////////////////////////////////
// empty

template <typename LIST>
struct empty : std::false_type
{
};

template <template <typename...> class LIST>
struct empty<LIST<>> : std::true_type
{
};

static_assert(empty<type_list<>>::value);

template <typename LIST>
static constexpr bool empty_v = empty<LIST>::value;

//////////////////////////////////////////////////////////////////////////////////////
// front

template <typename LIST>
struct front;

/*
for the class to work for all type instead of just type_list we can use template
template parameters as a container type by adding following as first parameter

    template <typename...> class LIST
*/

template <template <typename...> class LIST, typename T0, typename... T1toN>
struct front<LIST<T0, T1toN...>> : has_type<T0>
{
};

template <typename LIST>
using front_t = typename front<LIST>::type;

static_assert(std::is_same_v<front_t<type_list<int, bool, double>>, int>);

//////////////////////////////////////////////////////////////////////////////////////
// pop front

template <typename LIST>
struct pop_front
{
};

template <template <typename...> class LIST, typename T0, typename... T1toN>
struct pop_front<LIST<T0, T1toN...>> : has_type<type_list<T1toN...>>
{
};

template <typename LIST>
using pop_front_t = typename pop_front<LIST>::type;

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

template <template <typename...> class LIST, typename T0>
struct back<LIST<T0>> : has_type<T0>
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

template <template <typename...> class LIST, typename... T0toN, typename T>
struct push_back<LIST<T0toN...>, T> : has_type<LIST<T0toN..., T>>
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

template <template <typename...> class LIST, typename T0, typename RET_LIST>
struct pop_back<LIST<T0>, RET_LIST> : has_type<RET_LIST>
{
};

template <
    template <typename...> class LIST,
    typename T0,
    typename T1,
    typename... T2toN,
    typename RET_LIST>
struct pop_back<LIST<T0, T1, T2toN...>, RET_LIST>
    : pop_back<LIST<T1, T2toN...>, push_back_t<RET_LIST, T0>>
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

static_assert(std::is_same_v<
              typename at<type_list<int, bool, double>, 1>::type,
              bool>);

//////////////////////////////////////////////////////////////////////////////////////
// any
// template template parameters can also be used as actual inputs to algorithms

template <template <typename> class PREDICATE, typename LIST>
struct any;

template <
    template <typename> class PREDICATE,
    template <typename...> class LIST>
struct any<PREDICATE, LIST<>> : std::false_type
{
};

template <template <typename> class PREDICATE, typename LIST>
struct any : if_type<
                 PREDICATE<front_t<LIST>>::value,
                 std::true_type,
                 typename any<PREDICATE, pop_front_t<LIST>>::type>::type
{
};

template <template <typename> class PREDICATE, typename LIST>
static constexpr bool any_v = any<PREDICATE, LIST>::value;

static_assert(any_v<std::is_integral, type_list<int, double, std::string>>);
static_assert(any_v<std::is_integral, type_list<std::string, double, int>>);
static_assert(!any_v<std::is_integral, type_list<std::string, double, float>>);

//////////////////////////////////////////////////////////////////////////////////////
// contains type

//--- old
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

//--- new
template <typename T>
struct same_as_pred
{
    template <typename U>
    struct predicate : std::is_same<T, U>
    {
    };
};

template <typename SEARCH, typename LIST>
static constexpr bool contains_type_v =
    any<same_as_pred<SEARCH>::template predicate, LIST>::value;

type_list<int, bool, double> types;

static_assert(contains_type<int, decltype(types)>::value);
static_assert(contains_type<bool, decltype(types)>::value);
static_assert(!contains_type<float, decltype(types)>::value);

static_assert(contains_type_v<int, decltype(types)>);
static_assert(contains_type_v<bool, decltype(types)>);
static_assert(!contains_type_v<float, decltype(types)>);
