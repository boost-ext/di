//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[binding
//<-
#include <cassert>
#include <memory>
#include <functional>
#include <boost/function.hpp>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface1 { virtual ~interface1() { } };
struct interface2 { virtual ~interface2() { } };
struct implementation1 : interface1 { };
struct implementation2 : interface2 { };
struct some_name { };
struct int_name { };
//->

struct service {
    service(const std::shared_ptr<interface1>& sp
          , bool b
          , int i
          , std::function<int()> f
          , const di::named<int, int_name> ni)
        : sp(sp)
    {
        assert(dynamic_cast<implementation1*>(sp.get()));
        assert(!b); // default initialization
        assert(i == 42);
        assert(f() == 87);
        assert(ni == 123);
    }

    std::shared_ptr<interface1> sp;
};

struct app {
    app(service copy
      , std::shared_ptr<interface1> sp
      , std::unique_ptr<interface2> ap
      , int i
      , di::named<std::string, some_name> s
      //, di::named<const std::string&, some_name> s
      , float& f
      , const double& d)
      : str(s)
      , f(f)
      , d(d)
    {
        assert(dynamic_cast<implementation2*>(ap.get()));
        assert(dynamic_cast<implementation1*>(sp.get()));
        assert(copy.sp.get() == sp.get());
        assert(i == 42);
        assert(str == "some_name");
        assert(f == 0.f);
        assert(d == 0.f);
    }

    app& operator=(const app&) = delete;

    std::string str;
    float& f;
    const double& d;
};

int main() {
    float f = 0.f;
    double d = 0.f;

    /*<<create injector with configuration>>*/
    auto injector = di::make_injector(
        di::bind<interface1, implementation1>
      , di::bind<interface2, implementation2>
      , di::bind<int>.to(42)
      , di::bind<std::string>.named(some_name{}).to("some_name")
      , di::bind<float>.to(std::ref(f))
      , di::bind<double>.to(std::cref(d))
      , di::bind<std::function<int()>>.to([]{return 87;})
      , di::bind<int>.named(int_name{}).to(123)
    );

    /*<<create `service_app`>>*/
    auto service_app = injector.create<app>();

    /*<<verify parameter `f` affection by `service_app`>>*/
    service_app.f = 42.f;
    assert(f == 42.f);

    d = 42.f;
    assert(service_app.d == 42.f);

    return 0;
}

//]

