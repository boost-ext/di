#ifndef BOOST_DI_CONCEPTS_IS_CREATABLE_HPP
#define BOOST_DI_CONCEPTS_IS_CREATABLE_HPP

#include <type_traits>
#include "boost/di/aux_/utility.hpp"
#include "boost/di/core/binder.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost { namespace di { namespace concepts {

template<class, class>
struct is_creatable_impl;

template<class T, class... TArgs>
struct is_creatable_impl<T, aux::pair<type_traits::uniform, aux::type_list<TArgs...>>> {
    using type = aux::is_braces_constructible<T, TArgs...>;
};

template<class T, class... TArgs>
struct is_creatable_impl<T, aux::pair<type_traits::direct, aux::type_list<TArgs...>>> {
    using type = std::is_constructible<T, TArgs...>;
};

template<class TParent, class TDeps>
struct any {
    template<class T>
    using any_ = any<T, TDeps>;

    template<class T
          , class U = aux::decay_t<T>
          , class G = typename std::remove_reference_t<decltype(core::binder::resolve<U>((TDeps*)nullptr))>::given
          , class = std::enable_if_t<!(std::is_same<U, TParent>{} || std::is_base_of<TParent, U>{})>
          , class = std::enable_if_t<
                typename is_creatable_impl<
                    G, typename type_traits::ctor_traits<G, any_>::type
                >::type{}
            >
    > operator T();
};

template<class, class, class, class = void>
struct is_creatable : std::false_type { };

template<class T, class TParent, class TDeps>
struct is_creatable<T, TParent, TDeps, aux::void_t<decltype(static_cast<T>(any<TParent, TDeps>{}))>>
    : std::true_type
{ };

}}} // boost::di::concepts

#endif


