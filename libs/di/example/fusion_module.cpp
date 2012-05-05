//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <typeinfo>
#include <iostream>
#include <boost/mpl/int.hpp>
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

namespace {

struct name { };

struct i0 { virtual ~i0() { }; };

struct c0 : i0
{
    BOOST_DI_CTOR(c0, di::named<int, name>, double) { }
};

struct c01 : i0
{ };

struct c1
{
    BOOST_DI_CTOR(c1, boost::shared_ptr<i0>) { }
};

struct c2
{
    BOOST_DI_CTOR(c2, c1, int, double, char) { }
};

struct c3
{
    BOOST_DI_CTOR(c3, c1, c2, boost::shared_ptr<i0>) { }
};

} // namespace

struct visitor
{
    template<typename T>
    void operator()() const {
        std::cout << typeid(typename T::type).name() << std::endl;
    }
};

int main()
{
    {
        BOOST_AUTO(fusion_module, di::fusion_module<>()(
            di::singletons<
                c1
            >()
          , di::per_requests<
                c0
              , mpl::int_<42>
              , di::bind<int, mpl::int_<87> >::in_name<name>
              , di::bind<c01>::in_call<di::call_stack<c2, c1> >
            >()
          , di::bind<double>::to(42.0)
        ));

        fusion_module.create<c3>();
        fusion_module.visit<c3>(visitor());
    }

    {
        auto fusion_module = di::fusion_module<>()(
            di::singletons<
                c1
            >()
          , di::per_requests<
                c0
              , mpl::int_<42>
              , di::bind<int, mpl::int_<87> >::in_name<name>
              , di::bind<c01>::in_call<di::call_stack<c2, c1> >
            >()
          , di::bind<double>::to(42.0)
        );

        fusion_module.create<c3>();
        fusion_module.visit<c3>(visitor());
    }
}

