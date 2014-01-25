//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <memory>
#include <boost/di.hpp>

namespace di  = boost::di;
namespace mpl = boost::mpl;

namespace {

struct i
{
    virtual ~i(){};
    virtual void dummy() = 0;
};

struct impl1 : i
{
    BOOST_DI_CTOR(impl1, int i) {
        assert(i == 1);
    }

    void dummy() override {}
};

struct impl2 : i
{
    BOOST_DI_CTOR(impl2, int i) {
        assert(i == 2);
    }

    void dummy() override {}
};

template<typename T>
struct c
{
    BOOST_DI_CTOR(c, std::shared_ptr<i> i) {
        assert(i.get());
        assert(dynamic_cast<T*>(i.get()));
    }
};

enum eid { e1 = 1, e2 = 2 };

} // namespace

class config
{
public:
    explicit config(eid& id)
        : id(id)
    { }

    template<typename T>
    T create() const
    {
        auto common = di::make_injector(
            di::bind<int>::to(static_cast<int>(id))
        );

        auto injector = di::make_injector(
            common
          , di::bind<i>::to([&]() -> std::shared_ptr<i> {
                switch(id) {
                    case e1:
                        return common.create<std::shared_ptr<impl1>>();

                    case e2:
                        return common.create<std::shared_ptr<impl2>>();
                }

                return nullptr;
            }())
        );

        return injector.template create<T>();
    }

    eid& id;
};

int main() {
    auto id = e1;

    config conf((id));
    conf.create<c<impl1>>();

    id = e2;
    conf.create<c<impl2>>();

    return 0;
}

