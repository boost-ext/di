//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_POLICIES_PERMISSIONS_HPP
#define BOOST_DI_POLICIES_PERMISSIONS_HPP

#include "boost/di/aux_/config.hpp"

namespace boost {
namespace di {
namespace policies {

BOOST_DI_HAS_MEMBER_TYPE(element_type);
BOOST_DI_HAS_MEMBER_TYPE(value_type);

struct allow_smart_ptrs {
    template<typename T>
    struct allow
        : has_element_type<typename type_traits::remove_accessors<T>::type>
    { };
};

struct allow_refs {
    template<typename T>
    struct allow
        : aux::mpl::and_<
              aux::mpl::not_<is_const<typename remove_reference<T>::type>>
            , is_reference<T>
            , aux::mpl::not_<
                  has_element_type<typename type_traits::remove_accessors<T>::type>
              >
          >
    { };
};

struct allow_const_refs {
    template<typename T>
    struct allow
        : aux::mpl::and_<
              is_const<typename remove_reference<T>::type>
            , is_reference<T>
            , aux::mpl::not_<
                  has_element_type<typename type_traits::remove_accessors<T>::type>
              >
          >
    { };
};

struct allow_rvalue_refs {
    template<typename T>
    struct allow
        : is_rvalue_reference<T>
    { };
};

struct allow_ptrs {
    template<typename T>
    struct allow
        : is_pointer<T>
    { };
};

struct allow_copies {
    template<typename T>
    struct allow
        : aux::mpl::and_<
               aux::mpl::not_<is_reference<T>>
             , aux::mpl::not_<is_pointer<T>>
             , aux::mpl::not_<is_rvalue_reference<T>>
             , aux::mpl::not_<has_element_type<typename type_traits::remove_accessors<T>::type>>
          >
    { };
};

template<typename TValueType>
struct allow_type {
    template<typename T>
    struct allow
        : is_same<T, TValueType>
    { };
};

template<typename TExpr>
struct allow_type_expr {
    template<typename T>
    struct allow
        : aux::mpl::apply<TExpr, T>::type
    { };
};

template<typename TScope>
struct allow_scope {
    template<typename T>
    struct allow
        : is_same<T, TScope>
    { };
};

template<>
struct allow_scope<scopes::deduce>; // disabled

template<BOOST_DI_TYPES_DEFAULT_MPL(TArgs)>
class creation_permission
{
    typedef BOOST_DI_MPL_VECTOR_TYPES_PASS_MPL(TArgs) permitted_types;

    struct not_resolved
    {
        typedef not_resolved type;

        template<typename>
        struct rebind
        {
            typedef not_resolved other;
        };
    };

    template<typename TAllow, typename T>
    struct is_type_permitted_impl
        : TAllow::template allow<T>
    { };

    template<typename T>
    struct is_type_permitted
        : aux::mpl::or_<
              aux::mpl::not_<
                  is_same<
                      typename T::binder::template resolve<
                          typename T::type
                        , aux::mpl::vector<>
                        , not_resolved
                      >::type
                    , not_resolved
                  >
              >
            , aux::mpl::bool_<
                  aux::mpl::count_if<
                      permitted_types
                    , is_type_permitted_impl<aux::mpl::_, typename T::type>
                  >::value != 0
              >
          >
    { };

public:
    template<typename T>
    void assert_policy() const {
       BOOST_DI_ASSERT_MSG(
            is_type_permitted<T>::value
          , TYPE_HAS_TO_BE_EXPLICITLY_BINDED
          , typename T::type
        );
    }
};

} // namespace policies
} // namespace di
} // namespace boost

#endif


