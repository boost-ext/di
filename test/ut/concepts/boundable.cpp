//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/boundable.hpp"
#include "common/fakes/fake_dependency.hpp"

#include <iostream>
namespace boost { namespace di { inline namespace v1 { namespace concepts {

struct a { };
struct b : a { };
struct c : a { };
struct d { };

test bind_expected_given = [] {
    static_expect(boundable<int, int>::value);
    static_expect(boundable<int, short>::value);
    static_expect(std::is_same<bound_type<std::string>::is_not_related_to<int>, boundable<int, std::string>>::value);
    static_expect(std::is_same<bound_type<int*>::has_disallowed_specifiers, boundable<int*, int>>::value);
    static_expect(std::is_same<bound_type<int*>::has_disallowed_specifiers, boundable<int, int*>>::value);
    static_expect(std::is_same<bound_type<double*>::has_disallowed_specifiers, boundable<int, double*>>::value);
};

test bind_deps = [] {
    static_expect(boundable<aux::type_list<>>::value);
    static_expect(boundable<aux::type_list<fake_dependency<int>>>::value);
    static_expect(std::is_same<bound_type<int, no_name>::is_bound_more_than_once, boundable<aux::type_list<fake_dependency<int>, fake_dependency<int>>>>::value);
    static_expect(std::is_same<bound_type<double, no_name>::is_bound_more_than_once, boundable<aux::type_list<fake_dependency<double>, fake_dependency<int>, fake_dependency<double>>>>::value);
    static_expect(std::is_same<bound_type<int>::is_neither_a_dependency_nor_an_injector, boundable<aux::type_list<int>>>::value);
};

test bind_any_of = [] {
    static_expect(boundable<aux::type_list<a, c>, c>::value);
    static_expect(std::is_same<
        aux::type_list<
            bound_type<d>::is_not_related_to<a>
          , bound_type<d>::is_not_related_to<b>
          , bound_type<d>::is_not_related_to<c>
        >
      , boundable<aux::type_list<a, b, c>, d>
    >::value);
};

test bind_injector = [] {
    static_expect(boundable<aux::type<>>::value);
    static_expect(boundable<aux::type<int>>::value);
    static_expect(std::is_same<bound_type<int>::is_bound_more_than_once, boundable<aux::type<int, int>>>::value);
    static_expect(std::is_same<bound_type<int>::is_bound_more_than_once, boundable<aux::type<int, double, int, float, double>>>::value);
    static_expect(std::is_same<bound_type<double>::is_bound_more_than_once, boundable<aux::type<int, double, float, double>>>::value);
};

}}}} // boost::di::v1::concepts

