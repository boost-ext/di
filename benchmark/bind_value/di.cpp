#include <boost/di.hpp>

namespace di = boost::di;

auto module = [] { return di::make_injector(di::bind<>.to(42)); };

int main() {
  auto injector = di::make_injector(module());
  return injector.create<int>() != 42;
}
