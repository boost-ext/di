//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <memory>
#include <functional>
#include <cassert>

namespace di = boost::di;

struct i1 { virtual ~i1() noexcept = default; virtual void dummy1() = 0; };
struct i2 { virtual ~i2() noexcept = default; virtual void dummy2() = 0; };
struct impl1 : i1 { void dummy1() override { } };
struct impl2 : i2 { void dummy2() override { } };
struct impl : i1, i2 { void dummy1() override { } void dummy2() override { } };

template<int N> using int_ =
    std::integral_constant<int, N>;

int main() {
    auto l = 42l;
    auto b = false;

    auto injector = di::make_injector(
       di::bind<int>.to(0)
     , di::bind<int>.to(42) [di::override]
     , di::bind<i1>.to(std::make_shared<impl>())
     , di::bind<long>.to(std::ref(l))
     , di::bind<short>.to([]{return 87;})
     , di::bind<i2>.to(
           [&](const auto& injector) -> std::shared_ptr<i2> {
              if (b) {
                  return injector.template
                     create<std::shared_ptr<impl2>>();
              }

              return nullptr;
           }
       )
    );

    assert(42 == injector.create<int>());
    assert(injector.create<std::shared_ptr<i1>>()
           ==
           injector.create<std::shared_ptr<i1>>()
    );
    assert(l == injector.create<long&>());
    assert(&l == &injector.create<long&>());
    assert(87 == injector.create<short>());

    {
    auto object = injector.create<std::shared_ptr<i2>>();
    assert(nullptr == object);
    }

    {
    b = true;
    auto object = injector.create<std::shared_ptr<i2>>();
    assert(dynamic_cast<impl2*>(object.get()));
    }
}

