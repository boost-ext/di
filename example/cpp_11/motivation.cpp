//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[motivation_cpp_11
//<-
#include <memory>
//->

/*<<include di headers>>*/
#include <boost/di.hpp>

/*<<Define class as usual>>*/
class app {
public:
    app(const std::shared_ptr<int>&
      , boost::shared_ptr<int>
      , std::unique_ptr<int>
      , double
      , int
      , const std::string&)
    { }

    void start() { }
};

int main() {
    /*<<make injector>>*/
    auto injector = boost::di::make_injector();

    /*<<create app>>*/
    auto my_app = injector.create<app>();

    /*<<use my_app>>*/
    my_app.start();
    //...

    return 0;
}

//]

