//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[factory_cpp_14
//`[h6 C++ 14]
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di  = boost::di;
namespace mpl = boost::mpl;

namespace {

enum eid { e1 = 1, e2 = 2 };

struct i {
    virtual ~i() { };
    virtual void dummy() = 0;
};

struct impl1 : i {
    explicit impl1(const eid& id) {
        assert(id == e1);
    }

    void dummy() override { }
};

struct impl2 : i {
    explicit impl2(const eid& id) {
        assert(id == e2);
    }

    void dummy() override { }
};

template<typename T>
struct c {
    c(std::shared_ptr<i> sp) {
        assert(sp.get());
        assert(dynamic_cast<T*>(sp.get()));
    }
};

} // namespace

class config {
    config& operator=(const config&);

public:
    explicit config(eid& id)
        : id(id)
    { }

    auto configure() const {
        auto common = di::make_injector(
            di::bind<eid>::to(id)
        );

        return di::make_injector(
            common
          , di::bind<i>::to(
                [&]() -> std::shared_ptr<i> {
                    switch(id) {
                        case e1: return common.create<std::shared_ptr<impl1>>();
                        case e2: return common.create<std::shared_ptr<impl2>>();
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

    config conf((id));
    auto injector = di::make_injector(conf);
    injector.create<c<impl1>>();

    id = e2;
    injector.create<c<impl2>>();
    (void)id;

    return 0;
}

//`full code example: [@example/cpp_14/factory.cpp factory.cpp]
//]

