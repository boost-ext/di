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
    virtual ~i() { };
    virtual void dummy() = 0;
};

struct impl1 : i
{
    impl1(int i) {
        assert(i == 1);
    }

    void dummy() override {}
};

struct impl2 : i
{
    impl2(int i) {
        assert(i == 2);
    }

    void dummy() override {}
};

template<typename T>
struct c
{
    c(std::shared_ptr<i> sp) {
        assert(sp.get());
        assert(dynamic_cast<T*>(sp.get()));
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
    T create() const {
        auto common = di::make_injector(
            di::bind<int>::to(static_cast<int>(id))
        );

        const auto lambda = [&]() -> std::shared_ptr<i> {
            switch(id) {
                case e1:
                    return common.create<std::shared_ptr<impl1>>();

                case e2:
                    return common.create<std::shared_ptr<impl2>>();
            }

            return nullptr;
        };

        auto injector = di::make_injector(
            common
          , di::bind<i>::to(lambda)
        );

        return injector.template create<T>();
    }

private:
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

