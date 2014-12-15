//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_DATA_HPP
#define BOOST_DI_FAKE_DATA_HPP

namespace boost { namespace di {

template<class T, class TDependency>
struct fake_data {
    using type = T;

    TDependency&& dep;
    //pool_t& deps;

    template<class U, class TDefault>
    decltype(auto) resolve() const noexcept {
        //return binder::template resolve<U, TDefault>(&deps);
        return 0;
    }
};

}} // boost::di

#endif

