//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[modules_cpp_14
//````C++14```
//<-
#include <memory>
#include <cassert>
#include <utility>
//->

#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface { virtual ~interface() = default; };
struct implementation : interface { };
//->

struct app {
    app(std::unique_ptr<interface> up, int i) {
        assert(dynamic_cast<implementation*>(up.get()));
        assert(i == 42);
    }
};

class module1 {
public:
    /*<<module configuration>>*/
    auto configure() const {
        return di::make_injector(
            di::bind<interface, implementation>
        );
    }
};

class module2 {
public:
    explicit module2(int i)
        : i_(i)
    { }

    /*<<module configuration>>*/
    auto configure() const {
        return di::make_injector(
            di::bind<int>.to(i_)
        );
    }

private:
    int i_ = 0;
};

int main() {
    const int i = 42;

    /*<<create injector and pass `module1`, `module2`>>*/
    auto injector = di::make_injector(module1{}, module2{i});

    /*<<create `app`>>*/
    injector.create<app>();

    return 0;
}

//]

