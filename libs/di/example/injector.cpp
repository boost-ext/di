//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/string.hpp>
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

namespace {

struct if0
{
    virtual ~if0() { }
    virtual void dummy() = 0;
};

struct c0if0 : if0
{
    virtual void dummy() { }
};

struct c1
{
    explicit c1(int = 0) { }
};

struct c2
{
    BOOST_DI_CTOR(c2, int, double, char) { }
};

struct c3
{
    BOOST_DI_CTOR(explicit c3, boost::shared_ptr<if0>) { }
};

struct c4
{
    BOOST_DI_CTOR(c4, boost::shared_ptr<c3>) { }
};

} // namespace

int main()
{
    typedef di::generic_module<
        di::singletons<
            c2, c3, c4
        >,
        di::per_requests<
            c0if0
          , di::bind<c0if0>::in_call<c3>
        >
    > generic_module;

    BOOST_AUTO(fusion_module, di::fusion_module<>()(
        di::singletons<
            c1
        >()
      , di::per_requests<
            di::bind<int, mpl::int_<1> >
        >()
    ));

    di::injector<generic_module, BOOST_TYPEOF(fusion_module)> injector;

    injector.create<c4>();

    return 0;
}

