//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/shared_ptr.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/string.hpp>
#include <di/di.hpp>
#include "Common/Data.hpp"

using namespace boost::mpl;

namespace Base    = di::Front::Base;
namespace Fusion  = di::Front::Fusion;
namespace Utility = di::Utility;

struct BaseModule : Base::Module <
    Base::Singletons <
        C1, C2, C3, C4
    >,
    Base::PerRequests <
        CIf0,
        Base::Bind<CIf01>::InCallStack<C6, C5>,
        Base::Bind<CIf02>::InCallStack<C7>,
        Base::Bind<int, int_<1> >,
        Base::Bind<int, int_<2> >::InCallStack<C8>,
        Base::Bind<int, int_<3> >::InName< string<'1'> >::InCallStack<C7, C6, C4>,
        Base::Bind<int, int_<4> >::InName< string<'2'> >::InCallStack<C7, C6, C4>,
        Base::Bind<int, int_<5> >::InCallStack<C2>
    >
> { };

BOOST_AUTO(fusionModule, Fusion::Module<>()(
    Fusion::PerRequests <
        TransactionProvider
    >()
));

int main()
{
    Utility::Injector
    <
        BaseModule, BOOST_TYPEOF(fusionModule)
    >
    injector;

    injector.create<C8>();
    injector.create<C7>();
    injector.create<C6>();
    injector.create<C5>();
    injector.create<TransactionUsage>();

    return 0;
}

