//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CPP_14_MPL_HPP
#define BOOST_DI_AUX_CPP_14_MPL_HPP

#include <type_traits>
#include <boost/mpl/has_xxx.hpp>// tmp

namespace boost {
namespace di {

template<int N>
using int_ = std::integral_constant<int, N>;

template<bool N>
using bool_ = std::integral_constant<bool, N>;

template<typename T, typename E>
struct pair
{
    using first = T;
    using second = E;
};

template<unsigned...> struct seq{ using type = seq; };

template<class T> using Invoke = typename T::type;

template<class S1, class S2> struct concat;

template<unsigned... I1, unsigned... I2>
struct concat<seq<I1...>, seq<I2...>>
  : seq<I1..., (sizeof...(I1)+I2)...>
{ };

template<unsigned N> struct gen_seq;

template<unsigned N>
struct gen_seq
    : Invoke<concat<Invoke<gen_seq<N/2>>, Invoke<gen_seq<N - N/2>>>>{};

template<> struct gen_seq<0> : seq<>{};
template<> struct gen_seq<1> : seq<1>{};

template<typename T, typename Q>
struct ctor_traits_impl;

template<typename...>
struct type_list
{
    using type = type_list;
};

template<typename T, typename... TArgs>
struct is_constructible
{
    using type = typename std::conditional<std::is_constructible<T, TArgs...>::value, type_list<TArgs...>, type_list<>>::type;
};

template<typename T, int>
struct type_
{
    using type = T;
};

template<typename, typename T, typename>
struct gen_type;

template<typename R, typename T, unsigned... TArgs>
struct gen_type<R, T, seq<TArgs...>>
    : is_constructible<R, typename type_<T, TArgs>::type...>
{ };

template<typename R, typename T, int N>
using genn = gen_type<R, T, typename gen_seq<N>::type>;

template<typename>
struct size;

template<typename... T>
struct size<type_list<T...>>
{
    static const bool value = sizeof...(T);
};

template<typename, int I, typename... Ts>
struct longest_impl;

template<typename R, typename T, typename... Ts>
struct longest_impl<R, 0, T, Ts...>
{
    using type = R;
};

template<typename R, int I, typename T, typename... Ts>
struct longest_impl<R, I, T, Ts...>
{
    using type = typename std::conditional<(size<R>::value > size<T>::value), typename longest_impl<R, I - 1, Ts...>::type, typename longest_impl<T, I-1, Ts...>::type>::type;
};

template<typename... Ts>
using longest = longest_impl<type_list<>, sizeof...(Ts) - 1, Ts...>;

template<typename, int I, typename... Ts>
struct greatest_impl;

template<typename R, typename T, typename... Ts>
struct greatest_impl<R, 0, T, Ts...>
{
    using type = typename std::conditional<(T::second::value > R::second::value), typename T::first, typename R::first>::type;
};

template<typename R, int I, typename T, typename... Ts>
struct greatest_impl<R, I, T, Ts...>
{
    using type = typename std::conditional<(T::second::value > R::second::value), typename greatest_impl<T, I - 1, Ts...>::type, typename greatest_impl<R, I-1, Ts...>::type>::type;
};

template<typename T, typename... Ts>
struct greatest
    : greatest_impl<T, sizeof...(Ts) - 1, Ts...>
{ };

template<typename T>
struct greatest<T>
    : T::first
{ };

template<typename, int I, typename... Ts>
struct max_impl;

template<typename R, typename T, typename... Ts>
struct max_impl<R, 0, T, Ts...>
{
    using type = typename std::conditional<(R::value > T::value), R, T>::type;
};

template<typename R, int I, typename T, typename... Ts>
struct max_impl<R, I, T, Ts...>
{
    using type = typename std::conditional<(R::value > T::value), typename max_impl<R, I - 1, Ts...>::type, typename max_impl<T, I-1, Ts...>::type>::type;
};

template<typename T, typename... Ts>
using max = max_impl<T, sizeof...(Ts) - 1, Ts...>;


template<typename, typename...>
struct sum;

template<typename TMultiplicationFactor>
struct sum<TMultiplicationFactor>
	: int_<1>
{ };

template<typename TMultiplicationFactor, typename T, typename... Ts>
struct sum<TMultiplicationFactor, T, Ts...>
	: int_<TMultiplicationFactor::value * T::value * sum<TMultiplicationFactor, Ts...>::value>
{ };

template<typename>
struct is_type_list
    : std::false_type
{ };

template<typename... Ts>
struct is_type_list<type_list<Ts...>>
    : std::true_type
{ };

template <typename...> struct join;

template<>
struct join<>
{
    using type = type_list<>;
};

template <typename... TArgs> struct join<type_list<TArgs...>>
{
    using type = type_list<TArgs...>;
};

template <typename ...Args1,
          typename ...Args2>
struct join<type_list<Args1...>, type_list<Args2...>>
{
    typedef type_list<Args1..., Args2...> type;
};

template <typename ...Args1,
          typename ...Args2,
          typename ...Tail>
struct join<type_list<Args1...>, type_list<Args2...>, Tail...>
{
     typedef typename join<type_list<Args1..., Args2...>, Tail...>::type type;
};

template<bool...> struct bool_seq {
    using type = bool_seq;
};

template<bool B, bool... Ts>
struct contains_impl
    : bool_<B || contains_impl<Ts...>::value>
{ };

template<bool B>
struct contains_impl<B>
    : bool_<B>
{ };

template<typename, typename> struct contains;

template<typename T, typename... Ts>
struct contains<type_list<Ts...>, T>
    : contains_impl<std::is_same<T, Ts>::value...>
{ };

template<typename T>
struct contains<type_list<>, T>
    : bool_<false>
{ };

typedef char (&no_tag)[1];
typedef char (&yes_tag)[2];

struct void_ {};
//is_smart_ptr
//NAMED(int, "my_name")
//has table with typeid hash

}}

//TO BE REMOVED

/*
#include <boost/mpl/x11/if.hpp>
#include <boost/mpl/x11/advance.hpp>
#include <boost/mpl/x11/range_c.hpp>
#include <boost/mpl/x11/apply.hpp>
#include <boost/mpl/x11/at.hpp>
#include <boost/mpl/x11/begin_end.hpp>
#include <boost/mpl/x11/contains.hpp>
#include <boost/mpl/x11/copy.hpp>
#include <boost/mpl/x11/count.hpp>
#include <boost/mpl/x11/deref.hpp>
#include <boost/mpl/x11/equal.hpp>
#include <boost/mpl/x11/fold.hpp>
#include <boost/mpl/x11/front_back.hpp>
#include <boost/mpl/x11/if.hpp>
#include <boost/mpl/x11/is_sequence.hpp>
#include <boost/mpl/x11/iterator_range.hpp>
#include <boost/mpl/x11/joint_view.hpp>
#include <boost/mpl/x11/min_max_element.hpp>
#include <boost/mpl/x11/pair.hpp>
#include <boost/mpl/x11/size.hpp>
#include <boost/mpl/x11/logical.hpp>
#include <boost/mpl/x11/comparison.hpp>
#include <boost/mpl/x11/integral.hpp>
#include <boost/mpl/x11/transform.hpp>
#include <boost/mpl/x11/transform_view.hpp>
#include <boost/mpl/x11/push_pop.hpp>
#include <boost/mpl/x11/vector.hpp>
#include <boost/mpl/x11/void.hpp>

#include <boost/mpl/aux_/yes_no.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace mpl {

struct string_tag;
template<typename> struct c_str;

namespace x11 {
    typedef arg<0> _1;
    typedef arg<1> _2;
    typedef arg<0> _;
    typedef false_type false_;
    typedef true_type true_;

    template <typename T, T v>
    using integral_c = integral_constant<T, v>;

    using ::boost::mpl::string_tag;
    using ::boost::mpl::c_str;
    template < class T, class R >
    struct normalize;

    template < class... TTypes, class X >
    struct normalize< vector< TTypes... >, X >
       : vector< TTypes..., X >
    { };

    template < class T, class R >
    struct normalize_type_list_;

    template < class... TTypes, class X >
    struct normalize_type_list_< di::type_list< TTypes... >, X >
       : di::type_list< TTypes..., X >
    { };

    template<typename TSeq>
    struct normalize_vector
        : fold<
              TSeq
            , vector<>
            , normalize<_1, _2>
          >::type
    { };

    template<typename TSeq>
    struct normalize_type_list
        : fold<
              TSeq
            , di::type_list<>
            , normalize_type_list_<_1, _2>
          >::type
    { };

    namespace aux {
        using ::boost::mpl::aux::yes_tag;
        using ::boost::mpl::aux::no_tag;
    } // namespace aux

}}}

namespace boost {
namespace di {
namespace aux {
    namespace mpl = ::boost::mpl::x11;
}}}
*/

#endif


