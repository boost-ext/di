//
// Copyright (c) 2018 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/di.hpp>
#include <fstream>
#include <iostream>

class FileReader final {
  std::istream& input;

public:
  explicit FileReader(std::istream& in) : input{in} { }
  int read() const { return {}; }
};

class ConsolePrinter final {
 public:
  void print(int value) {
    std::cout << value << '\n';
  }
};

template<typename TReader = class Readable,
         typename TPrinter = class Printable>
class App {
  const TReader& reader;
  TPrinter& printer;

public:
  App(const TReader& reader, TPrinter& printer)
    : reader{reader}, printer{printer} {}

  void run() {
    printer.print(reader.read());
  }
};

int main() {
  {
    std::fstream file{"input.txt"};
    auto reader = FileReader{file};
    auto printer = ConsolePrinter{};
    App<FileReader, ConsolePrinter> app{reader, printer};
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
