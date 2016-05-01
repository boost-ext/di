#include <type_traits>

struct any_type {
  template<class T>
  operator T();
};


int main() {
  struct example {
    example(int, double);
  };

  static_assert(!std::is_constructible<example, any_type>::value);
}
