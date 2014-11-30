// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXPOSED_HPP
#define BOOST_DI_SCOPES_EXPOSED_HPP

#include "boost/di/type_traits/scope_traits.hpp"

namespace boost {
namespace di {
namespace scopes {

class exposed {
public:
    static constexpr auto priority = false;

    template<class T>
    struct provider {
        provider() noexcept {}

        template<typename TProvider>
        explicit provider(const TProvider& provider) noexcept
            : f([=]{ return provider.get(); })
        { }

        T* get_ptr() const noexcept { return f(); }
        T* get() const noexcept { return f(); }

        std::function<T*()> f;
    };

    template<class TExpected, class T>
    class scope {
    public:
        scope() noexcept { }

        template<class TProvider>
        explicit scope(const TProvider& provider) noexcept
            : provider_{provider}
        { }

        template<class TDst, class TProvider>
        decltype(auto) create(const TProvider&) const noexcept {
            using scope = typename type_traits::scope_traits_t<TDst>::template scope<TExpected, T>;
            return scope{}.template create<TDst>(provider_);
        }

    private:
        provider<T> provider_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

