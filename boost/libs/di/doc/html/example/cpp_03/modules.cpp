//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[modules_cpp_03
//````C++98/03```
//<-
#include <memory>
#include <boost/typeof/typeof.hpp>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct interface { virtual ~interface() { } };
struct implementation : interface { };

struct app {
    app(std::auto_ptr<interface> ap, int i) {
        assert(dynamic_cast<implementation*>(ap.get()));
        assert(i == 42);
    }
};

class module1 {
    /*<<module declaration>>*/
    typedef di::injector<
        di::bind<interface, implementation>
    > injector_t;

public:
    /*<<module configuration>>*/
    injector_t configure() const {
        return injector_t();
    }
};

class module2 {
    /*<<module declaration with lazy binding>>*/
    typedef di::injector<
        BOOST_TYPEOF(di::bind<int>::to(int()))
    > injector_t;

public:
    explicit module2(int i)
        : i_(i)
    { }

    /*<<module configuration>>*/
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

    /*<<create injector and pass `module1`, `module2`>>*/
    BOOST_AUTO(injector, (
        di::make_injector(module1(), module2(i))
    ));

    /*<<create `app`>>*/
    injector.create<app>();

    return 0;
}

//]

