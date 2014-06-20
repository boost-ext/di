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

struct converter {
    template<typename T> operator T() const {
        return T();
    }

    template<typename T> operator T&() const {
        static T t;
        return t;
    }

    template<typename T> operator const T&() const {
        static T t;
        return t;
    }
};

template<typename T>
struct ctor {
    struct any_type {
        template<typename U> operator const U&() const;
        template<typename U> operator U&() const;
        template<typename U> operator U();
        template<typename U> operator std::auto_ptr<U>&();
    };

    template<typename U>
    static mpl::aux::yes_tag test(BOOST_DI_FEATURE_DECLTYPE(
        U(any_type(), any_type(), any_type(), any_type(), any_type(), any_type()))*
    );

    template<typename>
    static mpl::aux::no_tag test(...);

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test<T>(0)) == sizeof(mpl::aux::yes_tag)
    );
};

struct empty { };
struct app {
    app(int, const int&, int&, std::auto_ptr<int>, int*, const int*)
    { }
};

BOOST_AUTO_TEST_CASE(conversion) {
    app app_(converter(), converter(), converter(), converter(), converter(), converter());
}

BOOST_AUTO_TEST_CASE(constructor) {
    BOOST_CHECK(!ctor<empty>::value);
    BOOST_CHECK(ctor<app>::value);
}

} // namespace di
} // namespace boost

