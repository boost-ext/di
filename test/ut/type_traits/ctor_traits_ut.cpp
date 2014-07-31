//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/ctor_traits.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/x11/vector.hpp>
#include <boost/mpl/x11/equal.hpp>
#include <boost/mpl/x11/string.hpp>
#include <boost/config.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/core/any_type.hpp"
#include "boost/di/inject.hpp"
#include "boost/di/named.hpp"

#include <iostream>
#include <boost/units/detail/utility.hpp>
namespace boost {
namespace di {

struct traits { };

template<typename>
struct ctor_traits;

template<>
struct ctor_traits<traits>
{
    static void BOOST_DI_INJECTOR();
};

namespace type_traits {

struct empty
{
    static void BOOST_DI_INJECTOR();
};

struct int_double
{
    static void BOOST_DI_INJECTOR(int, double);
};

struct extensions
{
    static void BOOST_DI_INJECTOR(char*, const int&);
};

struct ctor0
{ };

struct ctor1
{
    ctor1(int);
};

struct ctor2
{
    ctor2(char*, const int&);
};

struct ctor_complex
{
    ctor_complex(int, double&, aux::shared_ptr<int>, float&, const char*, const std::string&, void*, aux::auto_ptr<int>) { }
};

struct ctor_auto_ptr
{
    ctor_auto_ptr(aux::auto_ptr<int>) { }
};

struct ctor_inject_named
{
    static void BOOST_DI_INJECTOR(
        named<int, mpl::x11::string<'1'> >
      , named<int, mpl::x11::string<'2'> >
    );

    ctor_inject_named(int, int);
};

#if !defined(BOOST_NO_RVALUE_REFERENCES)
    struct rvalue
    {
        rvalue(int&&) { };
    };
#endif

BOOST_AUTO_TEST_CASE(ctors) {
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<>, ctor_traits<empty>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<>, ctor_traits<traits>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<>, ctor_traits<ctor0>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<int, double>, ctor_traits<int_double>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<char*, const int&>, ctor_traits<extensions>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<core::any_type<ctor2>, core::any_type<ctor2> >, ctor_traits<ctor2>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<core::any_type<ctor_complex>, core::any_type<ctor_complex>, core::any_type<ctor_complex>, core::any_type<ctor_complex>, core::any_type<ctor_complex>, core::any_type<ctor_complex>, core::any_type<ctor_complex>, core::any_type<ctor_complex> >, ctor_traits<ctor_complex>::type>::value));
    BOOST_CHECK((mpl::x11::equal<
        mpl::x11::vector<
            named<int, mpl::x11::string<'1'> >
          , named<int, mpl::x11::string<'2'> >
        >
      , ctor_traits<ctor_inject_named>::type>::value
    ));

#if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<>, ctor_traits<ctor1>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<>, ctor_traits<ctor_auto_ptr>::type>::value));
#else
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<core::any_type<ctor1> >, ctor_traits<ctor1>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<core::any_type<ctor_auto_ptr> >, ctor_traits<ctor_auto_ptr>::type>::value));
#endif

#if !defined(BOOST_NO_RVALUE_REFERENCES) && !defined(BOOST_MSVC)
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<core::any_type<rvalue> >, ctor_traits<rvalue>::type>::value));
#endif
}

#if (__cplusplus >= 201100L) &&                                             \
    !defined(BOOST_INTEL) &&                                                \
    !(defined(BOOST_GCC) && (BOOST_GCC < 40800)) &&                         \
    !(defined(BOOST_CLANG) && __clang_major__ >= 3 && __clang_minor__ < 3)

BOOST_AUTO_TEST_CASE(inheriting_ctors) {
    struct c0 { c0(int, double) { } };
    struct c1 : public c0 { using c0::c0; };

    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<core::any_type<c0>, core::any_type<c0> >, ctor_traits<c0>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<core::any_type<c1>, core::any_type<c1> >, ctor_traits<c1>::type>::value));
}

BOOST_AUTO_TEST_CASE(inheriting_ctors_inject) {
    struct c0 { c0(int, double) { } static void BOOST_DI_INJECTOR(int, double); };
    struct c1 : public c0 { using c0::c0; };
    struct c2 : public c0 { };
    struct c3 : public c0 { static void BOOST_DI_INJECTOR(); };

    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<int, double>, ctor_traits<c0>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<int, double>, ctor_traits<c1>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<int, double>, ctor_traits<c2>::type>::value));
    BOOST_CHECK((mpl::x11::equal<mpl::x11::vector<>, ctor_traits<c3>::type>::value));
}
#endif

} // namespace type_traits
} // namespace di
} // namespace boost

