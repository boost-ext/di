
#include <boost/di.hpp>
#include <extension/include/boost/di/extension/policies/uml_dumper_2.hpp>
#include <extension/include/boost/di/extension/scopes/shared.hpp>
#include <cassert>
#include <iostream>

namespace di = boost::di;

namespace complex {

struct n{};
struct m{};
struct l{ l(n&){} };
struct k{ k(m&, n&){} };
struct j{ j(m&){} };
struct i{ i(m&){} };
struct h{ h(n&){} };
struct g{};
struct f{ f(l&){} };
struct e{};
struct d{ d(j&, k&){} };
struct c{ c(g&, h&, i&){} };
struct b{ b(e&, f&, g&){} };
struct a{ a(b&, l&, c&, d&){} };

auto& get_injector()
{
  // clang-format off
  static auto ject{di::make_injector<di::extension::uml_dumper>(
         di::bind<n>().in(di::extension::shared)
       , di::bind<m>().in(di::extension::shared)
       , di::bind<l>().in(di::extension::shared)
       , di::bind<k>().in(di::extension::shared)
       , di::bind<j>().in(di::extension::shared)
       , di::bind<i>().in(di::extension::shared)
       , di::bind<h>().in(di::extension::shared)
       , di::bind<g>().in(di::extension::shared)
       , di::bind<f>().in(di::extension::shared)
       , di::bind<e>().in(di::extension::shared)
       , di::bind<d>().in(di::extension::shared)
       , di::bind<c>().in(di::extension::shared)
       , di::bind<b>().in(di::extension::shared)
       , di::bind<a>().in(di::extension::shared)
      )};
      // clang-format off
  return ject;
}

} //namespace complex

namespace simple {

struct e{};
struct d{};
struct c{};
struct b{};
struct a{ a(b&, c&){} };

auto& get_injector()
{

  // clang-format off
  static auto ject = di::make_injector<di::extension::uml_dumper>(
         di::bind<e>().in(di::extension::shared)
       , di::bind<d>().in(di::extension::shared)
       , di::bind<c>().in(di::extension::shared)
       , di::bind<b>().in(di::extension::shared)
       , di::bind<a>().in(di::extension::shared)
      );
      // clang-format off
  return ject;
}

} //namespace complex

int main() {
  //using namespace simple;
  using namespace complex;

  const auto& injector{get_injector()};

  //std::cout << "@startuml uml_dumper.png" << std::endl;

  injector.create<a>();

  //std::cout << "@enduml" << std::endl;
}
