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

auto module = [] {
  return di::make_injector(
      // di::bind<I>.to<Impl>()
      );
};

int main() {
  auto injector = di::make_injector(module());
  std::unique_ptr<I> object{injector.create<I *>()};
  object->dummy();
}

// di.cpp:23:38: warning: 'create<I *, 0>' is deprecated: creatable constraint
// not satisfied [-Wdeprecated-declarations]
// std::unique_ptr<I> object{injector.create<I *>()};
//^
//../../frameworks/boost.di/include/boost/di.hpp:2372:3: note: 'create<I *, 0>'
//has been explicitly marked deprecated here
// create
//^
//../../frameworks/boost.di/include/boost/di.hpp:1560:2: error: inline function
//'boost::di::v1_0_0::concepts::abstract_type<I>::is_not_bound::error' is not
//defined [-Werror,-Wundefined-inline]
// error(_ = "type is not bound, did you forget to add:
// 'di::bind<interface>.to<implementation>()'?");
//^
//../../frameworks/boost.di/include/boost/di.hpp:1556:41: note: used here
// return constraint_not_satisfied{}.error();
