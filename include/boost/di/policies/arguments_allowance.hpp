//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_ARGUMENTS_ALLOWANCE_HPP
#define BOOST_DI_POLICIES_ARGUMENTS_ALLOWANCE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/has_xxx.hpp>

#include "boost/di/detail/meta.hpp"
#include "boost/di/detail/binder.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/make_plain.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)

struct allow_smart_ptrs
{
    template<typename, typename = void>
    struct allow
        : mpl::false_
    { };

    template<typename T>
    struct allow<T, typename enable_if<has_element_type<T> >::type>
        : mpl::true_
    { };
};

struct allow_refs
{
    template<typename>
    struct allow
        : mpl::false_
    { };

    template<typename T>
    struct allow<T&>
        : mpl::true_
    { };
};

struct allow_const_refs { };
struct allow_rvalue_refs { };
struct allow_ptrs { };
struct allow_const_ptrs { };
struct allow_copies { };

/**
 * @code
 * struct c { c(int*); };
 * @endcode
 *
 * @code
 * singleton<int>
 * struct c { c(int&); }; // not ok
 *
 * singleton<int>
 * struct c { c(int&); }; // ok
 * struct d { d(shared_ptr<int>); }; // ok
 * @endcode
 */

template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class arguments_allowance
{
public:
    typedef arguments_allowance is_policy;

    template<typename T>
    struct ctor
        : type_traits::ctor_traits<
              typename type_traits::make_plain<T>::type
          >::type
    { };

    template<
        typename T
      , typename TCallStack
      , typename TBind
    >
    struct binder
        : TBind::template get_dependency<T, TCallStack>::type
    { };

    template<typename TAllow, typename T>
    struct is_allowed_impl
        : mpl::true_
    { };

    template<typename T>
    struct is_allowed
        : mpl::bool_<
              (mpl::count_if<
                  mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
                , is_allowed_impl<mpl::_, T>
              >::value) != 0
          >
    { };

    template<
        typename T
      , typename TBind
      , typename TCallStack =
            mpl::vector1<typename type_traits::make_plain<T>::type>
    >
    struct arguments
        : mpl::fold<
              ctor<typename binder<T, TCallStack, TBind>::given>
            , mpl::vector0<>
            , mpl::copy<
                  mpl::joint_view<
                      mpl::if_<
                          is_allowed<mpl::_2>
                        , mpl::vector1<mpl::_2>
                        , mpl::_2 // ignore
                      >
                    , arguments<
                          mpl::_2
                        , TBind
                        , mpl::push_back<
                              TCallStack
                            , type_traits::make_plain<mpl::_2>
                          >
                      >
                  >
                , mpl::back_inserter<mpl::_1>
              >
          >
    { };

    template<
        typename TDeps
      , typename TGiven
      , bool Assert = true
      , template<typename> class TBinder = detail::binder
    >
    struct verify
        : arguments<TGiven, TBinder<TDeps> >::type
    { };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

