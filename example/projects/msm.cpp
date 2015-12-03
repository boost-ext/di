//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[msm
//<-
#include <cassert>
#include <iostream>
#include <algorithm>
#include <memory>
#include <array>
#include <string>
#include <tuple>
#include <vector>
#include <array>
//->
#include <boost/di.hpp>

namespace di = boost::di;

namespace msm {

struct type_op {};

template <class...>
struct transition;
template <class T>
struct get_transition {
  using type = transition<T>;
};
template <class... Ts>
struct get_transition<transition<Ts...>> {
  using type = transition<Ts...>;
};

template <class T>
using get_transition_t = typename get_transition<T>::type;

struct always {
  auto operator()() const { return true; }
};
struct none {
  void operator()() const {}
};
struct anonymous {};

template <class T, class = di::aux::function_traits_t<decltype(&T::operator())>, class = int>
struct wrapper_impl;

template <class T, class... Ts>
struct wrapper_impl<T, di::aux::type_list<Ts...>, BOOST_DI_REQUIRES(!di::aux::is_base_of<type_op, T>::value)> {
  using boost_di_inject__ = di::inject<Ts...>;

  template <class... Tx>
  explicit wrapper_impl(Tx... ts) noexcept : args_(ts...) {}
  auto operator()() const noexcept { return (*this)(args_); }

 private:
  template <class... Tx>
  auto operator()(const std::tuple<Ts...> &args) const {
    return reinterpret_cast<const T &> (*this)(std::get<Ts>(args)...);
  }

  std::tuple<Ts...> args_;
};

template <class T, class X>
struct wrapper_impl<T, X, BOOST_DI_REQUIRES(di::aux::is_base_of<type_op, T>::value)> {
  wrapper_impl() = default;
  explicit wrapper_impl(T op) : op(op) {}
  auto operator()() const noexcept { return op(); }

 private:
  T op;
};

struct state_base {};
struct state_base_init {};

template <class T>
using wrapper = wrapper_impl<T>;

template <class S1, class S2, class E, class G, class A>
struct transition<S1, S2, E, G, A> {
  transition() = default;
  transition(wrapper<G> g, wrapper<A> a) : guard(g), action(a) {}

  template <class TEvent>
  void handle_event(bool &handled, int &current_state, const TEvent &) noexcept {
    if (!handled && current_state == S1::id && di::aux::is_base_of<TEvent, E>::value && guard()) {
      action();
      current_state = S2::id;
      handled = true;
    }
  }

  void init_state(std::vector<int> &current_states) {
    if (di::aux::is_base_of<state_base_init, S1>::value) {
      const auto id = S1::id;
      current_states.push_back(id);
    }
  }

 private:
  wrapper<G> guard;
  wrapper<A> action;
};

template <class...>
struct join;
template <>
struct join<> {
  using type = transition<>;
};

template <class... TArgs>
struct join<transition<TArgs...>> {
  using type = transition<TArgs...>;
};

template <class... TArgs1, class... TArgs2>
struct join<transition<TArgs1...>, transition<TArgs2...>> {
  using type = transition<TArgs1..., TArgs2...>;
};

template <class... TArgs1, class... TArgs2, class... Ts>
struct join<transition<TArgs1...>, transition<TArgs2...>, Ts...> {
  using type = typename join<transition<TArgs1..., TArgs2...>, Ts...>::type;
};

template <class... TArgs>
using join_t = typename join<TArgs...>::type;

template <class...>
struct transition {
  using type = transition;
  template <class T>
  auto operator/(T) {
    return join_t<transition, get_transition_t<T>>{};
  }
};

template <class T>
struct not_ : type_op {
  not_() = default;
  explicit not_(wrapper<T> t) noexcept : t_(t) {}
  auto operator()() const { return !t_(); }

 private:
  wrapper<T> t_;
};

template <class... Ts>
struct and_ : type_op {
  and_() = default;
  explicit and_(wrapper<Ts>... ts) noexcept : args_(ts...) {}

  auto operator()() const {
    std::array<bool, sizeof...(Ts)> a = {{std::get<wrapper<Ts>>(args_)()...}};
    return std::accumulate(a.begin(), a.end(), true, std::bit_and<>());
  }

 private:
  std::tuple<wrapper<Ts>...> args_;
};

template <class... Ts>
struct or_ : type_op {
  or_() = default;
  explicit or_(wrapper<Ts>... ts) noexcept : args_(ts...) {}
  auto operator()() const {
    std::array<bool, sizeof...(Ts)> a = {{std::get<wrapper<Ts>>(args_)()...}};
    return std::accumulate(a.begin(), a.end(), false, std::bit_or<>());
  }

 private:
  std::tuple<wrapper<Ts>...> args_;
};

template <class... Ts>
struct seq_ : type_op {
  seq_() = default;
  explicit seq_(wrapper<Ts>... ts) noexcept : args_(ts...) {}
  void operator()() const {
    [](...) {}((std::get<wrapper<Ts>>(args_)(), 0)...);  // todo order
  }

 private:
  std::tuple<wrapper<Ts>...> args_;
};

template <class T1, class T2>
auto operator&&(T1, T2) {
  return and_<T1, T2>{};
}
template <class T1, class T2>
auto operator||(T1, T2) {
  return or_<T1, T2>{};
}
template <class T>
auto operator!(T) {
  return not_<T>{};
}

template <class E>
struct event_ {
  using type = E;
  template <class T>
  auto operator[](T) {
    return transition<E, T>{};
  }
  template <class T>
  auto operator/(T) {
    return transition<E, always, T>{};
  }
};

template <class T1, class T2>
auto operator, (T1, T2) {
  return seq_<T1, T2>{};
}

template <class T>
event_<T> event{};

template <int N>
struct state : state_base {
  static constexpr auto id = N;
  template <class T>
  auto operator==(T) {
    return join_t<transition<state>, get_transition_t<T>>{};
  }
  template <class T>
  auto operator+(T) {
    return join_t<transition<state>, get_transition_t<typename T::type>>{};
  }
  template <class T>
  auto operator[](T) {
    return join_t<transition<state>, transition<anonymous>, get_transition_t<T>>{};
  }
  template <class T>
  auto operator/(T) {
    return join_t<transition<state>, transition<anonymous>, transition<always>, get_transition_t<T>>{};
  }
};

template <int N>
struct init_state : state_base_init {
  static constexpr auto id = N;
  template <class T>
  auto operator==(T) {
    return join_t<transition<init_state>, get_transition_t<T>>{};
  }
  template <class T>
  auto operator+(T) {
    return join_t<transition<init_state>, get_transition_t<typename T::type>>{};
  }
  template <class T>
  auto operator[](T) {
    return join_t<transition<init_state>, transition<anonymous>, get_transition_t<T>>{};
  }
  template <class T>
  auto operator/(T) {
    return join_t<transition<init_state>, transition<anonymous>, transition<always>, get_transition_t<T>>{};
  }
};

struct imsm {
  virtual ~imsm() noexcept = default;
  virtual void process_event(void *) noexcept = 0;
};

struct config {
  static void no_transition() noexcept {}
  static auto get_event(void *) noexcept { return 42; }
};

template <class TConfig, class... Ts>
class sm : public imsm, Ts... {
 public:
  explicit sm(Ts... ts) : Ts(ts)... {
    [](...) {}((static_cast<Ts &>(*this).init_state(current_states_), 0)...);
  }

  void process_event(void *) noexcept override {}
  template <class T>
  void process_event(const T &event) noexcept {
    bool handled = false;
    for (auto &state : current_states_) {
      [](...) {}((static_cast<Ts &>(*this).handle_event(handled, state, event), 0)...);
    }

    if (!handled) {
      TConfig::no_transition();
    }
  }

  // todo remove
  auto get_current_state() const { return current_states_[0]; }

 private:
  std::vector<int> current_states_;
};

template <class TConfig = config, class... Ts>
auto make_transition_table(Ts... ts) {
  return sm<TConfig, Ts...>(ts...);
}

}  // msm

struct e1 {};
struct e2 {};
struct e3 {};

auto guard = [](int i, double) {
  std::cout << "guard: " << i << std::endl;
  return true;
};

auto action = [] { std::cout << "action" << std::endl; };

struct config {
  static void no_transition() noexcept { std::cout << "no transition" << std::endl; }
  static auto get_event(void *) noexcept {}
};

template <class T>
auto controller() {
  using namespace msm;
  auto idle = init_state<__COUNTER__>{};
  auto s1 = state<__COUNTER__>{};
  auto s2 = state<__COUNTER__>{};

  // clang-format off
  return make_transition_table<T>(
   // +-----------------------------------------------------------------+
      idle == s1 + event<e1> [guard and guard] / (action, action, action)
    , s1   == s2 + event<e2> [guard] / action
   // +-----------------------------------------------------------------+
  );
  // clang-format on
}

int main() {
  auto injector = di::make_injector(di::bind<int>().to(42));
  auto sm = injector.create<decltype(controller<config>())>();

  std::cout << sm.get_current_state() << std::endl;
  sm.process_event(e1{});
  std::cout << sm.get_current_state() << std::endl;
  sm.process_event(e2{});
  std::cout << sm.get_current_state() << std::endl;
  sm.process_event(e3{});
  std::cout << sm.get_current_state() << std::endl;

  assert(false);
}

//]
