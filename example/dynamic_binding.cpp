//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[dynamic_binding
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di  = boost::di;

//<-
enum eid { e1 = 1, e2 = 2 };
struct interface { virtual ~interface() { }; };
struct implementation1 : interface { };
struct implementation2 : interface { };
//->

class dynamic_binding {
    dynamic_binding& operator=(const dynamic_binding&);

public:
    explicit dynamic_binding(eid& id)
        : id(id)
    { }

    /*<<module configuration>>*/
    auto configure() const {
        return di::make_injector(
            /*<<bind `interface` to lazy lambda expression>>*/
            di::bind<interface>.to(
                [&](const auto& injector) -> std::shared_ptr<interface> {
                    switch(id) {
                        default: return nullptr;
                        case e1: return injector.template create<std::shared_ptr<implementation1>>();
                        case e2: return injector.template create<std::shared_ptr<implementation2>>();
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

    /*<<create interface with `id = e1`>>*/
    auto injector = di::make_injector(dynamic_binding{id});
    assert(dynamic_cast<implementation1*>(injector.create<std::shared_ptr<interface>>().get()));

    id = e2;
    /*<<create interface with `id = e2`>>*/
    assert(dynamic_cast<implementation2*>(injector.create<std::shared_ptr<interface>>().get()));
    (void)id;

    return 0;
}

//]

