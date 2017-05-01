#include <iostream>
#include <boost/di.hpp>

struct a {
  a(std::string str) {
    std::cout << str << std::endl;
  }
};

struct b {
  b(std::string str) {
    std::cout << str << std::endl;
  }
};

struct c {
  c(std::string str) {
    std::cout << str << std::endl;
  }
};

namespace di = boost::di;

int main() {
  const auto injector = di::make_injector(
    di::bind<std::string>.when<a>().to("one"),
    di::bind<std::string>.when<b>().to("two"),
    di::bind<std::string>.to("3")
  );

  injector.create<a>();
  injector.create<b>();
  injector.create<c>();

}
