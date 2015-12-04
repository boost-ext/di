#pragma once

#include <array>
#include <vector>
#include <algorithm>
#include "boost/di.hpp"

namespace di = boost::di;

namespace msm {
struct state_base {};
struct state_base_init {};
struct type_op {};
struct anonymous {
  static constexpr auto id = -1;
  anonymous(...) {}
};
struct otherwise {
  static constexpr auto id = -2;
  otherwise(...) {}
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
struct opaque {
  opaque() noexcept = default;
  opaque(T t) noexcept : object(t) {}
  decltype(auto) get() const noexcept { return object; }

 private:
  T object;
};

template <class T>
struct opaque<T &> {
  opaque() noexcept = default;
  opaque(T &t) noexcept : object(&t) {}
  decltype(auto) get() noexcept { return *object; }

 private:
  T *object = nullptr;
};

template <class... Ts>
struct pool : opaque<Ts>... {
  pool() = default;
  explicit pool(Ts... args) noexcept : opaque<Ts>(args)... {}
};

template <>
struct pool<> {};

template <class T, class TPool>
decltype(auto) get(const TPool &p) noexcept {
  return static_cast<opaque<T>>(p).get();
}

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
  auto operator()(const TEvent &event, const pool<Ts...> &args) const noexcept {
    return reinterpret_cast<const T &> (*this)(event, get<Ts>(args)...);
  }

  template <class TEvent, class... Tx, BOOST_DI_REQUIRES(!di::aux::is_callable_with<T, TEvent, Ts...>::value) = 0>
  bool operator()(const TEvent &, const pool<Ts...> &) const noexcept {
    return typename guard_action<T>::template is_not_callable<TEvent, Ts...>{};
  }

  pool<Ts...> args_;
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

template <class...>
struct transition;

template <class S1, class S2, class E, class G, class A>
struct transition<S1, S2, E, G, A> {
  using event = E;

  transition() = default;
  transition(wrapper<G> g, wrapper<A> a) : guard(g), action(a) {}

  void init_state(std::vector<int> &current_states) const noexcept {
    const auto id = S1::id;
    if (di::aux::is_base_of<state_base_init, S1>::value &&
        std::find(current_states.begin(), current_states.end(), id) == current_states.end()) {
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

  template <class TEvent>
  void handle_event(bool &handled, int &current_state, const TEvent &event) const noexcept {
    if (!handled && current_state == S1::id && di::aux::is_base_of<TEvent, E>::value && guard(event)) {
      action(event);
      current_state = S2::id;
      handled = true;
    }
  }

 private:
  wrapper<G> guard;
  wrapper<A> action;
};

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

template <class... Ts>
struct transition {
  using type = transition;
  template <class T>
  auto operator/(const T &) const noexcept {
    return merge_transition_t<transition, get_transition_t<T>>{};
  }
};

template <class S1, class S2, class E>
struct transition<S1, S2, E> : transition<S1, S2, E, always, none> {
  using type = transition;
  template <class T>
  auto operator/(const T &) const noexcept {
    return merge_transition_t<transition, get_transition_t<T>>{};
  }
};

template <class S1, class S2, class E, class G>
struct transition<S1, S2, E, G> : transition<S1, S2, E, G, none> {
  using type = transition;
  template <class T>
  auto operator/(const T &) const noexcept {
    return merge_transition_t<transition, get_transition_t<T>>{};
  }
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
    std::array<bool, sizeof...(Ts)> a = {{get<wrapper<Ts>>(args_)(event)...}};
    return std::accumulate(a.begin(), a.end(), true, std::bit_and<>());
  }

 private:
  pool<wrapper<Ts>...> args_;
};

template <class... Ts>
struct or_ : type_op {
  or_() = default;
  explicit or_(wrapper<Ts>... ts) noexcept : args_(ts...) {}

  template <class TEvent>
  auto operator()(const TEvent &event) const noexcept {
    std::array<bool, sizeof...(Ts)> a = {{get<wrapper<Ts>>(args_)(event)...}};
    return std::accumulate(a.begin(), a.end(), false, std::bit_or<>());
  }

 private:
  pool<wrapper<Ts>...> args_;
};

template <class... Ts>
struct seq_ : type_op {
  seq_() = default;
  explicit seq_(wrapper<Ts>... ts) noexcept : args_(ts...) {}

  template <class TEvent>
  void operator()(const TEvent &event) const noexcept {
    int _[]{0, (get<wrapper<Ts>>(args_)(event), 0)...};
    (void)_;
  }

 private:
  pool<wrapper<Ts>...> args_;
};

template <class T1, class T2>
auto operator&&(const T1 &, const T2 &) noexcept {
  return and_<T1, T2>{};
}
template <class T1, class T2>
auto operator||(const T1 &, const T2 &) noexcept {
  return or_<T1, T2>{};
}
template <class T>
auto operator!(const T &)noexcept {
  return not_<T>{};
}
template <class T1, class T2, BOOST_DI_REQUIRES(di::aux::is_callable<T1>::value &&di::aux::is_callable<T2>::value) = 0>
auto operator, (const T1 &, const T2 &) noexcept {
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

template <template <int, class... Ts> class TState, int N, class... Ts>
struct state_impl<TState<N, Ts...>> {
  static constexpr auto id = N;

  template <class T>
  auto operator==(const T &) const noexcept {
    return merge_transition_t<transition<TState<N, Ts...>>, get_transition_t<T>>{};
  }
  template <class T>
  auto operator+(const T &) const noexcept {
    return merge_transition_t<transition<TState<N, Ts...>>, get_transition_t<typename T::type>>{};
  }
  template <class T>
  auto operator[](const T &) const noexcept {
    return merge_transition_t<transition<TState<N, Ts...>>, transition<anonymous>, get_transition_t<T>>{};
  }
  template <class T>
  auto operator/(const T &) const noexcept {
    return merge_transition_t<transition<TState<N, Ts...>>, transition<anonymous>, transition<always>,
                              get_transition_t<T>>{};
  }
};

template <int N, class... Ts>
struct state : state_impl<state<N, Ts...>>, state_base, Ts... {};

template <int N, class... Ts>
struct init_state : state_impl<init_state<N, Ts...>>, state_base_init, Ts... {};

template <class... Ts>
class sm : public Ts... {
  template <class T>
  struct get_event {
    using type = typename T::event;
  };

 public:
  using events = di::aux::type_list<typename get_event<Ts>::type...>;

  sm() = delete;
  explicit sm(Ts... ts) : Ts(ts)... {
    [](...) {}((static_cast<Ts &>(*this).init_state(current_states_), 0)...);
  }

  void start() noexcept { process_event_impl(anonymous{}); }

  template <class T>
  void process_event(const T &event) noexcept {
    if (!process_event_impl(event)) {
      process_event_impl(otherwise{});
    }
  }

  template <class T>
  void visit_current_states(const T &visitor) const noexcept {
    for (auto &state : current_states_) {
      auto visited = false;
      int _[]{0, (static_cast<const Ts &>(*this).visit_state(visited, state, visitor), 0)...};
      (void)_;
    }
  }

 private:
  template <class T>
  bool process_event_impl(const T &event) noexcept {
    auto handled = false;
    for (auto &state : current_states_) {
      [](...) {}((static_cast<Ts &>(*this).handle_event(handled, state, event), 0)...);
    }
    return handled;
  }

  std::vector<int> current_states_;
};

template <class... Ts>
auto make_transition_table(Ts... ts) {
  return sm<Ts...>(ts...);
}

template <class TEvent, class TConfig>
struct dispatcher {
  template <class T>
  static void dispatch_event(const TEvent &e, T &sm) noexcept {
    dispatch_event_impl(e, sm, typename T::events{});
  }

 private:
  template <class T, class E, class... Ts>
  static void dispatch_event_impl(const TEvent &event, T &sm, const di::aux::type_list<E, Ts...> &) noexcept {
    if (TConfig::template get_id<E>() == TConfig::get_id(event)) {
      sm.process_event(TConfig::template get_event<E>(event));
    }
    dispatch_event_impl(event, sm, di::aux::type_list<Ts...>{});
  }

  template <class T>
  static void dispatch_event_impl(const TEvent &, T &, const di::aux::type_list<> &) noexcept {}
};

}  // msm
