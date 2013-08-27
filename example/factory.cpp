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

struct i
{
    virtual ~i(){};
    virtual void dummy() = 0;
};

struct impl : i
{
    void dummy() {}
};

struct c
{
    BOOST_DI_CTOR(c, boost::shared_ptr<i> i) {
        std::cout << i.get() << std::endl;
    }
};

enum eid
{
    e1, e2
};

class i_factory
{
public:
    i* BOOST_DI_CREATE() {
        //std::cout << i << std::endl;
        return new impl();
    }
};

} // namespace

int main()
{
    auto module = di::fusion_module<>()(
        di::deduce<
            di::bind<i, i_factory>
        >()
      , di::bind<int>::to(11)
    );

    di::injector<decltype(module)> injector(module);
    injector.create<c>();

    return 0;
}

