//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_SCOPE_HPP
#define BOOST_DI_CONCEPTS_SCOPE_HPP

#include "boost/di/aux_/meta.hpp"
#include "boost/di/concepts/type_traits/is_req_type.hpp"

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace di {
namespace concepts {

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
    BOOST_MPL_HAS_XXX_TRAIT_DEF(bind)

    template<typename T>
    struct is_dependency
        : has_bind<T>
    { };

	template<typename T>
	struct dependency
		: TDependency<
		  	  mpl::_1
		  	, T
		    , T
			, typename mpl::lambda<
				type_traits::is_req_type<T>
			  >::type
		  >
	{ };

    template<typename T, typename U>
    struct rebind
        : T::template rebind<U>::other
    { };

public:
    template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
    struct bind
        : mpl::fold<
              mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
            , mpl::vector0<>
            , mpl::push_back<
                  mpl::_1
                , mpl::if_<
                      is_dependency<mpl::_2>
                    , rebind<mpl::_2, TScope>
                    , rebind<dependency<mpl::_2>, TScope>
                  >
              >
          >::type
    { };
};

} // namespace concepts
} // namespace di
} // namespace boost

#endif

