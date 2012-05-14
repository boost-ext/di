//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <boost/mpl/int.hpp>
#include <boost/mpl/string.hpp>
#include <boost/di.hpp>

namespace mpl = boost::mpl;
namespace di  = boost::di;

namespace {

class named
{
public:
    BOOST_DI_CTOR(named
      , di::named<int, mpl::string<'1'> > i1
      , di::named<int, mpl::string<'2'> > i2
    ) : i1(i1), i2(i2) {
        std::clog <<"i1:" <<i1 <<", i2: " <<i2 <<std::endl;
    }

private:
    int i1;
    int i2;
};

} // namespace

int main()
{
    typedef di::generic_module<
        di::bind<int, mpl::int_<42> >::in_name<mpl::string<'1'> >
      , di::bind<int, mpl::int_<87> >::in_name<mpl::string<'2'> >
    > module;

    di::injector<module> injector;
    injector.create<named>(); //i1: 42, i2: 87

    return 0;
}

