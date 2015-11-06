#include <cassert>
#include <memory>
#include "boost/di.hpp"

struct i1 {
  virtual ~i1() = default;
  virtual void dummy1() = 0;
};
struct impl1 : i1 {
  void dummy1() override {}
};
struct example {
  example(std::unique_ptr<i1> up, int i) {
    assert(dynamic_cast<impl1*>(up.get()));
    assert(i == 42);
  }
};

namespace di = boost::di;

int main() {
  auto injector = di::make_injector(di::bind<i1>().to<impl1>(), di::bind<int>().to(42));

  injector.create<example>();
}
