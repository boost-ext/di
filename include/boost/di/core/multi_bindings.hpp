//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_MULTI_BINDINGS_HPP
#define BOOST_DI_CORE_MULTI_BINDINGS_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace core {

template<class T, class U>
struct t_traits {
    using type = U;
    using type_ = U;
};

template<class T, class U>
struct t_traits<T, named<U>> {
    using type = detail::named_type<U, T>;
    using type_ = T;
};

template<class T, class U>
struct t_traits<std::shared_ptr<T>, U> {
    using type = std::shared_ptr<U>;
    using type_ = std::shared_ptr<U>;
};

template<class T, class U>
struct t_traits<std::shared_ptr<T>, named<U>> {
    using type = detail::named_type<U, std::shared_ptr<T>>;
    using type_ = std::shared_ptr<T>;
};

template<class T, class D, class U>
struct t_traits<std::unique_ptr<T, D>, U> {
    using type = std::unique_ptr<U, D>;
    using type_ = std::unique_ptr<U, D>;
};

template<class T, class D, class U>
struct t_traits<std::unique_ptr<T, D>, named<U>> {
    using type = di::detail::named_type<U, std::unique_ptr<T, D>>;
    using type_ = std::unique_ptr<T, D>;
};

template<class T, class U>
using t_traits_t = typename t_traits<T, U>::type;

template<class T, class U>
using t_traits_t_ = typename t_traits<T, U>::type_;

template<class T>
struct get {
    using type = T;
};

template<class T>
struct get<std::shared_ptr<T>> {
    using type = typename get<T>::type;
};

template<class T, class TAllocator>
struct get<std::vector<T, TAllocator>> {
    using type = T;
};

/*template<class T, class TAllocator>*/
//struct get<std::list<T, TAllocator>> {
    //using type = T;
//};

template<class TKey, class TCompare, class TAllocator>
struct get<std::set<TKey, TCompare, TAllocator>> {
    using type = TKey;
};

template<class T>
struct get_ {
    using type = T;
};

template<class T>
struct get_<std::shared_ptr<T>> {
    using type = T;
};

template<class TScope, class TExpected, class TGiven, class... Ts>
class multi_bindings {
    template<class TInjector, class TArray, class T>
    struct provider {
        template<class TMemory = type_traits::heap>
        auto get(const TMemory& memory = {}) const {
            return TInjector::config::provider(injector_).template
                get<typename get_<T>::type, typename get_<T>::type>(
                    type_traits::direct{}
                  , memory
                  , std::move_iterator<TArray*>(array_)
                  , std::move_iterator<TArray*>(array_ + sizeof...(Ts))
                );
        }

        provider(const TInjector& injector, TArray* array)
            : injector_(injector), array_(array)
        { }

        const TInjector& injector_;
        TArray* array_ = nullptr;
    };

    template<class T, class TInjector>
    struct wrapper {
        operator T() const {
            using TArray = typename get<T>::type;
            TArray array[sizeof...(Ts)] = {
                static_cast<t_traits_t_<TArray, Ts>>(
                    static_cast<const di::core::injector__<TInjector>&>(injector_).
                        create_successful_impl(di::aux::type<t_traits_t<TArray, Ts>>{})
                )...
            };

            return scope_.template create<T>(
                provider<TInjector, TArray, T>(injector_, array)
            );
        }

        const TInjector& injector_;
        typename TScope::template scope<TExpected, typename get_<T>::type>& scope_;
    };

public:
    template<class TInjector, class TArg>
    auto operator()(const TInjector& injector, const TArg&) {
        typename TScope::template scope<TExpected, typename get_<di::aux::remove_specifiers_t<typename TArg::type>>::type> scope;
        return wrapper<di::aux::remove_specifiers_t<typename TArg::type>, TInjector>{injector, scope};
    }
};

}}}} // boost::di::v1::core

#endif

