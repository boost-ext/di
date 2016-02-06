//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//<-
#include <memory>
#include <cassert>
#include <typeinfo>
#include <iostream>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
struct interface {
  virtual ~interface() noexcept = default;
};
struct implementation1 : interface {};
struct implementation2 : interface {};

struct ixml_parser {
  virtual ~ixml_parser() noexcept = default;
  virtual std::string parse(const std::string&) = 0;
};

struct xml_parser_stub : ixml_parser {
  std::string parse(const std::string&) override {
    static auto i = 0;

    switch (i++) {
      default:
        return {};
      case 0:
        return typeid(implementation1).name();
      case 1:
        return typeid(implementation2).name();
    };

    return {};
  }
};

template <class...>
struct xml_list {};

template <class... TImpl>
class inject_from_xml {
 public:
  template <class TInjector, class T>
  auto operator()(const TInjector& injector, const T&) const {
    auto parser = injector.template create<std::unique_ptr<ixml_parser>>();
    auto parsed = parser->parse(typeid(typename T::expected).name());
    return create_impl(injector, parsed, xml_list<TImpl...>{});
  }

 private:
  template <class TInjector, class T, class... Ts>
  std::shared_ptr<interface> create_impl(const TInjector& injector, const std::string& parsed,
                                         const xml_list<T, Ts...>&) const {
    return typeid(T).name() == parsed ? injector.template create<std::shared_ptr<T>>()
                                      : create_impl(injector, parsed, xml_list<Ts...>{});
  }

  template <class TInjector>
  std::shared_ptr<interface> create_impl(TInjector&, const std::string&, const xml_list<>&) const {
    return nullptr;
  }
};

template <class... TImpl>
struct xml : inject_from_xml<TImpl...> {};
//->

auto module = [] { return di::make_injector(di::bind<ixml_parser>().to<xml_parser_stub>()); };

auto xml_module = [] { return di::make_injector(di::bind<interface>().to(xml<implementation1, implementation2>())); };

int main() {
  /*<<make injector>>*/
  auto injector = di::make_injector(module(), xml_module());

  /*<<create `interface` from xml configuration>>*/
  {
    auto object = injector.create<std::shared_ptr<interface>>();
    assert(object.get());
    assert(dynamic_cast<implementation1*>(object.get()));
  }

  /*<<create `interface` from different xml configuration>>*/
  {
    auto object = injector.create<std::shared_ptr<interface>>();
    assert(object.get());
    assert(dynamic_cast<implementation2*>(object.get()));
  }

  /*<<create `interface` from different xml configuration>>*/
  {
    auto object = injector.create<std::shared_ptr<interface>>();
    assert(!object.get());
  }
}

