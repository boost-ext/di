//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/type_traits/function_traits.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

namespace boost {
namespace di {
namespace type_traits {

void f1() { }
int f2(int) { return {}; }
int f3(int, const double&) { return {}; }

struct c1
{
    void f1() { }
    int f2(int) { return {}; }
    int f3(int, const double&) { return {}; }
};

struct c2
{
    void f1() const { }
    int f2(int) const { return {}; }
    int f3(int, const double&) const { return {}; }
};

BOOST_AUTO_TEST_CASE(parameters_type_functions) {
    BOOST_CHECK((is_same<void, typename function_traits<decltype(&f1)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<>, typename function_traits<decltype(&f1)>::type>::value));

    BOOST_CHECK((is_same<int, typename function_traits<decltype(&f2)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int>, typename function_traits<decltype(&f2)>::type>::value));

    BOOST_CHECK((is_same<int, typename function_traits<decltype(&f3)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int, const double&>, typename function_traits<decltype(&f3)>::type>::value));
}

BOOST_AUTO_TEST_CASE(parameters_type_methods) {
    BOOST_CHECK((is_same<void, typename function_traits<decltype(&c1::f1)>::result_type>::value));
    BOOST_CHECK((is_same<c1, typename function_traits<decltype(&c1::f1)>::base_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<>, typename function_traits<decltype(&c1::f1)>::type>::value));

    BOOST_CHECK((is_same<int, typename function_traits<decltype(&c1::f2)>::result_type>::value));
    BOOST_CHECK((is_same<c1, typename function_traits<decltype(&c1::f2)>::base_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int>, typename function_traits<decltype(&c1::f2)>::type>::value));

    BOOST_CHECK((is_same<int, typename function_traits<decltype(&c1::f3)>::result_type>::value));
    BOOST_CHECK((is_same<c1, typename function_traits<decltype(&c1::f3)>::base_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int, const double&>, typename function_traits<decltype(&c1::f3)>::type>::value));
}

BOOST_AUTO_TEST_CASE(parameters_type_const_methods) {
    BOOST_CHECK((is_same<void, typename function_traits<decltype(&c2::f1)>::result_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<>, typename function_traits<decltype(&c2::f1)>::type>::value));

    BOOST_CHECK((is_same<int, typename function_traits<decltype(&c2::f2)>::result_type>::value));
    BOOST_CHECK((is_same<c2, typename function_traits<decltype(&c2::f2)>::base_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int>, typename function_traits<decltype(&c2::f2)>::type>::value));

    BOOST_CHECK((is_same<int, typename function_traits<decltype(&c2::f3)>::result_type>::value));
    BOOST_CHECK((is_same<c2, typename function_traits<decltype(&c2::f3)>::base_type>::value));
    BOOST_CHECK((mpl::equal<mpl::vector<int, const double&>, typename function_traits<decltype(&c2::f3)>::type>::value));
}

} // namespace type_traits
} // namespace di
} // namespace boost


//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/type_traits/has_call_operator.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/config.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct call_operator_void_const
{
    void operator()() const { }
};

struct call_operator_int_arg
{
    int operator()(int i) { return i; }
};

struct call_operator_derived
    : call_operator_void_const
{ };

BOOST_AUTO_TEST_CASE(none) {
    BOOST_CHECK(!has_call_operator<int>::value);
    BOOST_CHECK(!has_call_operator<void>::value);
    BOOST_CHECK(!has_call_operator<empty>::value);
}

BOOST_AUTO_TEST_CASE(class_with_call_operator) {
    BOOST_CHECK(has_call_operator<call_operator_void_const>::value);
    BOOST_CHECK(has_call_operator<call_operator_int_arg>::value);
}

BOOST_AUTO_TEST_CASE(class_with_derived_call_operator) {
    BOOST_CHECK(has_call_operator<call_operator_derived>::value);
}

BOOST_AUTO_TEST_CASE(function) {
    BOOST_CHECK(has_call_operator<boost::function<int()> >::value);
}

BOOST_AUTO_TEST_CASE(bind_with_class) {
    typedef BOOST_TYPEOF(boost::bind<void>(call_operator_void_const())) bind_t;
    BOOST_CHECK(has_call_operator<bind_t>::value);
}

int return_int(int i) { return i; }

BOOST_AUTO_TEST_CASE(bind_with_function) {
    typedef BOOST_TYPEOF(boost::bind(&return_int, 0)) bind_t;
    BOOST_CHECK(has_call_operator<bind_t>::value);
}

#if !defined(BOOST_NO_CXX11_LAMBDAS)
    template<typename T>
    void test(const T&) {
        BOOST_CHECK(has_call_operator<T>::value);
    }

    BOOST_AUTO_TEST_CASE(lambda) {
        test([]{});
        test([]{ return 42; });
        test([]() -> int { return 42; });
    }
#endif

} // namespace type_traits
} // namespace di
} // namespace boost


//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/type_traits/has_call.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct call_int
{
    void call(const int&) { }
};

struct call_derived
    : call_int
{ };

struct call_template
{
    template<typename T>
    void call(const T&) { }
};

BOOST_AUTO_TEST_CASE(calls) {
    BOOST_CHECK((!has_call<empty, int>::value));

    BOOST_CHECK((has_call<call_int, int>::value));
    BOOST_CHECK((has_call<call_derived, int>::value));
    BOOST_CHECK((has_call<call_template, int>::value));
    BOOST_CHECK((has_call<call_template, double>::value));

    BOOST_CHECK((has_call<call_int, short>::value)); // short is convertible to int
    BOOST_CHECK((has_call<call_derived, short>::value)); // short is convertible to int
}

} // namespace type_traits
} // namespace di
} // namespace boost


//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/type_traits/has_configure.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct with_configure
{
    int configure() { return 42; };
};

struct with_configure_derived
    : with_configure
{ };

struct with_configure_const
{
    int configure() const { return 42; };
};

BOOST_AUTO_TEST_CASE(configures) {
    BOOST_CHECK((!has_configure<empty>::value));

    BOOST_CHECK((has_configure<with_configure>::value));
    BOOST_CHECK((has_configure<with_configure_derived>::value));
    BOOST_CHECK((has_configure<with_configure_const>::value));
}

} // namespace type_traits
} // namespace di
} // namespace boost


//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/type_traits/has_ctor.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/function.hpp>

#include "boost/di/cpp_0x/aux_/config.hpp"
#include "boost/di/cpp_0x/aux_/memory.hpp"

namespace boost {
namespace di {
namespace type_traits {

enum e { };
struct trivial_ctor { };
struct default_ctor { default_ctor(int = 0) { } };
struct explicit_ctor { explicit explicit_ctor(int = 0) { } };
struct copy_ctor { copy_ctor(const copy_ctor&) { } };
struct ctor_auto_ptr { ctor_auto_ptr(aux::auto_ptr<int>) { } };
struct ctor_shared_ptr { ctor_shared_ptr(aux::shared_ptr<int>) { } };
struct copy_ctor_and_int { copy_ctor_and_int(const copy_ctor_and_int&) { } copy_ctor_and_int(int) { } };
struct copy_ctor_and_many { copy_ctor_and_many(const copy_ctor_and_many&) { } copy_ctor_and_many(int, double) { } };
struct many { many(int, double, float) { } };
struct many_2_3 { many_2_3(int, double) { } many_2_3(int, double, float) { } };
struct many_ref { many_ref(int&, const double&) { } };
struct many_complex { many_complex(const int&, double, float*, aux::shared_ptr<void>, char&, function<int()>) { }};

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
    BOOST_CHECK((has_ctor<ctor_shared_ptr, mpl::int_<1> >::value));
#else
    BOOST_CHECK((!has_ctor<default_ctor, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<explicit_ctor, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<copy_ctor_and_int, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<ctor_auto_ptr, mpl::int_<1> >::value));
    BOOST_CHECK((!has_ctor<ctor_shared_ptr, mpl::int_<1> >::value));
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

    BOOST_CHECK((!has_ctor<many_complex, mpl::int_<5> >::value));
    BOOST_CHECK((has_ctor<many_complex, mpl::int_<6> >::value));
    BOOST_CHECK((!has_ctor<many_complex, mpl::int_<7> >::value));

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


//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/type_traits/has_injector.hpp"

#include <boost/test/unit_test.hpp>

#include <boost/preprocessor/cat.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct injector_empty
{
    static void BOOST_DI_INJECTOR();
};

struct injector_args
{
    static void BOOST_DI_INJECTOR(int, double);
};

struct injector_derived
    : injector_empty
{ };

BOOST_AUTO_TEST_CASE(injectors) {
    BOOST_CHECK(!BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<void>::value);
    BOOST_CHECK(!BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<int>::value);
    BOOST_CHECK(!BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<empty>::value);

    BOOST_CHECK(BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<injector_empty>::value);
    BOOST_CHECK(BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<injector_args>::value);
    BOOST_CHECK(BOOST_PP_CAT(has_, BOOST_DI_INJECTOR)<injector_derived>::value);
}

} // namespace type_traits
} // namespace di
} // namespace boost


//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/type_traits/has_value.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct value_int { int value; };
struct value_double { double value; };
struct value_int_static { static double value; };
struct value_derived : value_int { };

BOOST_AUTO_TEST_CASE(values) {
    BOOST_CHECK(!has_value<int>::value);
    BOOST_CHECK(!has_value<void>::value);

    BOOST_CHECK(has_value<value_int>::value);
    BOOST_CHECK(has_value<value_double>::value);
    BOOST_CHECK(has_value<value_int_static>::value);
    BOOST_CHECK(has_value<value_derived>::value);
}

} // namespace type_traits
} // namespace di
} // namespace boost


//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/type_traits/is_convertible_to_ref.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct empty { };

struct convertible_to_int
{
    int operator()() { return 0; }
};

struct convertible_to_int_const
{
    int operator()() const { return 0; }
};

struct convertible_to_int_ref
{
    int& operator()(const boost::type<int&>&) const { static int i = 0; return i; }
};

struct convertible_to_const_int_ref
{
    const int& operator()(const boost::type<const int&>&) const { static int i = 0; return i; }
};

BOOST_AUTO_TEST_CASE(convertbiles) {
    BOOST_CHECK((detail::is_convertible<convertible_to_int, int(convertible_to_int::*)()>::value));
    BOOST_CHECK((detail::is_convertible<convertible_to_int_const, int(convertible_to_int_const::*)() const>::value));

    BOOST_CHECK((!detail::is_convertible<int, int(empty::*)()>::value));
    BOOST_CHECK((!detail::is_convertible<convertible_to_int, int(convertible_to_int::*)() const>::value));
    BOOST_CHECK((!detail::is_convertible<convertible_to_int, double(convertible_to_int::*)() const>::value));
    BOOST_CHECK((!detail::is_convertible<convertible_to_int_const, int(convertible_to_int_const::*)()>::value));
    BOOST_CHECK((!detail::is_convertible<convertible_to_int_const, double(convertible_to_int_const::*)() const>::value));
}

BOOST_AUTO_TEST_CASE(convertbiles_to_const_ref) {
    BOOST_CHECK((is_convertible_to_ref<convertible_to_const_int_ref, int>::value));

    BOOST_CHECK((!is_convertible_to_ref<convertible_to_int_ref, int>::value));
    BOOST_CHECK((!is_convertible_to_ref<empty, int>::value));
    BOOST_CHECK((!is_convertible_to_ref<convertible_to_int_const, int>::value));
    BOOST_CHECK((!is_convertible_to_ref<convertible_to_int, int>::value));
}

} // namespace type_traits
} // namespace di
} // namespace boost


//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/type_traits/is_same_base_of.hpp"

#include <boost/test/unit_test.hpp>

namespace boost {
namespace di {
namespace type_traits {

struct i { };
struct c : i { };
struct a { };

BOOST_AUTO_TEST_CASE(types) {
    BOOST_CHECK((is_same_base_of<int, int>::value));
    BOOST_CHECK((is_same_base_of<c, c>::value));
    BOOST_CHECK((is_same_base_of<c, i>::value));

    BOOST_CHECK((!is_same_base_of<c, a>::value));
}

} // namespace type_traits
} // namespace di
} // namespace boost

//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/cpp_0x/type_traits/make_plain.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/type_traits/is_same.hpp>

#include "boost/di/cpp_0x/aux_/memory.hpp"
#include "boost/di/cpp_0x/named.hpp"
#include "common/data.hpp"

namespace boost {
namespace di {
namespace type_traits {

typedef mpl::vector<int, a> make_plain_t;

BOOST_AUTO_TEST_CASE_TEMPLATE(types, T, make_plain_t) {
    BOOST_CHECK((is_same<T, typename make_plain<T>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<T*>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T*>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<T&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<aux::shared_ptr<T>>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const aux::shared_ptr<T>&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<aux::shared_ptr<T>&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<volatile T>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const named<T, mpl::_1>&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<named<T, mpl::_1>>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<named<aux::shared_ptr<T>, mpl::_1>>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<named<const aux::shared_ptr<T>&, mpl::_1>>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<aux::shared_ptr<named<const aux::shared_ptr<T>&, mpl::_1>>>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const aux::shared_ptr<named<const aux::shared_ptr<T>&, mpl::_1>>&>::type>::value));
}

#if defined(BOOST_HAS_RVALUE_REFERENCES)

BOOST_AUTO_TEST_CASE_TEMPLATE(rvalue_references, T, make_plain_t) {
    BOOST_CHECK((is_same<T, typename make_plain<T&&>::type>::value));
    BOOST_CHECK((is_same<T, typename make_plain<const T&&>::type>::value));
}

#endif

} // namespace type_traits
} // namespace di
} // namespace boost

