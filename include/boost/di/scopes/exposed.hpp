//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXPOSED_HPP
#define BOOST_DI_SCOPES_EXPOSED_HPP

#include "boost/di/scopes/deduce.hpp"

namespace boost { namespace di { inline namespace v1 { namespace scopes {

template<class TScope = scopes::deduce>
class exposed {
public:
    template<class TExpected, class TGiven>
    class scope {
        #if defined(BOOST_DI_GCC) || defined(BOOST_DI_MSVC)
            using type = std::conditional_t<
                std::is_copy_constructible<TExpected>::value
              , TExpected
              , TExpected*
            >;
        #else
            using type = TExpected;
        #endif

        struct iprovider {
            TExpected* (*heap)(const iprovider*);
            type (*stack)(const iprovider*);

            auto get(const type_traits::heap& = {}) const noexcept {
                return ((iprovider*)(this))->heap(this);
            }

            auto get(const type_traits::stack&) const noexcept {
                return ((iprovider*)(this))->stack(this);
            }
        };

        template<class TInjector>
        struct provider_impl {
            TExpected* (*heap)(const provider_impl*);
            type (*stack)(const provider_impl*);

            template<class T>
            static T create(const provider_impl* object) noexcept {
                return object->injector.create_impl(aux::type<T>{});
            }

            explicit provider_impl(const TInjector& injector) noexcept
                : provider_impl(injector
                              , std::integral_constant<bool, TInjector::template is_creatable<TExpected*>::value>{}
                              , std::integral_constant<bool, TInjector::template is_creatable<TExpected>::value>{}
                  )
            { }

            provider_impl(const TInjector& injector, const std::true_type&, const std::true_type&) noexcept
                : heap(&provider_impl::template create<TExpected*>)
                , stack(&provider_impl::template create<type>)
                , injector(injector)
            { }

            provider_impl(const TInjector& injector, const std::false_type&, const std::true_type&) noexcept
                : stack(&provider_impl::template create<type>)
                , injector(injector)
            { }

            provider_impl(const TInjector& injector, const std::true_type&, const std::false_type&) noexcept
                : heap(&provider_impl::template create<TExpected*>)
                , injector(injector)
            { }

            provider_impl(const TInjector& injector, const std::false_type&, const std::false_type&)
                : heap(&provider_impl::template create<TExpected*>) // for creatable error
                , injector(injector)
            { }

            TInjector injector;
        };

    public:
        template<class>
        using is_referable = std::false_type;

        template<class TInjector>
        explicit scope(const TInjector& injector) noexcept {
            static auto provider = provider_impl<TInjector>{injector};
            provider.injector = injector;
            provider_ = (iprovider*)&provider;
        }

        template<class T, class TProvider>
        T try_create(const TProvider&);

        template<class T>
        void try_create(...);

        template<class T, class TProvider>
        auto create(const TProvider&) {
            return scope_.template create<T>(*provider_);
        }

    private:
        iprovider* provider_ = nullptr;
        typename TScope::template scope<TExpected, TGiven> scope_;
    };
};

}}}} // boost::di::v1::scopes

#endif

