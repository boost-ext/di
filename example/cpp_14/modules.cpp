//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[modules_cpp_14
//`[h6 C++ 14]
//<-
#include <memory>
#include <utility>
//->

#include <boost/di.hpp>

namespace di = boost::di;

struct i { virtual ~i() { } };
struct impl : i { };

struct app {
    app(std::unique_ptr<i> up, int i) {
        assert(dynamic_cast<impl*>(up.get()));
        assert(i == 42);
    }
};

class module1 {
public:
    auto configure() const {
        return di::make_injector(
            di::bind<i, impl>()
        );
    }
};

class module2 {
public:
    explicit module2(int i)
        : i_(i)
    { }

    auto configure() const {
        return di::make_injector(
            di::bind<int>::to(i_)
        );
    }

private:
    int i_ = 0;
};

int main() {
    const int i = 42;

    auto injector = di::make_injector(module1(), module2(i));
    injector.create<app>();

    return 0;
}

//`full code example: [@example/cpp_14/modules.cpp modules.cpp]
//]

