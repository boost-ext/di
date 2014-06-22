//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[modules_cpp_03
//`[h6 C++ 98/03]
//<-
#include <memory>
#include <boost/typeof/typeof.hpp>
//->

#include <boost/di.hpp>

namespace di = boost::di;

struct i { virtual ~i() { } };
struct impl : i { };

struct app {
    app(std::auto_ptr<i> ap, int i) {
        assert(dynamic_cast<impl*>(ap.get()));
        assert(i == 42);
    }
};

class module1 {
    typedef di::injector<
        di::bind<i, impl>
    > injector_t;

public:
    injector_t configure() const {
        return injector_t();
    }
};

class module2 {
    typedef di::injector<
        BOOST_TYPEOF(di::bind<int>::to(int()))
    > injector_t;

public:
    explicit module2(int i)
        : i_(i)
    { }

    injector_t configure() const {
        return injector_t(
            di::bind<int>::to(i_)
        );
    }

private:
    int i_;
};

int main() {
    const int i = 42;

    BOOST_AUTO(injector, (
        di::make_injector(module1(), module2(i))
    ));

    injector.create<app>();

    return 0;
}

//`full code example: [@example/cpp_03/modules.cpp modules.cpp]
//]

