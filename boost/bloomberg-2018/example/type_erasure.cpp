//
// Copyright (c) 2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <fstream>
#include <iostream>

class Printable {
  void* ptr_{}; // Small Buffer Optimization (SBO)
  struct { void (*print)(void*, int);
           void*(*clone)(void*);
           void (*delete_ptr)(void*);
  } const vptr_{};

public:
  Printable(const Printable& other)
    : ptr_{other.vptr_.clone(other.ptr_)}, vptr_{other.vptr_} {}

 template<class T, class = std::enable_if_t<!std::is_convertible<T, Printable>::value>> Printable(T t)
  : ptr_{new T{t}}, vptr_{
      [](void* self, int value) { static_cast<T*>(self)->print(value); },
      [](void* self) -> void *{ return new T{*static_cast<T*>(self)}; },
      [](void* self) { delete static_cast<T*>(self); } } { }

 void print(int value) { vptr_.print(ptr_, value); }
};

class Readable {
  void* ptr_{}; // Small Buffer Optimization (SBO)
  struct { int (*read)(void*);
           void*(*clone)(void*);
           void (*delete_ptr)(void*);
  } const vptr_{};

public:
  Readable(const Readable& other)
    : ptr_{other.vptr_.clone(other.ptr_)}, vptr_{other.vptr_} {}

 template<class T, class = std::enable_if_t<!std::is_convertible<T, Readable>::value>> Readable(T t)
  : ptr_{new T{t}}, vptr_{
      [](void* self) { return static_cast<T*>(self)->read(); },
      [](void* self) -> void *{ return new T{*static_cast<T*>(self)}; },
      [](void* self) { delete static_cast<T*>(self); } } {
      }
 ~Readable() { vptr_.delete_ptr(ptr_); }

 int read() const { return vptr_.read(ptr_); }
};

class FileReader final {
  std::istream& input;

public:
  explicit FileReader(std::istream& in) : input{in} { }
  int read() { return {}; }
};

class ConsolePrinter final {
 public:
  void print(int value) {
    std::cout << value << '\n';
  }
};

class App {
  const Readable reader;
  Printable printer;

 public:
  App(const Readable reader, Printable printer)
    : reader{reader}, printer{printer} {}

  void run() { printer.print(reader.read()); }
};

int main() {
  {
    std::fstream file{"input.txt"};
    Readable reader = FileReader{file};
    Printable printer = ConsolePrinter{};
    App app{reader, printer};
    app.run();
  }

  {
    namespace di = boost::di;

    const auto injector = di::make_injector(
      di::bind<std::istream>.to<std::fstream>(/*"input.txt"*/),
      di::bind<class Readable>.to<FileReader>(),
      di::bind<class Printable>.to<ConsolePrinter>()
    );
    auto app = injector.create<App>();
    app.run();
  }
}
