//
// Copyright (c) 2012-2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//<-
#include <cassert>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
//->
#include <boost/di.hpp>

namespace di = boost::di;

//<-
template <class T>
auto get_type() {
#if defined(_MSC_VER)
  auto type = std::string{&__FUNCSIG__[28]};
  const auto i = type[0] == ' ' ? 1 : 0;
  return type.substr(i, type.length() - 7 - i);
#elif defined(__clang__)
  auto type = std::string{&__PRETTY_FUNCTION__[21]};
  return type.substr(0, type.length() - 1);
#elif defined(__GCC__)
  auto type = std::string{&__PRETTY_FUNCTION__[26]};
  return type.substr(0, type.length() - 1);
#else
#error "Platform not supported!"
#endif
}

struct archive {
  std::string path;
  std::function<char*()> ptr;
  std::string type;
  int offset = 0;
};
struct context : std::vector<archive> {
  int offset = 0;
};
struct serializable : std::vector<archive> {};

struct policy_guard {
  policy_guard() { check() = true; }
  ~policy_guard() { check() = false; }
  static bool& check() {
    static auto guard = false;
    return guard;
  }
};

template <class TGiven, class TInjector>
std::function<char*()> get_ptr(const TInjector&, std::false_type) {
  return {};
}

template <class TGiven, class TInjector>
std::function<char*()> get_ptr(const TInjector& injector, std::true_type) {
  return [&injector] {
    policy_guard _;
    return reinterpret_cast<char*>(&injector.template create<TGiven&>());
  };
}

class serializable_policy : public di::config {
 public:
  template <class TInjector>
  static auto policies(TInjector* injector) noexcept {
    return di::make_policies([&](auto type) {
      if (policy_guard::check()) return;
      policy_guard _;
      using T = decltype(type);
      using given = di::aux::decay_t<typename decltype(type)::type>;
      auto&& v = injector->template create<context&>();
      auto ptr = get_ptr<given>(*injector, std::is_reference<typename T::type>{});
      if (std::is_pod<typename T::type>::value && T::arity::value) {
        ptr = v.back().ptr;
      }

      std::string element;
      if (!v.empty()) {
        element = v.back().path + "->";
        if (v.back().ptr) {
          auto& serialize = injector->template create<serializable&>();
          if (!T::arity::value) {
            const auto align = alignof(given);
            v.offset = ((v.offset + align - 1) / align) * align;
            serialize.push_back(archive{v.back().path, v.back().ptr, get_type<given>(), v.offset});
            v.offset += sizeof(given);
          }
        }
        v.pop_back();
        if (v.empty()) {
          v.offset = {};
        }
      }

      auto ctor_size = T::arity::value;
      while (ctor_size--) {
        v.push_back(archive{element + get_type<given>(), ptr, {}, 0});
      }
    });
  }
};

template <class... Ts>
struct serializable_call {
  template <class TInjector, class TExpr>
  static void apply(const TInjector& injector, TExpr expr) {
    for (auto&& o : injector.template create<serializable&>()) {
      apply_impl(o, expr, di::aux::type_list<Ts...>{});
    }
  }

  template <class TExpr, class T, class... Us>
  static void apply_impl(const archive& o, TExpr expr, di::aux::type_list<T, Us...>) {
    if (get_type<T>() == o.type) {
      expr(o, T{});
    } else {
      apply_impl(o, expr, di::aux::type_list<Us...>{});
    }
  }

  template <class TExpr>
  static void apply_impl(const archive&, TExpr, di::aux::type_list<>) {}
};

// clang-format off
using serializable_call_t = serializable_call<
  unsigned char
, unsigned short
, unsigned int
, unsigned long
, unsigned long long
, signed char
, short
, int
, long
, long long
, char
, float
, double
, long double
, bool
>;
// clang-format on

auto serialize = [](const auto& injector, auto& str) {
  serializable_call_t::apply(injector, [&](const auto& o, auto t) {
    str << o.path << ", " << o.type << ", " << o.offset << ", "
        << std::to_string(*reinterpret_cast<decltype(t)*>(o.ptr() + o.offset)) << std::endl;
  });
};

auto deserialize = [](const auto& injector, auto& str) {
  auto split = [](auto&& txt) {
    std::stringstream s;
    s << txt;
    std::vector<std::string> tokens;
    for (std::string line; std::getline(s, line, ',');) {
      tokens.push_back(line);
    }
    return tokens;
  };

  serializable_call_t::apply(injector, [&](const auto& o, auto t) {
    for (std::string line; std::getline(str, line);) {
      if (o.type == get_type<decltype(t)>()) {
        auto&& v = split(line);
        *reinterpret_cast<decltype(t)*>(o.ptr() + std::atoi(v[2].c_str())) = std::atof(v[3].c_str());
        break;
      }
    }
  });
};
//->

struct data {
  unsigned int ui;
  long l;
  float f;
};

struct even_more_data {
  double d;
  bool b;
  long long ll;
};

struct more_data {
  int i;
  char c;
  even_more_data d;
  short s;
};

struct example {
  example(data& d, more_data& md) : d(d), md(md) {}

  void update() {
    d.ui = 13;
    d.l = 23l;
    d.f = .33f;
    md.i = 44;
    md.c = 'c';
    md.d.d = 55.0;
    md.d.b = true;
    md.d.ll = 66ll;
    md.s = 77;
  }

  void reset() {
    d.ui = {};
    d.l = {};
    d.f = {};
    md.i = {};
    md.c = {};
    md.d.d = {};
    md.d.b = {};
    md.d.ll = {};
    md.s = {};
  }

  data& d;
  more_data& md;
};

int main() {
  // clang-format off
  auto injector = di::make_injector<serializable_policy>(
      di::bind<>().to(42)
  );
  // clang-format on

  /*<<create `example`>>*/
  auto object = injector.create<example>();
  {
    assert(0 == object.d.ui);
    assert(0 == object.d.l);
    assert(.0f == object.d.f);
    assert(42 == object.md.i);  // bound
    assert(char{} == object.md.c);
    assert(0.0 == object.md.d.d);
    assert(false == object.md.d.b);
    assert(0 == object.md.d.ll);
    assert(0 == object.md.s);
  }

  /*<<update internal data>>*/
  object.update();
  {
    assert(13 == object.d.ui);
    assert(23 == object.d.l);
    assert(.33f == object.d.f);
    assert(44 == object.md.i);
    assert('c' == object.md.c);
    assert(55.0 == object.md.d.d);
    assert(true == object.md.d.b);
    assert(66 == object.md.d.ll);
    assert(77 == object.md.s);
  }

  /*<<serialize internal data to the stream>>*/
  std::stringstream str;
  serialize(injector, str);

  /*<<reset internal data>>*/
  object.reset();

  /*<<deserialize from the stream>>*/
  deserialize(injector, str);
  {
    assert(13 == object.d.ui);
    assert(23 == object.d.l);
    assert(.33f == object.d.f);
    assert(44 == object.md.i);
    assert('c' == object.md.c);
    assert(55.0 == object.md.d.d);
    assert(true == object.md.d.b);
    assert(66 == object.md.d.ll);
    assert(77 == object.md.s);
  }
}
