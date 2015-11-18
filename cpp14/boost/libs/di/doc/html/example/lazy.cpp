//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[lazy
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct interface {
  virtual ~interface() noexcept = default;
  virtual void dummy1() = 0;
};
struct expensive_to_create : interface {
  void dummy1() override {}
};

/*<<simple lazy wrapper>>*/
template <class T>
using lazy = di::injector<T>;

/*<<example `example` class>>*/
struct example {
  explicit example(const lazy<interface>& l) {
    auto object = l.create<std::unique_ptr<interface>>();
    assert(dynamic_cast<expensive_to_create*>(object.get()));
  }
};

int main() {
  /*<<define injector>>*/
  auto injector = di::make_injector(di::bind<interface>().to<expensive_to_create>());

  /*<<create `example`>>*/
  injector.create<example>();
}

//]
