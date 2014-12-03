// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXPOSED_HPP
#define BOOST_DI_SCOPES_EXPOSED_HPP

#include "boost/di/type_traits/scope_traits.hpp"
#include "boost/di/type_traits/expr_traits.hpp"
#include "boost/di/providers/nothrow_reduce_heap_usage.hpp"

namespace boost { namespace di { namespace scopes {

template<class T>
struct ifunction {
    virtual ~ifunction() = default;
    virtual T* operator()(type_traits::ptr) const noexcept = 0;
    virtual T  operator()(type_traits::value) const noexcept = 0;
};

template<class T, class TInjector>
class function : public ifunction<T> {
public:
    explicit function(const TInjector& injector)
        : injector_(injector)
    { }

    T* operator()(type_traits::ptr) const noexcept override {
        return injector_.template
            create_impl<T*, T>(providers::nothrow_reduce_heap_usage{}, core::pool<>{});
    }

    T operator()(type_traits::value) const noexcept override {
        return injector_.template
            create_impl<T, T>(providers::nothrow_reduce_heap_usage{}, core::pool<>{});
    }

private:
    TInjector injector_;
};

class exposed {
public:
    static constexpr auto priority = false;

    template<class TExpected, class T>
    class scope {
        struct provider {
            template<typename TInjector>
            explicit provider(const TInjector& injector) noexcept
                : create_(new function<T, TInjector>(injector))
            { }

            template<typename TExpr = type_traits::ptr>
            decltype(auto) get(const TExpr& expr = TExpr{}) const noexcept {
                return (*create_)(expr);
            }

            std::shared_ptr<ifunction<T>> create_;
        };

    public:
        template<class TInjector>
        explicit scope(const TInjector& injector) noexcept
            : provider_{injector}
        { }

        template<class TDst, class TProvider>
        decltype(auto) create(const TProvider&) const noexcept {
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

