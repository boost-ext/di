//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_MULTIPLE_HPP
#define BOOST_DI_SCOPES_MULTIPLE_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/type_traits/memory_traits.hpp"
#include "boost/di/type_traits/rebind_traits.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace scopes {

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

template<class TScope, class... Ts>
class multiple {
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

    template<class T, class TScope_, class TInjector>
    struct wrapper {
        operator T() const {
            using TArray = typename get<T>::type;
            TArray array[sizeof...(Ts)] = {
                static_cast<type_traits::t_traits_t_<TArray, Ts>>(
                    static_cast<const di::core::injector__<TInjector>&>(injector_).
                        create_successful_impl(di::aux::type<type_traits::t_traits_t<TArray, Ts>>{})
                )...
            };

            return scope_.template create<T>(
                provider<TInjector, TArray, T>(injector_, array)
            );
        }

        TScope_& scope_;
        const TInjector& injector_;
    };

public:
    template<class TExpected, class TGiven>
    class scope {
    public:
        template<class>
        using is_referable = std::false_type;

        template<class T, class TProvider>
        static T try_create(const TProvider&);

        template<class T, class TProvider>
        auto create(const TProvider& provider) const {
            using type = di::aux::remove_specifiers_t<T>;
            using scope_t = typename TScope::template scope<TExpected, typename get_<type>::type>;
            scope_t scope;
            return wrapper<type
                         , scope_t
                         , di::aux::remove_specifiers_t<decltype(provider.injector_)>
                   >{scope, provider.injector_};
        }
    };
};

}}}} // boost::di::v1::scopes

#endif

