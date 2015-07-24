//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <type_traits>
#include <initializer_list>
#include <string>
#include "boost/di/type_traits/ctor_traits.hpp"
#include "boost/di/core/any_type.hpp"

namespace boost { namespace di { inline namespace v1 {

struct traits { };

template<>
struct ctor_traits<traits> {
    BOOST_DI_INJECT_TRAITS();
};

namespace type_traits {

test ctors = [] {
    struct empty {
        BOOST_DI_INJECT_TRAITS();
    };

    struct int_double {
        BOOST_DI_INJECT_TRAITS(int, double);
    };

    struct extensions {
        BOOST_DI_INJECT_TRAITS(char*, const int&);
    };

    struct ctor1 {
        ctor1(int);
    };

    struct ctor2 {
        ctor2(char*, const int&);
    };

    struct ctor_complex {
        ctor_complex(int
                   , double&
                   , std::shared_ptr<int>
                   , float&
                   , const char*
                   , const std::string&
                   , void*
                   , std::unique_ptr<int>)
        { }
    };

    struct ctor_unique_ptr {
        ctor_unique_ptr(std::unique_ptr<int>) { }
    };

    struct rvalue {
        rvalue(int&&) { };
    };

    enum e { };
    enum class ec { };

    static_expect(std::is_same<aux::pair<direct, aux::type_list<>>, ctor_traits__<empty>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<>>, ctor_traits__<traits>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<>>, ctor_traits__<empty>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<int, double>>, ctor_traits__<int_double>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<char*, const int&>>, ctor_traits__<extensions>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<core::any_type_fwd<ctor2>, core::any_type_fwd<ctor2>>>, ctor_traits__<ctor2>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>, core::any_type_ref_fwd<ctor_complex>>>, ctor_traits__<ctor_complex>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<core::any_type_fwd<ctor1>>>, ctor_traits__<ctor1>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<core::any_type_fwd<ctor_unique_ptr>>>, ctor_traits__<ctor_unique_ptr>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<core::any_type_fwd<rvalue>>>, ctor_traits__<rvalue>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<>>, ctor_traits__<e>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<>>, ctor_traits__<ec>::type>{});
};

test uniforms = [] {
    struct empty { };
    struct ctor1 {
        int i = 0;
    };
    struct ctor2_ref {
        std::unique_ptr<int> ptr;
        int& i;
    };
    static_expect(std::is_same<aux::pair<uniform, aux::type_list<>>, ctor_traits__<empty>::type>{});

#if !defined(BOOST_DI_MSVC)
    static_expect(std::is_same<aux::pair<uniform, aux::type_list<core::any_type_ref_fwd<ctor2_ref>, core::any_type_ref_fwd<ctor2_ref>>>, ctor_traits__<ctor2_ref>::type>{});
#endif
};

test inheriting_ctors = [] {
    struct c0 { c0(int, double) { } };
    struct c1 : public c0 { using c0::c0; };

    static_expect(std::is_same<aux::pair<direct, aux::type_list<core::any_type_fwd<c0>, core::any_type_fwd<c0>>>, ctor_traits__<c0>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<core::any_type_fwd<c1>, core::any_type_fwd<c1>>>, ctor_traits__<c1>::type>{});
};

test inheriting_ctors_inject = [] {
    struct c0 { c0(int, double) { }  BOOST_DI_INJECT_TRAITS(int, double); };
    struct c1 : public c0 { using c0::c0; };
    struct c2 : public c0 { };
    struct c3 : public c0 { BOOST_DI_INJECT_TRAITS(); };

    static_expect(std::is_same<aux::pair<direct, aux::type_list<int, double>>, ctor_traits__<c0>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<int, double>>, ctor_traits__<c1>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<int, double>>, ctor_traits__<c2>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<>>, ctor_traits__<c3>::type>{});
};

test special_std_types = [] {
    static_expect(std::is_same<aux::pair<direct, aux::type_list<>>, ctor_traits__<std::string>::type>{});
    static_expect(std::is_same<aux::pair<direct, aux::type_list<>>, ctor_traits__<std::initializer_list<int>>::type>{});
};

}}}} // boost::di::v1::type_traits

