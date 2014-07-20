//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[binding_cpp_11
//````C++11```
//<-
#include <cassert>
#include <memory>
#include <boost/function.hpp>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface { virtual ~interface() { } };
struct implementation : interface { };
struct some_name { };
//->

struct service {
    service(const std::shared_ptr<interface>& sp, double d, std::unique_ptr<int> up, boost::function<int()> f)
        : sp(sp)
    {
        assert(dynamic_cast<implementation*>(sp.get()));
        assert(d == 0.0); // default zero initialization
        assert(*up == 42);
        assert(f() == 87);
    }

    std::shared_ptr<interface> sp;
};

struct app {
    app(service copy
        , boost::shared_ptr<interface> sp
        , int i
        , di::named<const std::string&, some_name> s
        , float& f)
        : str(s), f(f)
    {
        assert(dynamic_cast<implementation*>(sp.get()));
        assert(copy.sp.get() == sp.get());
        assert(i == 42);
        assert(str == "some_name");
    }

    app& operator=(const app&);

    std::string str;
    float& f;
};

int main() {
    float f = 0.f;

    /*<<create injector with configuration>>*/
    auto injector = di::make_injector(
        di::bind<interface, implementation>()
      , di::bind_int<42>()
      , di::bind<std::string>::named<some_name>::to("some_name")
      , di::bind<float>::to(boost::ref(f))
      , di::bind<boost::function<int()>>::to([]{return 87;})
    );

    /*<<create `service_app`>>*/
    auto service_app = injector.create<app>();

    /*<<verify parameter `f` affection by `service_app`>>*/
    service_app.f = 42.f;
    assert(f == 42.f);

    return 0;
}

//]

