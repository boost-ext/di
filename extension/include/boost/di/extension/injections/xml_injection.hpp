//
// Copyright (c) 2012-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>
#include <string>
#include <typeinfo>

#include "boost/di.hpp"

BOOST_DI_NAMESPACE_BEGIN
namespace extension {

template <class...>
struct xml_list {};

struct ixml_parser {
  virtual ~ixml_parser() noexcept = default;
  virtual std::string parse(const std::string&) = 0;
};

template <class... TImpl>
class inject_from_xml {
 public:
  template <class TInjector, class T>
  auto operator()(const TInjector& injector, const T&) const -> std::shared_ptr<typename T::expected> {
    auto parser = injector.template create<std::unique_ptr<ixml_parser>>();
    auto parsed = parser->parse(typeid(typename T::expected).name());
    return create_impl<typename T::expected>(injector, parsed, xml_list<TImpl...>{});
  }

 private:
  template <class I, class TInjector, class T, class... Ts>
  std::shared_ptr<I> create_impl(const TInjector& injector, const std::string& parsed, const xml_list<T, Ts...>&) const {
    return typeid(T).name() == parsed ? injector.template create<std::shared_ptr<T>>()
                                      : create_impl<I>(injector, parsed, xml_list<Ts...>{});
  }

  template <class I, class TInjector>
  std::shared_ptr<I> create_impl(TInjector&, const std::string&, const xml_list<>&) const {
    return nullptr;
  }
};

template <class... TImpl>
struct xml : inject_from_xml<TImpl...> {};

}  // namespace extension
BOOST_DI_NAMESPACE_END
