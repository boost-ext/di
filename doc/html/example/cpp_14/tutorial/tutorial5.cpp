//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if (__cplusplus < 201300L)
    int main() { return 0; }
#else

#include <iostream>
#include <memory>
#include <cstdlib>
#include <boost/di.hpp>

namespace di  = boost::di;

class iprinter
{
public:
    virtual ~iprinter() { }
    virtual void print(const std::string&) = 0;
};

class printer : public iprinter
{
public:
    void print(const std::string& s) override {
        std::cout << s << std::endl;
    }
};

class app
{
public:
    app(int value, const std::string& text, std::shared_ptr<iprinter> printer)
        : value_(value), text_(text), printer_(printer)
    { }

    void start() {
        if (value_)
            printer_->print(text_);
    }

private:
    int value_ = 0;
    std::string text_;
    std::shared_ptr<iprinter> printer_;
};

class module
{
public:
    explicit module(int i)
        : i_(i)
    { }

    auto configure() const {
        return di::make_injector(
            di::deduce<printer>() // we can deduce the interface as well!
          , di::bind<int>::to(i_)
          , di::bind<std::string>::to("hello world")
        );
    }

private:
    int i_ = 0;
};

int main(int argc, char** argv) {
    auto injector = di::make_injector(module(argc > 1 ? std::atoi(argv[1]) : 0));
    injector.create<app>().start();

    return 0;
}

#endif

