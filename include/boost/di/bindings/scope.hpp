//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_SCOPE_HPP
#define BOOST_DI_BINDINGS_SCOPE_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/bindings/detail/requires.hpp"
#include "boost/di/bindings/type_traits/is_required_type.hpp"
#include "boost/di/bindings/type_traits/is_required_priority.hpp"

namespace boost {
namespace di {
namespace bindings {

template<
    typename TScope
  , template<
        typename
      , typename
      , typename
      , typename
    > class TDependency
>
class scope {
    BOOST_MPL_HAS_XXX_TRAIT_DEF(given)

    template<typename T>
    using is_dependency = has_given<T>;

    template<typename T>
    using dependency = TDependency<
          TScope
        , T
        , T
        , detail::requires_<
              type_traits::is_required_priority
            , type_traits::is_required_type<T>
          >
      >;

    template<typename T, typename U>
    using rebind = typename T::template rebind<U>::other;

    template<typename T, typename = void>
    struct bind_impl
        : dependency<T>
    { };

    template<typename T>
    struct bind_impl<T, typename std::enable_if<is_dependency<T>::value>::type>
        : rebind<T, TScope>
    { };

public:
    template<typename... Ts>
    using bind = type_list<typename bind_impl<Ts>::type...>;
};

} // namespace bindings
} // namespace di
} // namespace boost

#endif

