//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>

namespace di = boost::di;

struct i { virtual ~i() noexcept = default; virtual int dummy() = 0; };
struct impl : i { impl(int i) : i_(i) { }; int dummy() override { return i_; }; int i_ = 0; };

struct module {
    di::injector<i> configure() const noexcept {
        return  di::make_injector(
            di::bind<int>.to(42)
          , di::bind<i, impl>
        );
    };
};

int main() {
    auto injector = di::make_injector(module{});
    auto* ptr =  injector.create<i*>();
    return ptr->dummy();
}

