#include <memory>
#include <boost/di.hpp>

namespace di = boost::di;

struct I {
  virtual ~I() noexcept = default;
  virtual void dummy() = 0;
};
struct Impl : I {
  BOOST_DI_INJECT(Impl) {}
  void dummy() override {}
};

auto module = [] { return di::make_injector(di::bind<I>.to<Impl>()); };

int main() {
  auto injector = di::make_injector(module());
  std::unique_ptr<I> object{injector.create<I *>()};
  object->dummy();
}
