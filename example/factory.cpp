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
    void dummy() {}
};

struct impl2 : i
{
    void dummy() {}
};

struct c
{
    BOOST_DI_CTOR(c, std::shared_ptr<i> i) {
        assert(i.get());
        assert(dynamic_cast<impl1*>(i.get()));
    }
};

enum eid
{
    e1, e2
};

class i_factory
{
public:
    i* BOOST_DI_CREATE(eid id) {
        switch(id) {
            case e1: return new impl1();
            case e2: return new impl2();
        }

        return nullptr;
    }
};

} // namespace

int main() {
    auto injector = di::make_injector(
        di::bind<eid>::to(e1)
      , di::bind<i, i_factory>()
    );

    injector.create<c>();

    return 0;
}

