#ifndef BOOST_DI_POLICIES_ARGUMENTS_PERMISSION_HPP
#define BOOST_DI_POLICIES_ARGUMENTS_PERMISSION_HPP

#include "boost/di_preprocessed/aux_/common.hpp"
#include "boost/di_preprocessed/type_traits/remove_accessors.hpp"
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

}
template< typename T0 = ::boost::mpl::na , typename T1 = ::boost::mpl::na , typename T2 = ::boost::mpl::na , typename T3 = ::boost::mpl::na , typename T4 = ::boost::mpl::na , typename T5 = ::boost::mpl::na , typename T6 = ::boost::mpl::na , typename T7 = ::boost::mpl::na , typename T8 = ::boost::mpl::na , typename T9 = ::boost::mpl::na , typename T10 = ::boost::mpl::na , typename T11 = ::boost::mpl::na , typename T12 = ::boost::mpl::na , typename T13 = ::boost::mpl::na , typename T14 = ::boost::mpl::na , typename T15 = ::boost::mpl::na , typename T16 = ::boost::mpl::na , typename T17 = ::boost::mpl::na , typename T18 = ::boost::mpl::na , typename T19 = ::boost::mpl::na >
class arguments_permission
{
    typedef mpl::vector< T0 , T1 , T2 , T3 , T4 , T5 , T6 , T7 , T8 , T9 , T10 , T11 , T12 , T13 , T14 , T15 , T16 , T17 , T18 , T19> allows_type;

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
        BOOST_MPL_ASSERT_MSG(is_allowed<typename T::type>::value, ARGUMENT_NOT_PERMITTED, (typename T::type))



         ;
    }
};

}
}
}

#endif

