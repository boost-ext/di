// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXPOSED_HPP
#define BOOST_DI_SCOPES_EXPOSED_HPP

#include "boost/di/type_traits/scope_traits.hpp"
#include "boost/di/providers/nothrow_heap.hpp"

namespace boost { namespace di { namespace scopes {

class exposed {
public:
    static constexpr auto priority = false;

    template<class TExpected, class T>
    class scope {
        struct provider {
            struct ifunction {
                virtual ~ifunction() = default;
                virtual T* operator()() = 0;
            };

            template<typename TInjector>
            class function : public ifunction {
            public:
                explicit function(const TInjector& injector)
                    : injector_(injector)
                { }

                T* operator()() override {
                    //create_impl like in any_type with provider and policies
                    return injector_.template provide<T*>(providers::nothrow_heap{});
                }

            private:
                TInjector injector_;
            };

            template<typename TInjector>
            explicit provider(const TInjector& injector) noexcept
                : create_(new function<TInjector>(injector))
            { }

            T* get_ptr() const noexcept { return (*create_)(); }
            T* get() const noexcept { return (*create_)(); }

            std::shared_ptr<ifunction> create_;
        };

    public:
        template<class TInjector>
        explicit scope(const TInjector& injector) noexcept
            : provider_{injector}
        { }

        template<class TDst, class TProvider>
        decltype(auto) create(const TProvider& pr) const noexcept {
            using scope_traits = type_traits::scope_traits_t<TDst>;
            using scope = typename scope_traits::template scope<TExpected, T>;
            return scope{}.template create<TDst>(provider_);
        }

    private:
        provider provider_;
    };
};

}}} // namespace boost::di::scopes

#endif

