//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/aux_/instance.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "data.hpp"

namespace boost {
namespace di {
namespace aux_ {

template<typename T, typename TName = void>
struct named
{
    typedef T value_type;

    explicit named(T i)
        : i(i)
    { }

    T i;
};

BOOST_AUTO_TEST_CASE(arithmetic_value)
{
    const int i = 42;
    BOOST_CHECK_EQUAL(i, instance<int>(i).get());
}

BOOST_AUTO_TEST_CASE(arithmetic_with_value_type)
{
    const int i = 42;
    BOOST_CHECK_EQUAL(i, instance<named<int> >(i).get());
}

BOOST_AUTO_TEST_CASE(string_value)
{
    const std::string s = "string";
    BOOST_CHECK_EQUAL(s, instance<std::string>(s).get());
}

BOOST_AUTO_TEST_CASE(string_with_value_type)
{
    const std::string s = "string";
    BOOST_CHECK_EQUAL(s, instance<named<std::string> >(s).get());
}

BOOST_AUTO_TEST_CASE(variant_ref)
{
    c c_;
    c& c_ref_ = get<c&>(instance<c>(c_).get());
    BOOST_CHECK_EQUAL(&c_, &c_ref_);
}

BOOST_AUTO_TEST_CASE(variant_const_ref)
{
    c c_;
    const c& const_c_ref_ = get<const c&>(instance<c>(c_).get());
    BOOST_CHECK_EQUAL(&c_, &const_c_ref_);
}

BOOST_AUTO_TEST_CASE(variant_shared_ptr)
{
    shared_ptr<c> c_(new c);
    BOOST_CHECK_EQUAL(c_, get<shared_ptr<c> >(instance<c>(c_).get()));
}

BOOST_AUTO_TEST_CASE(named_shared_ptr)
{
    typedef named<shared_ptr<int>, a> c1_t;
    typedef named<shared_ptr<int>, b> c2_t;

    shared_ptr<c1_t> c1_(new c1_t(make_shared<int>(42)));
    shared_ptr<c2_t> c2_(new c2_t(make_shared<int>(87)));

    BOOST_CHECK((
        *get<shared_ptr<c1_t> >(instance<c1_t>(c1_).get())->i
        !=
        *get<shared_ptr<c2_t> >(instance<c2_t>(c2_).get())->i
    ));
}

BOOST_AUTO_TEST_CASE(context)
{
    shared_ptr<c> c1_(new c);
    shared_ptr<c> c2_(new c);

    BOOST_CHECK((instance<int, a>(87).get() != instance<int, b>(42).get()));
    BOOST_CHECK((
        get<shared_ptr<c> >(instance<c, a>(c1_).get())
        !=
        get<shared_ptr<c> >(instance<c, b>(c2_).get())
    ));
}

} // namespace aux_
} // namespace di
} // namespace boost

