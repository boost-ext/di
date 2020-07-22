//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or unique at http://www.boost.org/LICENSE_1_0.txt)
//
#include "boost/di/wrappers/unique.hpp"
#include <memory>
#if __has_include(<boost / shared_ptr.hpp>)
#include <boost/shared_ptr.hpp>
#endif
#include "common/fakes/fake_scope.hpp"

namespace wrappers {

struct interface {
  virtual ~interface() noexcept = default;
  virtual int get_f() = 0;
};
struct implementation : interface {
  implementation() = default;
  explicit implementation(int f) : f(f) {}
  implementation(implementation&& other) noexcept = default;
  implementation(implementation&) = delete;
  int get_f() override { return f; }

 private:
  int f = 0;
};

constexpr auto i = 42;

test to_value = [] {
  auto object = static_cast<int>(unique<fake_scope<>, int>{i});
  expect(i == object);
};

test to_rvalue = [] {
  auto object = static_cast<int&&>(unique<fake_scope<>, int>{i});
  expect(i == object);
};

test to_interface = [] {
  auto u = unique<fake_scope<>, interface*>{new implementation{}};
  auto object = std::unique_ptr<interface>{static_cast<interface*>(u)};
  expect(dynamic_cast<implementation*>(object.get()));
};

test to_ptr = [] {
  auto object = std::unique_ptr<int>{static_cast<int*>(unique<fake_scope<>, int*>{new int{i}})};
  expect(i == *object);
};

test to_const_ptr = [] {
  std::unique_ptr<const int> object = unique<fake_scope<>, int*>{new int{i}};
  expect(i == *object);
};

test to_move_no_copy_ctor = [] {
  auto u = unique<fake_scope<>, implementation*>{new implementation{i}};
  expect(static_cast<implementation>(u).get_f() == i);
};

test to_move = [] {
  auto object = static_cast<int>(unique<fake_scope<>, int*>{new int{i}});
  expect(i == object);
};

test to_shared_ptr = [] {
  std::shared_ptr<int> object = unique<fake_scope<>, int*>{new int{i}};
  expect(i == *object);
};

#if __has_include(<boost / shared_ptr.hpp>)
test to_boost_shared_ptr = [] {
  boost::shared_ptr<int> object = unique<fake_scope<>, int*>{new int{i}};
  expect(i == *object);
};
#endif

test to_unique_ptr = [] {
  std::unique_ptr<int> object = unique<fake_scope<>, int*>{new int{i}};
  expect(i == *object);
};

test to_unique_ptr_from_unique_ptr = [] {
  std::unique_ptr<int> object = unique<fake_scope<>, std::unique_ptr<int>>{std::make_unique<int>(i)};
  expect(i == *object);
};

}  // namespace wrappers
