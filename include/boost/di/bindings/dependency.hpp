//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_BINDINGS_DEPENDENCY_HPP
#define BOOST_DI_BINDINGS_DEPENDENCY_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/aux_/ref.hpp"
#include "boost/di/wrappers/shared.hpp"
#include "boost/di/wrappers/reference.hpp"
#include "boost/di/wrappers/value.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/type_traits/function_traits.hpp"
#include "boost/di/bindings/detail/requires.hpp"
#include "boost/di/bindings/type_traits/is_required_priority.hpp"
#include "boost/di/bindings/type_traits/is_required_type.hpp"

namespace boost {
namespace di {
namespace bindings {

BOOST_DI_HAS_MEMBER_TYPE(result_type);

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TBind =
        detail::requires_<
            bindings::type_traits::is_required_priority
          , bindings::type_traits::is_required_type<TExpected>
        >
>
class dependency : public TScope::template scope<TExpected>
{
    using scope_type = typename TScope::template scope<TExpected>;
    using ref_type = scopes::external<wrappers::reference>;
    using shared_type = scopes::external<wrappers::shared>;
    using value_type = scopes::external<wrappers::value>;

    template<typename, typename = void>
    struct get_wrapper_impl {
        using type = value_type;
    };

    template<typename T>
    struct get_wrapper_impl<aux::shared_ptr<T>> {
        using type = shared_type;
    };

    template<typename T>
    struct get_wrapper_impl<T, typename std::enable_if<aux::is_reference_wrapper<T>::value>::type> {
        using type = ref_type;
    };

    template<
        typename T
      , typename = void
      , typename = void
      , typename = void
    >
    struct get_wrapper {
        using type = T;
    };

    template<typename T>
    struct get_wrapper<T, typename std::enable_if<has_result_type<T>::value>::type>
        : get_wrapper_impl<typename T::result_type>
    { };

    template<typename T>
    struct get_wrapper<T, typename std::enable_if<has_call_operator<T>::value>::type
                        , typename std::enable_if<!has_result_type<T>::value>::type
                        , typename std::enable_if<!aux::is_reference_wrapper<T>::value>::type>
        : get_wrapper_impl<
              typename di::type_traits::function_traits<
                  decltype(&T::operator())
              >::result_type
          >
    { };

public:
    using type = dependency;
    using scope = TScope;
    using expected = TExpected;
    using given = TGiven;
    using bind = TBind;

    template<typename T>
    struct rebind
    {
        using other = dependency<
            typename std::conditional<
                std::is_same<scope, scopes::deduce>::value
              , T
              , TScope
            >::type
          , TExpected
          , TGiven
          , TBind
        >;
    };

    dependency() { }

    template<typename T>
    explicit dependency(const T& object)
        : scope_type(object)
    { }

    template<typename T>
    static dependency<value_type, expected, T, TBind>
    to(const T& object, typename std::enable_if<!aux::is_reference_wrapper<T>::value>::type* = 0
                      , typename std::enable_if<!has_call_operator<T>::value>::type* = 0) {
        return dependency<value_type, expected, T, TBind>(object);
    }

    template<typename T>
    static dependency<ref_type, typename aux::unwrap_reference<T>::type, T, TBind>
    to(const T& object, typename std::enable_if<aux::is_reference_wrapper<T>::value>::type* = 0) {
        return dependency<ref_type, typename aux::unwrap_reference<T>::type, T, TBind>(object);
    }

    template<typename T>
    static dependency<typename get_wrapper<T>::type, expected, T, TBind>
    to(const T& object, typename std::enable_if<!aux::is_reference_wrapper<T>::value>::type* = 0
                      , typename std::enable_if<has_call_operator<T>::value>::type* = 0) {
        return dependency<typename get_wrapper<T>::type, expected, T, TBind>(object);
    }

    template<typename T>
    static dependency<shared_type, expected, T, TBind>
    to(const aux::shared_ptr<T>& object) {
        return dependency<shared_type, expected, T, TBind>(object);
    }
};

} // namespace bindings
} // namespace di
} // namespace boost

#endif

