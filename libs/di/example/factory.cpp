//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct c0
{ };

struct c
{
    BOOST_DI_CTOR(c, boost::shared_ptr<i> i) {

    }
};

enum eid
{
    e1, e2
};

class i_factory
{
public:
    BOOST_DI_CTOR(factory, eid id)
        : id(id)
    { }

    i* create(di::creator& creator) const {
        switch(id) {
            default: return nullptr;
            case e1: return creator.create<c1*>();
            case e2: return creator.create<c2*>();
        }

        return nullptr;
    }

private:
    eid id;
};

} // namespace

int main()
{
    eid id = e2;

    typedef di::generic_module<
        i_factory // or bind<i, factory>
      , external<eid>
    > generic_module;

    generic_module generic_module_(
        generic_module::set<eid>(id)
    );

    assert(dynamic_cast<c1generic_module_.create<c>();

    id = e1;

    generic_module_.create<c>();

    return 0;
}

