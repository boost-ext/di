//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <cassert>
#include <memory>
#include <boost/mpl/int.hpp>
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
    using module = di::generic_module<
        di::bind<i, i_factory>
      , di::external<eid>
    >;

    di::injector<module> injector(
        module(module::set<eid>(e1))
    );

    injector.create<c>();

    return 0;
}

