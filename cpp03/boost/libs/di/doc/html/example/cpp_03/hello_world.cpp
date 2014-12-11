//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[hello_world_cpp_03]
//````C++98/03```
//<-
#include <cassert>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/typeof/typeof.hpp>
//->
#include <boost/di.hpp>

namespace di = boost::di;
//<-
class ilogger {
public:
    virtual ~ilogger() { }
    virtual void log(const std::string&) = 0;
};

class logger : public ilogger {
public:
    virtual void log(const std::string& text) {
        std::clog << text << std::endl;
    }
};

class ilogic {
public:
    virtual ~ilogic() { }
    virtual void do_it() = 0;
};

class logic : public ilogic {
public:
    virtual void do_it() { }
};
//->

/*<define `hello_world` class as usual>*/
class hello_world {
public:
    hello_world(boost::shared_ptr<ilogger> logger
              , boost::shared_ptr<ilogic> logic
              , bool value)
        : logger_(logger)
        , logic_(logic)
        , value_(value)
    { }

    int run() const {
        if (value_) {
            logger_->log("hello world");
            logic_->do_it();
        }
        return 0;
    }

    boost::shared_ptr<ilogger> logger_;
    boost::shared_ptr<ilogic> logic_;
    bool value_;
};

int main() {
    /*<<make injector configuration>>*/
    BOOST_AUTO(injector, (
        di::make_injector(
            di::bind<ilogger, logger>() // or di::deduce<logger>()
          , di::bind<ilogic, logic>()   // or di::deduce<logic>()
          , di::bind<bool>::to(true)    // or di::bind_bool<true>()
        )
    ));

    /*<<create `hello_world` and run [pre
        hello world
    ]>>*/
    return injector.create<hello_world>().run();
}

//]

