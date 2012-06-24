//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct c1 { };

struct c2
{
    BOOST_DI_CTOR(c2, boost::shared_ptr<c1> ptr) {
        if (ptr) {
            std::clog << "in session scope" << std::endl;
        } else {
            std::clog << "not in session scope" << std::endl;
        }
    }
};

} // namespace

class session_scope
{
public:
    class entry { };
    class exit { };

    template<typename T>
    class scope
    {
    public:
        scope()
            : in_scope_(false) {
        }

        void call(const entry&) {
            in_scope_ = true;
        }

        void call(const exit&) {
            in_scope_ = false;
        }

        template<typename... Args>
        boost::shared_ptr<T> create(Args&&... args) {
            if (in_scope_) {
                return boost::make_shared<T>(std::forward(args)...);
            }
            return boost::shared_ptr<T>();
        }

    private:
        bool in_scope_;
    };
};

int main()
{
    typedef di::generic_module<
        di::scope<session_scope>::bind<
            c1
        >
    > module;

    di::injector<module> injector;

    injector.create<c2>(); // not in session scope
    injector.call<session_scope>(session_scope::entry());
    injector.create<c2>(); // in session scope
    injector.call<session_scope>(session_scope::exit());
    injector.create<c2>(); // not in session scope
}

