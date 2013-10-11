//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_ARGUMENTS_PERMISSION_HPP
#define BOOST_DI_POLICIES_ARGUMENTS_PERMISSION_HPP

#include "boost/di/aux_/meta.hpp"
#include "boost/di/detail/binder.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/make_plain.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_rvalue_reference.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/has_xxx.hpp>

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
    template<typename T>
    struct allow
        : is_reference<T>
    { };
};

struct allow_rvalue_refs
{
    template<typename T>
    struct allow
        : is_rvalue_reference<T>
    { };
};

struct allow_ptrs
{
    template<typename T>
    struct allow
        : is_pointer<T>
    { };
};

struct allow_copies
{
    template<typename T>
    struct allow
        : mpl::and_<
               mpl::not_<is_reference<T> >
             , mpl::not_<is_pointer<T> >
             , mpl::not_<is_rvalue_reference<T> >
             , mpl::not_<has_element_type<T> >
          >
    { };
};

/**
 * @code
 * arguments_permission<>
 * struct c { c(int*); }; // error
 *
 * arguments_permission<allow_refs>
 * struct c { c(int*); }; // error
 *
 * arguments_permission<allow_ptrs>
 * struct c { c(int*); }; // ok
 *
 * arguments_permission<allow_copies>
 * struct c { c(std::vector<int*>); }; // error
 *
 * @endcode
 */
template<BOOST_DI_TYPES_DEFAULT_MPL(T)>
class arguments_permission
{
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

    BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)

    template<typename T>
    struct value_type
    {
        typedef typename T::value_type type;
    };

    template<typename TAllow, typename T>
    struct is_allowed_impl
        : TAllow::template allow<T>
    { };

    template<typename, typename, typename = void>
    struct is_allowed_nested_impl
        : mpl::true_
    { };

    template<typename TAllow, typename T>
    struct is_allowed_nested_impl<TAllow, T, typename enable_if<has_value_type<T> >::type>
        : TAllow::template allow<typename value_type<T>::type>
    { };

    template<
        typename T
      , typename TAllows = mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
    >
    struct is_allowed_nested
        : mpl::bool_<
              mpl::count_if<
                  TAllows
                , is_allowed_nested_impl<
                      mpl::_
                    , typename type_traits::remove_accessors<T>::type
                  >
              >::value != 0
          >
    { };

    template<
        typename T
      , typename TAllows = mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)>
    >
    struct is_allowed
        : mpl::bool_<
              mpl::count_if<
                  TAllows
                , mpl::and_<
                      is_allowed_impl<mpl::_, T>
                    , is_allowed_nested<T>
                  >
              >::value != 0
          >
    { };

    template<
        typename T
      , typename TBind
      , typename TCallStack =
            mpl::vector1<typename type_traits::make_plain<T>::type>
    >
    struct arguments_permission_impl
        : mpl::fold<
              ctor<typename binder<T, TCallStack, TBind>::given>
            , mpl::vector0<>
            , mpl::copy<
                  mpl::joint_view<
                      mpl::if_<
                          is_allowed<mpl::_2>
                        , mpl::_2 // ignore
                        , mpl::vector1<mpl::_2>
                      >
                    , arguments_permission_impl<
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

    template<typename T, typename TAssert>
    struct verify_impl : T
    {
        BOOST_MPL_ASSERT_MSG(
            !TAssert::value || mpl::empty<T>::value
          , ARGUMENTS_NOT_PERMITTED
          , (T)
        );
    };

public:
    typedef arguments_permission is_policy;

    template<
        typename TDeps
      , typename TGiven
      , typename TAssert = mpl::true_
      , template<typename> class TBinder = detail::binder
    >
    struct verify
        : verify_impl<
              typename arguments_permission_impl<TGiven, TBinder<TDeps> >::type
            , TAssert
          >
    { };
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

