//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/mpl.hpp"

#include <memory>
#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {

struct any_type {
    BOOST_DI_WKND(NO_MSVC)(
        template<typename T> operator T() {
            return T();
        }
    )

    template<typename T> operator const T&() const {
        static T t;
        return t;
    }

    template<typename T> operator T&() const {
        static T t;
        return t;
    }

    template<typename T> operator std::auto_ptr<T>&() {
        static std::auto_ptr<T> t;
        return t;
    }
};

struct empty { };
struct app {
    app(int, const int&, int&, std::auto_ptr<int>, int*, const int*) { }
};

BOOST_AUTO_TEST_CASE(conversion) {
    std::auto_ptr<app> app_(new app(any_type(), any_type(), any_type(), any_type(), any_type(), any_type()));
}

BOOST_AUTO_TEST_CASE(constructor) {
    BOOST_CHECK((!std::is_constructible<empty, any_type, any_type, any_type, any_type, any_type, any_type>::value));
    BOOST_CHECK((std::is_constructible<app, any_type, any_type, any_type, any_type, any_type, any_type>::value));
}

} // namespace di
} // namespace boost

