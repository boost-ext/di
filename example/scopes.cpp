//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct per_request {
  virtual ~per_request() noexcept = default;
  virtual void dummy() = 0;
};
struct per_request_impl : per_request {
  void dummy() override {}
};
struct shared {
  virtual ~shared() noexcept = default;
  virtual void dummy() = 0;
};
struct shared_impl : shared {
  void dummy() override {}
};
//->

struct example {
  // clang-format off
  example(std::shared_ptr<per_request> sp1 /*unique*/
        , std::shared_ptr<per_request> sp2 /*unique*/
        , const shared& s1 /*singleton*/
        , shared& s2 /*singleton*/) {
    assert(sp1 != sp2);
    assert(&s1 == &s2);
  }
  // clang-format on
};

int main() {
  // clang-format off
  auto injector = di::make_injector(
    di::bind<per_request>().to<per_request_impl>().in(di::unique)
  , di::bind<shared>().to<shared_impl>().in(di::singleton)
  );
  // clang-format on

  /*<<create `example`>>*/
  injector.create<example>();
}
