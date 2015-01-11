//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[modules
//<-
#include <memory>
#include <cassert>
#include <utility>
//->

#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface { virtual ~interface() = default; virtual void dummy() = 0;};
struct implementation1 : interface { void dummy() override { } };
struct implementation2 : interface { void dummy() override { } };
//->

struct data {
    std::shared_ptr<interface> sp;
};

struct app {
    app(std::unique_ptr<interface> up, int i, const data& data) {
        assert(dynamic_cast<implementation1*>(up.get()));
        assert(dynamic_cast<implementation2*>(data.sp.get()));
        assert(i == 42);
    }
};

class module1 {
public:
    /*<<module configuration>>*/
    auto configure() const {
        return di::make_injector(
            di::bind<interface, implementation1>
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

class exposed_module {
public:
    /*<<module configuration with exposed `data`>>*/
    di::injector<data> configure() const {
        return di::make_injector(
            di::bind<interface, implementation2>
        );
    }
};

int main() {
    const auto i = 42;

    /*<<create injector and pass `module1`, `module2` and `exposed_module`>>*/
    auto injector = di::make_injector(
        module1{}
      , module2{i}
      , exposed_module{}
    );

    /*<<create `app`>>*/
    injector.create<app>();

    return 0;
}

//]

