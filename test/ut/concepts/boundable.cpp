//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/concepts/boundable.hpp"
#include <string>
#include <type_traits>
#include "common/fakes/fake_dependency.hpp"

namespace concepts {

struct a {};
struct b : a {};
struct c : a {};
struct d {};
struct i {
  virtual ~i() noexcept = default;
  virtual void dummy() = 0;
};
struct impl_fail : i {};
struct impl_okay : i {
  void dummy() override{};
};

test bind_expected_given = [] {
  static_expect(boundable<int, int>::value);
  static_expect(boundable<i, impl_okay>::value);
  static_expect(boundable<class fwd1, class fwd2>::value);
  static_expect(boundable<i, impl_fail, aux::valid<>>::value);  // instance
  static_expect(std::is_same<type_<short>::is_not_related_to<int>, boundable<int, short>>::value);
  static_expect(std::is_same<type_<std::string>::is_not_related_to<int>, boundable<int, std::string>>::value);
  static_expect(std::is_same<type_<int*>::has_disallowed_qualifiers, boundable<any_of<int*, int>>>::value);
  static_expect(std::is_same<type_<int*>::has_disallowed_qualifiers, boundable<any_of<int, int*>>>::value);
  static_expect(std::is_same<type_<double*>::has_disallowed_qualifiers, boundable<any_of<int, double*>>>::value);
  static_expect(std::is_same<type_<impl_fail>::is_abstract, boundable<i, impl_fail>>::value);
};

test bind_any_of = [] {
  static_expect(boundable<any_of<a, c>, c>::value);
  static_expect(
      std::is_same<any_of<type_<d>::is_not_related_to<a>, type_<d>::is_not_related_to<b>, type_<d>::is_not_related_to<c>>,
                   boundable<any_of<a, b, c>, d>>::value);
};

test bind_deps = [] {
  const auto empty_expr = [] {};
  static_expect(boundable<aux::type_list<>>::value);
  static_expect(boundable<aux::type_list<fake_dependency<int>>>::value);
  static_expect(boundable<aux::type_list<decltype(empty_expr)>>::value);
  static_expect(std::is_same<type_<int>::is_bound_more_than_once,
                             boundable<aux::type_list<fake_dependency<int>, fake_dependency<int>>>>::value);
  static_expect(
      std::is_same<type_<double>::is_bound_more_than_once,
                   boundable<aux::type_list<fake_dependency<double>, fake_dependency<int>, fake_dependency<double>>>>::value);
  static_expect(
      std::is_same<type_<double>::named<a>::is_bound_more_than_once,
                   boundable<aux::type_list<fake_dependency<double, fake_scope<>, std::false_type, a>, fake_dependency<int>,
                                            fake_dependency<double, fake_scope<>, std::false_type, a>>>>::value);
  static_expect(std::is_same<type_<int>::is_neither_a_dependency_nor_an_injector, boundable<aux::type_list<int>>>::value);
};

test bind_injector = [] {
  static_expect(boundable<aux::type<>>::value);
  static_expect(boundable<aux::type<int>>::value);
  static_expect(std::is_same<type_<int>::is_bound_more_than_once, boundable<aux::type<int, int>>>::value);
  static_expect(
      std::is_same<type_<int>::is_bound_more_than_once, boundable<aux::type<int, double, int, float, double>>>::value);
  static_expect(std::is_same<type_<double>::is_bound_more_than_once, boundable<aux::type<int, double, float, double>>>::value);
};

}  // namespace concepts
