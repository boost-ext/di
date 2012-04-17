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
namespace utility = di::utility;

struct basemodule : base::module <
    base::singletons <
        C1, C2, C3, C4
    >,
    base::per_requests <
        CIf0,
        base::bind<CIf01>::in_call_stack<C6, C5>,
        base::bind<CIf02>::in_call_stack<C7>,
        base::bind<int, int_<1> >,
        base::bind<int, int_<2> >::in_call_stack<C8>,
        base::bind<int, int_<3> >::in_name< string<'1'> >::in_call_stack<C7, C6, C4>,
        base::bind<int, int_<4> >::in_name< string<'2'> >::in_call_stack<C7, C6, C4>,
        base::bind<int, int_<5> >::in_call_stack<C2>
    >
> { };

BOOST_AUTO(fusionmodule, fusion::module<>()(
    fusion::per_requests <
        Transactionprovider
    >()
));

int main()
{
    utility::injector
    <
        basemodule, BOOST_TYPEOF(fusionmodule)
    >
    injector;

    injector.create<C8>();
    injector.create<C7>();
    injector.create<C6>();
    injector.create<C5>();
    injector.create<TransactionUsage>();

    return 0;
}

