//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONCEPTS_DETAIL_REQUIRES_HPP
#define BOOST_DI_CONCEPTS_DETAIL_REQUIRES_HPP

#include "boost/di/aux_/config.hpp"

#include <typeinfo>
#include <vector>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/max_element.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/integral_c.hpp>

#include <boost/units/detail/utility.hpp>
namespace boost {
namespace di {
namespace concepts {
namespace detail {

struct no_name { };

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class requires
{
    template<
        typename TBind
      , typename T
      , typename TCallStack
      , typename TScope
    >
    struct apply_bind
        : TBind::template apply<
              T
            , TCallStack
            , TScope
          >::type
    { };


public:
    typedef requires type;

BOOST_MPL_HAS_XXX_TRAIT_DEF(max)
BOOST_MPL_HAS_XXX_TRAIT_DEF(nameq)

    template<typename T>
    struct namee
    {
        typedef typename T::nameq type;
    };
    template<typename T>
    struct maxx
    {
        typedef typename T::max type;
    };
    typedef typename mpl::fold<
        mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
      , mpl::vector0<>
      , mpl::if_<
            has_max<mpl::_2>
          , maxx<mpl::_2>
          , mpl::_1
        >
    >::type context;
    typedef typename  mpl::fold<
        mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
      , no_name
      , mpl::if_<
            has_nameq<mpl::_2>
          , namee<mpl::_2>
          , mpl::_1
        >
    >::type name;

    template<typename T, typename TCallStack, typename TScope>
    struct apply
        : mpl::second<
              typename mpl::fold<
                  mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                , mpl::pair<mpl::integral_c<long, 1>, mpl::integral_c<long, 1> >
                , mpl::pair<
                      mpl::times<mpl::first<mpl::_1>, mpl::integral_c<long, 100> >
                    , mpl::times<mpl::first<mpl::_1>, mpl::second<mpl::_1>, apply_bind<mpl::_2, T, TCallStack, TScope> >
                  >
              >::type
          >
    { };

    int operator()(const std::type_info* t, const std::type_info* name, const std::vector<const std::type_info*>& call_stack, int priority) const {
        int result =  (1 * (priority + 1) * (100 * T1()(t, name, call_stack))) * t2(T2(), t, name, call_stack) * t3(T3(), t, name, call_stack);
        return result;
    }

    int t2(const mpl::na&, const std::type_info*, const std::type_info*, const std::vector<const std::type_info*>&) const {
        return 1;
    }

    template<typename T>
    int t2(const T&, const std::type_info* t, const std::type_info* n, const std::vector<const std::type_info*>& stack) const {
        return 10000 * T()(t, n, stack);
    }

    int t3(const mpl::na&, const std::type_info*, const std::type_info*, const std::vector<const std::type_info*>&) const {
        return 1;
    }

    template<typename T>
    int t3(const T&, const std::type_info* t, const std::type_info* n, const std::vector<const std::type_info*>& stack) const {
        return 1000000 * T()(t, n, stack);
    }
};

} // namespace detail
} // namespace concepts
} // namespace di
} // namespace boost

#endif

