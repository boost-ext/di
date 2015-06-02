//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <memory>
#if __has_include(<boost/shared_ptr.hpp>)
    #include <boost/shared_ptr.hpp>
#endif
#include "boost/di.hpp"

namespace di = boost::di;

#if __has_include(<boost/shared_ptr.hpp>)
    test conversion_to_boost_shared_ptr = [] {
        struct c {
            boost::shared_ptr<int> sp;
        };

        auto injector = di::make_injector(
            di::bind<int>.in(di::shared)
        );

        auto object = injector.create<c>();
        expect(object.sp.get());
    };
#endif

