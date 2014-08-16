//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_TYPE_TRAITS_HAS_CALL_HPP
#define BOOST_DI_TYPE_TRAITS_HAS_CALL_HPP

#include "boost/di/aux_/mpl.hpp"

namespace boost {
namespace di {
namespace type_traits {

template<typename, typename = void>
class has_call;

template<typename T>
class has_call<T, void> {
    struct base_impl { void call() { } };
    struct base : T, base_impl { base() { } };

    template<typename U>
    static no_tag test(
        U*
      , non_type<void (base_impl::*)(), &U::call>* = 0
    );

    template<typename>
    static yes_tag test(...);

public:
    static constexpr bool value = sizeof(test<base>(0)) == sizeof(yes_tag);
};

template<typename T, typename TAction>
class has_call {
    template<typename>
    struct void_ { };

    template<typename S, typename U>
    friend U& operator,(const U&, void_<S>);

    struct base : T
    {
        using T::call;
        no_tag call(...) const;
    };

    template<typename, typename = void>
    struct base_call
        : std::false_type
    { };

    template<typename TDummy>
    struct base_call<std::true_type, TDummy>
        : std::is_same<
              decltype(
                 ((((base*)0)->call(*(TAction*)0)), void_<T>())
              )
            , void_<T>
          >
    { };

public :
    static constexpr bool value = base_call<bool_<has_call<T>::value>>::value;
};

} // namespace type_traits
} // namespace di
} // namespace boost

#endif

