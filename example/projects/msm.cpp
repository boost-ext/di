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
struct anonymous {};
struct state_base {};
struct state_base_init {};
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

template <class T>
auto args__(int) -> di::aux::function_traits_t<decltype(&T::template operator() < anonymous > )>;

template <class T>
auto args__(int) -> di::aux::function_traits_t<decltype(&T::operator())>;

template <class T>
di::aux::type_list<> args__(...);

template <class T, class = decltype(args__<T>(0)), class = int>
struct wrapper_impl;

template <class T>
struct guard_action {
  template <class...>
  struct is_not_callable {
    operator bool() const {
      using constraint_not_satisfied = is_not_callable;
      return constraint_not_satisfied{}.error();
    }

    // clang-format off
    static inline bool
	error(di::_ = "guard/action is not callable with required parameters");
    // clang-format on
  };
};

template <class T, class X, class... Ts>
struct wrapper_impl<T, di::aux::type_list<X, Ts...>, BOOST_DI_REQUIRES(!di::aux::is_base_of<type_op, T>::value)> {
  using boost_di_inject__ = di::inject<Ts...>;

  template <class... Tx>
  explicit wrapper_impl(Tx... ts) noexcept : args_(ts...) {}

  template <class TEvent>
  auto operator()(const TEvent &event) const noexcept {
    return (*this)(event, args_);
  }

 private:
  template <class TEvent, class... Tx, BOOST_DI_REQUIRES(di::aux::is_callable_with<T, TEvent, Ts...>::value) = 0>
  auto operator()(const TEvent &event, const std::tuple<Ts...> &args) const noexcept {
    return reinterpret_cast<const T &> (*this)(event, std::get<Ts>(args)...);
  }

  template <class TEvent, class... Tx, BOOST_DI_REQUIRES(!di::aux::is_callable_with<T, TEvent, Ts...>::value) = 0>
  bool operator()(const TEvent &, const std::tuple<Ts...> &) const noexcept {
    return typename guard_action<T>::template is_not_callable<TEvent, Ts...>{};
  }

  std::tuple<Ts...> args_;
};

template <class T, class X>
struct wrapper_impl<T, X, BOOST_DI_REQUIRES(di::aux::is_base_of<type_op, T>::value)> {
  wrapper_impl() = default;
  explicit wrapper_impl(T op) : op(op) {}
  template <class TEvent>
  auto operator()(const TEvent &event) const noexcept {
    return op(event);
  }

 private:
  T op;
};

template <class T>
using wrapper = wrapper_impl<T>;

template <class S1, class S2, class E, class G, class A>
struct transition<S1, S2, E, G, A> {
  transition() = default;
  transition(wrapper<G> g, wrapper<A> a) : guard(g), action(a) {}

  template <class TEvent>
  void handle_event(bool &handled, int &current_state, const TEvent &event) const noexcept {
    if (!handled && current_state == S1::id && di::aux::is_base_of<TEvent, E>::value && guard(event)) {
      action(event);
      current_state = S2::id;
      handled = true;
    }
  }

  void init_state(std::vector<int> &current_states) const noexcept {
    if (di::aux::is_base_of<state_base_init, S1>::value) {
      const auto id = S1::id;
      current_states.push_back(id);
    }
  }

  template <class TVisitor>
  void visit_state(bool &visited, int state, const TVisitor &visitor) const noexcept {
    if (!visited && S1::id == state) {
      visitor(S1{});
      visited = true;
    }

    if (!visited && S2::id == state) {
      visitor(S2{});
      visited = true;
    }
  }

 private:
  wrapper<G> guard;
  wrapper<A> action;
};

template <class...>
struct merge_transition;
template <>
struct merge_transition<> {
  using type = transition<>;
};

template <class... TArgs>
struct merge_transition<transition<TArgs...>> {
  using type = transition<TArgs...>;
};

template <class... TArgs1, class... TArgs2>
struct merge_transition<transition<TArgs1...>, transition<TArgs2...>> {
  using type = transition<TArgs1..., TArgs2...>;
};

template <class... TArgs1, class... TArgs2, class... Ts>
struct merge_transition<transition<TArgs1...>, transition<TArgs2...>, Ts...> {
  using type = typename merge_transition<transition<TArgs1..., TArgs2...>, Ts...>::type;
};

template <class... TArgs>
using merge_transition_t = typename merge_transition<TArgs...>::type;

template <class...>
struct transition {
  using type = transition;
  template <class T>
  auto operator/(const T &) const noexcept {
    return merge_transition_t<transition, get_transition_t<T>>{};
  }
};

struct always {
  template <class TEvent>
  auto operator()(const TEvent &) const noexcept {
    return true;
  }
};
struct none {
  template <class TEvent>
  void operator()(const TEvent &) const noexcept {}
};

template <class T>
struct not_ : type_op {
  not_() = default;
  explicit not_(wrapper<T> t) noexcept : t_(t) {}
  template <class TEvent>
  auto operator()(const TEvent &event) const noexcept {
    return !t_(event);
  }

 private:
  wrapper<T> t_;
};

template <class... Ts>
struct and_ : type_op {
  and_() = default;
  explicit and_(wrapper<Ts>... ts) noexcept : args_(ts...) {}

  template <class TEvent>
  auto operator()(const TEvent &event) const noexcept {
    std::array<bool, sizeof...(Ts)> a = {{std::get<wrapper<Ts>>(args_)(event)...}};
    return std::accumulate(a.begin(), a.end(), true, std::bit_and<>());
  }

 private:
  std::tuple<wrapper<Ts>...> args_;
};

template <class... Ts>
struct or_ : type_op {
  or_() = default;
  explicit or_(wrapper<Ts>... ts) noexcept : args_(ts...) {}

  template <class TEvent>
  auto operator()(const TEvent &event) const noexcept {
    std::array<bool, sizeof...(Ts)> a = {{std::get<wrapper<Ts>>(args_)(event)...}};
    return std::accumulate(a.begin(), a.end(), false, std::bit_or<>());
  }

 private:
  std::tuple<wrapper<Ts>...> args_;
};

template <class... Ts>
struct seq_ : type_op {
  seq_() = default;
  explicit seq_(wrapper<Ts>... ts) noexcept : args_(ts...) {}

  template <class TEvent>
  void operator()(const TEvent &event) const noexcept {
    int _[]{0, (std::get<wrapper<Ts>>(args_)(event), 0)...};
    (void)_;
  }

 private:
  std::tuple<wrapper<Ts>...> args_;
};

template <class T1, class T2>
auto operator&&(T1, T2) noexcept {
  return and_<T1, T2>{};
}
template <class T1, class T2>
auto operator||(T1, T2) noexcept {
  return or_<T1, T2>{};
}
template <class T>
auto operator!(const T &)noexcept {
  return not_<T>{};
}
template <class T1, class T2>
auto operator, (T1, T2) noexcept {
  return seq_<T1, T2>{};
}

template <class E>
struct event_impl {
  using type = E;
  template <class T>
  auto operator[](const T &) const noexcept {
    return transition<E, T>{};
  }
  template <class T>
  auto operator/(const T &) const noexcept {
    return transition<E, always, T>{};
  }
#if defined(__cpp_variable_templates)
  auto operator()() const noexcept { return *this; }
#endif
};

#if defined(__cpp_variable_templates)
template <class T>
event_impl<T> event{};
#else
template <class T>
struct event : event_impl<T> {};
#endif

template <class>
struct state_impl;

template <template <int> class TState, int N>
struct state_impl<TState<N>> {
  static constexpr auto id = N;

  template <class T>
  auto operator==(const T &) const noexcept {
    return merge_transition_t<transition<TState<N>>, get_transition_t<T>>{};
  }
  template <class T>
  auto operator+(const T &) const noexcept {
    return merge_transition_t<transition<TState<N>>, get_transition_t<typename T::type>>{};
  }
  template <class T>
  auto operator[](const T &) const noexcept {
    return merge_transition_t<transition<TState<N>>, transition<anonymous>, get_transition_t<T>>{};
  }
  template <class T>
  auto operator/(const T &) const noexcept {
    return merge_transition_t<transition<TState<N>>, transition<anonymous>, transition<always>, get_transition_t<T>>{};
  }
};

template <int N>
struct state : state_impl<state<N>>, state_base {};

template <int N>
struct init_state : state_impl<init_state<N>>, state_base_init {};

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

  template <class T>
  void visit_current_states(const T &visitor) noexcept {
    for (auto &state : current_states_) {
      bool visited = false;
      int _[]{0, (static_cast<Ts &>(*this).visit_state(visited, state, visitor), 0)...};
      (void)_;
    }
  }

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

auto guard = [](auto, int i) {
  assert(42 == i);
  return true;
};

auto action = [](auto) {};

struct config {
  static void no_transition() noexcept {}
  static auto get_event(void *) noexcept {}
};

template <class TConfig>
auto controller() {
  using namespace msm;
  auto idle = init_state<__COUNTER__>{};
  auto idle2 = init_state<__COUNTER__>{};
  auto s1 = state<__COUNTER__>{};
  auto s2 = state<__COUNTER__>{};

  // clang-format off
  return make_transition_table<TConfig>(
   // +-----------------------------------------------------------------+
      idle    == s1 + event<e1> [guard and guard] / (action, action, action)
   // +-----------------------------------------------------------------+
	, idle2   == s2 + event<e2> [guard] / action
   // +-----------------------------------------------------------------+
  );
  // clang-format on
}

int main() {
  auto injector = di::make_injector(di::bind<int>().to(42));
  auto sm = injector.create<decltype(controller<config>())>();

  sm.visit_current_states([](auto s) { std::cout << "\t" << typeid(s).name() << std::endl; });
  sm.process_event(e2{});
  sm.visit_current_states([](auto s) { std::cout << "\t" << typeid(s).name() << std::endl; });
  sm.process_event(e2{});
  sm.visit_current_states([](auto s) { std::cout << "\t" << typeid(s).name() << std::endl; });
  sm.process_event(e3{});
  sm.visit_current_states([](auto s) { std::cout << "\t" << typeid(s).name() << std::endl; });
}

//]
