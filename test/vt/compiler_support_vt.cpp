//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/aux_/config.hpp"

#include <memory>
#include <boost/mpl/aux_/yes_no.hpp>
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

template<typename T>
struct ctor {
    template<typename U>
    static aux::mpl::aux::yes_tag test(BOOST_DI_FEATURE_DECLTYPE(
        U(any_type(), any_type(), any_type(), any_type(), any_type(), any_type()))*
    );

    template<typename>
    static aux::mpl::aux::no_tag test(...);

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test<T>(0)) == sizeof(aux::mpl::aux::yes_tag)
    );
};

struct empty { };
struct app {
    app(int, const int&, int&, std::auto_ptr<int>, int*, const int*)
    { }
};

BOOST_AUTO_TEST_CASE(conversion) {
    std::auto_ptr<app> app_(new app(any_type(), any_type(), any_type(), any_type(), any_type(), any_type()));
}

BOOST_AUTO_TEST_CASE(constructor) {
    BOOST_CHECK(!ctor<empty>::value);
    BOOST_CHECK(ctor<app>::value);
}

} // namespace di
} // namespace boost

