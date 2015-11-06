//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <string>
#include <type_traits>
#include "boost/di/concepts/boundable.hpp"
#include "common/fakes/fake_dependency.hpp"

namespace concepts {

struct a {};
struct b : a {};
struct c : a {};
struct d {};

test bind_expected_given = [] {
  expect(boundable<int, int>::value);
  expect(std::is_same<bind<short>::is_not_related_to<int>, boundable<int, short>>::value);
  expect(std::is_same<bind<std::string>::is_not_related_to<int>, boundable<int, std::string>>::value);
  expect(std::is_same<bind<int*>::has_disallowed_qualifiers, boundable<any_of<int*, int>>>::value);
  expect(std::is_same<bind<int*>::has_disallowed_qualifiers, boundable<any_of<int, int*>>>::value);
  expect(std::is_same<bind<double*>::has_disallowed_qualifiers, boundable<any_of<int, double*>>>::value);
};

test bind_deps = [] {
  expect(boundable<aux::type_list<>>::value);
  expect(boundable<aux::type_list<fake_dependency<int>>>::value);
  expect(std::is_same<bind<int>::is_bound_more_than_once,
                      boundable<aux::type_list<fake_dependency<int>, fake_dependency<int>>>>::value);
  expect(std::is_same<
         bind<double>::is_bound_more_than_once,
         boundable<aux::type_list<fake_dependency<double>, fake_dependency<int>, fake_dependency<double>>>>::value);
  expect(std::is_same<
         bind<double>::named<a>::is_bound_more_than_once,
         boundable<aux::type_list<fake_dependency<double, fake_scope<>, std::false_type, a>, fake_dependency<int>,
                                  fake_dependency<double, fake_scope<>, std::false_type, a>>>>::value);
  expect(std::is_same<bind<int>::is_neither_a_dependency_nor_an_injector, boundable<aux::type_list<int>>>::value);
};

test bind_any_of = [] {
  expect(boundable<any_of<a, c>, c>::value);
  expect(
      std::is_same<any_of<bind<d>::is_not_related_to<a>, bind<d>::is_not_related_to<b>, bind<d>::is_not_related_to<c>>,
                   boundable<any_of<a, b, c>, d>>::value);
};

test bind_injector = [] {
  expect(boundable<aux::type<>>::value);
  expect(boundable<aux::type<int>>::value);
  expect(std::is_same<bind<int>::is_bound_more_than_once, boundable<aux::type<int, int>>>::value);
  expect(
      std::is_same<bind<int>::is_bound_more_than_once, boundable<aux::type<int, double, int, float, double>>>::value);
  expect(std::is_same<bind<double>::is_bound_more_than_once, boundable<aux::type<int, double, float, double>>>::value);
};

}  // concepts
