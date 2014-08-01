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
class scope
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(given)

    template<typename T>
    struct is_dependency
        : has_given<T>
    { };

    template<typename T>
    struct dependency
        : TDependency<
              TScope
            , T
            , T
            , detail::requires_<
                  type_traits::is_required_priority
                , type_traits::is_required_type<T>
              >
          >
    { };

    template<typename T, typename U>
    struct rebind
        : T::template rebind<U>::other
    { };

public:
    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct bind
        : aux::mpl::fold<
              BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(T)
            , aux::mpl::vector<>
            , aux::mpl::push_back<
                  aux::mpl::_1
                , aux::mpl::if_<
                      is_dependency<aux::mpl::_2>
                    , rebind<aux::mpl::_2, TScope>
                    , dependency<aux::mpl::_2>
                  >
              >
          >::type
    { };
};

} // namespace bindings
} // namespace di
} // namespace boost

#endif

