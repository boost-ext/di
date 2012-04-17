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
#include "common/data.hpp"

using namespace boost::mpl;

namespace base    = di::front::base;
namespace fusion  = di::front::fusion;
namespace Utility = di::Utility;

struct baseModule : base::Module <
    base::singletons <
        C1, C2, C3, C4
    >,
    base::per_requests <
        CIf0,
        base::Bind<CIf01>::InCallStack<C6, C5>,
        base::Bind<CIf02>::InCallStack<C7>,
        base::Bind<int, int_<1> >,
        base::Bind<int, int_<2> >::InCallStack<C8>,
        base::Bind<int, int_<3> >::InName< string<'1'> >::InCallStack<C7, C6, C4>,
        base::Bind<int, int_<4> >::InName< string<'2'> >::InCallStack<C7, C6, C4>,
        base::Bind<int, int_<5> >::InCallStack<C2>
    >
> { };

BOOST_AUTO(fusionModule, fusion::Module<>()(
    fusion::per_requests <
        TransactionProvider
    >()
));

int main()
{
    Utility::Injector
    <
        baseModule, BOOST_TYPEOF(fusionModule)
    >
    injector;

    injector.create<C8>();
    injector.create<C7>();
    injector.create<C6>();
    injector.create<C5>();
    injector.create<TransactionUsage>();

    return 0;
}

