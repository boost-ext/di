//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_HAS_CALL_HPP
#define BOOST_DI_TYPE_TRAITS_HAS_CALL_HPP

#include "boost/di/aux_/config.hpp"

#include <boost/non_type.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/aux_/yes_no.hpp>

namespace boost {
namespace di {
namespace type_traits {

namespace detail {

template<typename T>
class has_call_impl
{
    struct base_impl { void call() { } };
    struct base : T, base_impl { };

    template<typename U>
    static mpl::aux::no_tag test(
        U*
      , non_type<void (base_impl::*)(), &U::call>* = 0
    );

    template<typename>
    static mpl::aux::yes_tag test(...);

public:
    typedef has_call_impl type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test<base>(0)) == sizeof(mpl::aux::yes_tag)
    );
};

} // namespace detail

template<
    typename T
  , typename TAction
>
class has_call
{
    template<typename>
    struct void_ { };

    template<typename S, typename U>
    friend U& operator,(const U&, void_<S>);

    struct base : T
    {
        using T::call;
        mpl::aux::no_tag call(...) const;
    };

    template<typename, typename = void>
    struct base_call
        : mpl::false_
    { };

    template<typename TDummy>
    struct base_call<mpl::true_, TDummy>
        : is_same<
              BOOST_DI_FEATURE_DECLTYPE(
                 ((((base*)0)->call(*(TAction*)0)), void_<T>())
              )
            , void_<T>
          >
    { };

public :
    typedef has_call type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = base_call<
            mpl::bool_<detail::has_call_impl<T>::value>
        >::value
    );
};

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

