//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[factory_cpp_14
//````C++14```
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di  = boost::di;
namespace mpl = boost::mpl;

namespace {

enum eid { e1 = 1, e2 = 2 };
struct i { virtual ~i() { }; };
struct impl1 : i { };
struct impl2 : i { };

} // namespace

class config {
    config& operator=(const config&);

public:
    explicit config(eid& id)
        : id(id)
    { }

    config(const config& other)
        : id(other.id)
    { }

    auto configure() const {
        return di::make_injector(
            di::bind<i>::to(
                [&]() -> std::shared_ptr<i> {
                    switch(id) {
                        default: return nullptr;
                        case e1: return std::make_shared<impl1>();
                        case e2: return std::make_shared<impl2>();
                    }

                    return nullptr;
                }
            )
        );
    }

private:
    eid& id;
};

int main() {
    auto id = e1;

    auto injector = di::make_injector(config(id));
    assert(dynamic_cast<impl1*>(injector.create<std::shared_ptr<i>>().get()));

    id = e2;
    assert(dynamic_cast<impl2*>(injector.create<std::shared_ptr<i>>().get()));
    (void)id;

    return 0;
}

//`[table
//`[[Full code example: [@example/cpp_14/factory.cpp factory.cpp]]]]
//]

