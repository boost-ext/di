//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CORE_POLICY_HPP
#define BOOST_DI_CORE_POLICY_HPP

#include "boost/di/core/binder.hpp"

namespace boost { namespace di { namespace core {

template<
    class TParent
  , class T
  , class TName
  , class TDependency
  , class TDeps
> struct policy {
    struct arg {
        using type = T;
        using parent = TParent;
        using name = TName;
    };

    using dependency = TDependency;

    template<class TT, class Name, class TDefault>
    using resolve = decltype(binder::resolve<TT, Name, TDefault>((TDeps*)nullptr));
};

}}} // boost::di::core

#endif

