// clang-format off
#include <iostream>
#include <cassert>
#include <typeinfo>
#include <iostream>
#include <memory>
#include <fstream>
#include <boost/di.hpp>

namespace di = boost::di;

class reader_ {};
class printer_ {};

class FileReader {
  static constexpr auto path = "input.txt";

public:
  auto read() {
    std::ifstream file{path};
    auto value = 0;
    file >> value;
    return value;
  }
};

class ConsolePrinter {
  public:
    void print(int value) {
      std::cout << value << '\n';
    }
};

template<class TReader = reader_, class TPrinter = printer_>
class App {
  TReader reader;
  TPrinter printer;

public:
  App(TReader& reader, TPrinter& printer)
    : reader(reader), printer(printer)
  { }

  void run() {
    printer.print(reader.read());
  }
};

// --------------------------------------------------------------

template <class T>
class Reader {
public:
  int read_impl() { return {}; } // default
  int read() { return static_cast<T*>(this)->read_impl(); }
};

class FileReader2 final : public Reader<FileReader2> {
  std::string path;
public:
  int read_impl() {
    std::ifstream file{path};
    assert(file.good());
    auto value = 0;
    file >> value;
    return value;
  }

  explicit FileReader2(const std::string& path) : path(path) {
  }
};

template <class T>
class Printer {
public:
  void print_impl(int) {} // default
  void print(int value) {
    static_cast<T*>(this)->print_impl(value);
  }
};

class ConsolePrinter2 final: public Printer<ConsolePrinter2> {
public:
  void print_impl(int value) {
    std::cout << value << '\n';
  }
};

template <class TReader = Reader<di::_>, class TPrinter = Printer<di::_>>
class App2 {
  TReader& reader;
  std::shared_ptr<TPrinter> printer;

public:
  App2(TReader& reader, std::shared_ptr<TPrinter> printer)
   : reader(reader), printer(printer)
  {}

  void run() { printer->print(reader.read()); }
};


int main() {
  // clang-format off
  const auto injector = di::make_injector(
      di::bind<reader_>.to<FileReader>()
    , di::bind<printer_>.to<ConsolePrinter>()
    , di::bind<Reader<di::_>>.to<FileReader2>()
    , di::bind<Printer<di::_>>.to<ConsolePrinter2>().in(di::unique)
    , di::bind<std::string>.to("input.txt")
  );

  {
  auto app = injector.create<App>();
  app.run();
  }

  {
  auto app = injector.create<App2>();
  app.run();
  }
}
