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

namespace di = boost::di;

struct generic_module : di::generic_module <
    di::singletons <
        c1, c2, c3, c4
    >,
    di::per_requests <
        c0if0,
        base::bind<c1if0>::in_call_stack<c6, c5>,
        base::bind<c2if0>::in_call_stack<c7>,
        base::bind<int, int_<1> >,
        base::bind<int, int_<2> >::in_call_stack<c8>,
        base::bind<int, int_<3> >::in_name< string<'1'> >::in_call_stack<c7, c6, c4>,
        base::bind<int, int_<4> >::in_name< string<'2'> >::in_call_stack<c7, c6, c4>,
        base::bind<int, int_<5> >::in_call_stack<c2>
    >
> { };

BOOST_AUTO(fusion_module, di::fusion_module<>()(
    di::per_requests <
        transaction_provider
    >()
));

int main()
{
    di::injector<basemodule, BOOST_TYPEOF(fusionmodule)> injector;

    injector.create<c8>();
    injector.create<c7>();
    injector.create<c6>();
    injector.create<c5>();
    injector.create<transaction_usage>();

    return 0;
}

