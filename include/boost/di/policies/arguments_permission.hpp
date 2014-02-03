//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_ARGUMENTS_PERMISSION_HPP
#define BOOST_DI_POLICIES_ARGUMENTS_PERMISSION_HPP

#include "boost/di/aux_/meta.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_rvalue_reference.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace boost {
namespace di {
namespace policies {

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(value_type)

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

struct allow_const_refs
{
    template<typename T>
    struct allow
        : mpl::and_<
              is_const<typename remove_reference<T>::type>
            , is_reference<T>
          >
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

namespace detail {

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

} // namespace detail

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
    typedef mpl::vector<BOOST_DI_TYPES_PASS_MPL(T)> allows_type;

    template<typename T>
    struct is_allowed_nested
        : mpl::bool_<
              mpl::count_if<
                  allows_type
                , detail::is_allowed_nested_impl<
                      mpl::_
                    , typename type_traits::remove_accessors<T>::type
                  >
              >::value != 0
          >
    { };

    template<typename T>
    struct is_allowed
        : mpl::bool_<
              mpl::count_if<
                  allows_type
                , mpl::and_<
                      detail::is_allowed_impl<mpl::_, T>
                    , is_allowed_nested<T>
                  >
              >::value != 0
          >
    { };

public:
    template<
        typename TDeps
      , typename T
    >
    static void assert_policy() {
        BOOST_DI_ASSERT_MSG(
            is_allowed<T>::value
          , ARGUMENT_NOT_PERMITTED
          , T
        );
    }
};

} // namespace policies
} // namespace di
} // namespace boost

#endif

