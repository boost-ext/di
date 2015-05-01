//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <memory>
#include <cassert>

namespace di = boost::di;

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct impl1 : i1 { void dummy1() override { } };

struct custom_scope {
    template<class TExpected, class>
    struct scope {
        template<class>
        using is_referable = std::false_type;

        template<class T, class TProvider>
        auto try_create(const TProvider& provider) -> decltype(std::shared_ptr<TExpected>{provider.try_get()});

        template<class T, class TProvider>
        auto create(const TProvider& provider) {
            return std::shared_ptr<TExpected>{provider.get()};
        }
    };
};

int main() {
    auto injector = di::make_injector(
        di::bind<i1, impl1>.in(custom_scope{})
    );

    assert(injector.create<std::shared_ptr<i1>>()
           !=
           injector.create<std::shared_ptr<i1>>()
    );
}

