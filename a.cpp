#include <boost/di.hpp>
#include <cassert>

namespace di = boost::di;

struct i {
  virtual ~i() = default;
  virtual void dummy() = 0;
};

struct impl1 : i {
  void dummy() {}
};

struct impl2 : i {
  void dummy() {}
};

struct c {
  c(i& x) {
    assert(dynamic_cast<impl1*>(&x));
  }
};

struct b {
  using inject = b(i&);

  b(i& x) {
    assert(dynamic_cast<impl2*>(&x));
  }
};

struct a {
  a(b, c) {
  }
};

int main() {
  const auto injector = di::make_injector(
     di::bind<i(c)>.to<impl1>()
   , di::bind<i(b)>.to<impl2>()
  );

  injector.create<a>();
}
