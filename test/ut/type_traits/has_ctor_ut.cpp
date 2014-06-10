//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/type_traits/has_ctor.hpp"

#include <boost/test/unit_test.hpp>

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"

namespace boost {
namespace di {
namespace type_traits {

enum e { };
struct trivial_ctor { };
struct default_ctor { default_ctor(int = 0) { } };
struct explicit_ctor { explicit explicit_ctor(int = 0) { } };
struct copy_ctor { copy_ctor(const copy_ctor&) { } };
struct ctor_auto_ptr { ctor_auto_ptr(aux::auto_ptr<int>) { } };
struct copy_ctor_and_int { copy_ctor_and_int(const copy_ctor_and_int&) { } copy_ctor_and_int(int) { } };
struct copy_ctor_and_many { copy_ctor_and_many(const copy_ctor_and_many&) { } copy_ctor_and_many(int, double) { } };
struct many { many(int, double, float) { } };
struct many_2_3 { many_2_3(int, double) { } many_2_3(int, double, float) { } };
struct many_ref { many_ref(int&, const double&) { } };
struct many_complex { many_complex(const int&, double, float*, aux::shared_ptr<void>, char&) { }};

#if (__cplusplus >= 201100L) || defined(BOOST_MSVC)
    struct many_sp { many_sp(aux::shared_ptr<int>, aux::unique_ptr<int>) { } };
    struct rvalue1 { rvalue1(int&&) { } };
    struct rvalue2 { rvalue2(int, int&&) { } };
#else
    struct many_sp { many_sp(aux::shared_ptr<int>, aux::auto_ptr<int>) { } };
#endif

BOOST_AUTO_TEST_CASE(fundamental_types) {
    BOOST_CHECK((!has_ctor<int, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<char, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<float, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<double, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<e, mpl::int_<1> >::value));
}

BOOST_AUTO_TEST_CASE(copy_ctors) {
    BOOST_CHECK((!has_ctor<trivial_ctor, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<copy_ctor, mpl::int_<1> >::value));

#if (__cplusplus >= 201100L) || defined(BOOST_MSVC)
    BOOST_CHECK((has_ctor<default_ctor, mpl::int_<1> >::value));
    BOOST_CHECK((has_ctor<explicit_ctor, mpl::int_<1> >::value));
    BOOST_CHECK((has_ctor<copy_ctor_and_int, mpl::int_<1> >::value));
    BOOST_CHECK((has_ctor<ctor_auto_ptr, mpl::int_<1> >::value));
#else
    BOOST_CHECK((!has_ctor<default_ctor, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<explicit_ctor, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<copy_ctor_and_int, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<ctor_auto_ptr, mpl::int_<1> >::value));
#endif

#if (__cplusplus >= 201100L) && !defined(BOOST_MSVC)
    BOOST_CHECK((has_ctor<rvalue1, mpl::int_<1> >::value));
#endif
}

BOOST_AUTO_TEST_CASE(many_arguments) {
    BOOST_CHECK((!has_ctor<copy_ctor_and_many, mpl::int_<1> >::value));
    BOOST_CHECK((has_ctor<copy_ctor_and_many, mpl::int_<2> >::value));
    BOOST_CHECK((!has_ctor<copy_ctor_and_many, mpl::int_<3> >::value));

    BOOST_CHECK((!has_ctor<many, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<many, mpl::int_<2> >::value));
    BOOST_CHECK((has_ctor<many, mpl::int_<3> >::value));

    BOOST_CHECK((!has_ctor<many_2_3, mpl::int_<1> >::value));
    BOOST_CHECK((has_ctor<many_2_3, mpl::int_<2> >::value));
    BOOST_CHECK((has_ctor<many_2_3, mpl::int_<3> >::value));

    BOOST_CHECK((!has_ctor<many_sp, mpl::int_<1> >::value));
    BOOST_CHECK((has_ctor<many_sp, mpl::int_<2> >::value));
    BOOST_CHECK((!has_ctor<many_sp, mpl::int_<3> >::value));

    BOOST_CHECK((!has_ctor<many_ref, mpl::int_<1> >::value));
    BOOST_CHECK((has_ctor<many_ref, mpl::int_<2> >::value));
    BOOST_CHECK((!has_ctor<many_ref, mpl::int_<3> >::value));

    BOOST_CHECK((!has_ctor<many_complex, mpl::int_<4> >::value));
    BOOST_CHECK((has_ctor<many_complex, mpl::int_<5> >::value));
    BOOST_CHECK((!has_ctor<many_complex, mpl::int_<6> >::value));

#if (__cplusplus >= 201100L) && !defined(BOOST_MSVC)
    BOOST_CHECK((has_ctor<rvalue2, mpl::int_<2> >::value));
#endif
}

#if (__cplusplus >= 201100L) &&                                             \
    !defined(BOOST_INTEL) &&                                                \
    !(defined(BOOST_GCC) && (BOOST_GCC < 40800)) &&                         \
    !(defined(BOOST_CLANG) && __clang_major__ >= 3 && __clang_minor__ < 3)

BOOST_AUTO_TEST_CASE(inheriting_ctors) {
    struct c0 { c0(int, double) { } };
    struct c1 : public c0 { using c0::c0; };

    BOOST_CHECK((has_ctor<c0, mpl::int_<2> >::value));
    BOOST_CHECK((has_ctor<c1, mpl::int_<2> >::value));
}
#endif

} // namespace type_traits
} // namespace di
} // namespace boost

