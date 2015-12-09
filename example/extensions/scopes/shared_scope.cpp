//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[shared_scope
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

class shared_scope {
 public:
  template <class, class T>
  class scope {
   public:
    template <class T_>
    using is_referable = typename di::wrappers::shared<shared_scope, T>::template is_referable<T_>;

    template <class, class TProvider>
    static auto try_create(const TProvider& provider) -> decltype(di::wrappers::shared<shared_scope, T>{
        std::shared_ptr<T>{provider.get()}});

    template <class, class TProvider>
    auto create(const TProvider& provider) {
      if (!object_) {
        object_ = std::shared_ptr<T>{provider.get()};
      }
      return di::wrappers::shared<shared_scope, T>{object_};
    }

   private:
    std::shared_ptr<T> object_;
  };
};

static constexpr shared_scope shared{};

//<-
struct interface1 {
  virtual ~interface1() noexcept = default;
};
struct implementation1 : interface1 {};
//->

class example {
 public:
  example(std::shared_ptr<interface1> sp1, const std::shared_ptr<interface1>& sp2) {
    assert(sp1 == sp2);
    assert(dynamic_cast<implementation1*>(sp1.get()));
    assert(dynamic_cast<implementation1*>(sp2.get()));
  }
};

int main() {
  auto injector =
      di::make_injector(di::bind<interface1>().to<implementation1>().in(shared)  // di::unique will fail (sp1 == sp2)
                        );

  injector.create<example>();
}

//]
