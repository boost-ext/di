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

/*<<define class as usual>>*/
class app {
public:
    app(std::unique_ptr<ilogger>);
    void start();
};

int main() {
    /*<<dependency injection configuration>>*/
    auto injector = di::make_injector(
        di::bind<ilogger, logger>()
    );

    /*<<composition root>>*/
    auto my_app = injector.create<app>();

    return 0;
}

//]

