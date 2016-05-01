#include <type_traits>

template<class TParent>
struct any_type {
  template<class T, class = std::enable_if_t<!std::is_convertible<TParent, T>{}>>
  operator T();
};


int main() {
  struct example {
    example(int, double);
  };

  static_assert(!std::is_constructible<example, any_type<example>>::value);
}
