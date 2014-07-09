//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[custom_scope_cpp_11
//````C++11```
//<-
#include <cassert>
#include <memory>
#include <boost/function.hpp>
#include <boost/type.hpp>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct example
{
    explicit example(const std::shared_ptr<int>& sp)
        : sp_(sp)
    { }

    std::shared_ptr<int> sp_;
};

class custom_scope
{
public:
    typedef boost::mpl::int_<0> priority; // lowest = 0, highest = N

    class entry { };
    class exit { };

    template<typename T>
    class scope
    {
        class wrapper
        {
        public:
            wrapper(const std::shared_ptr<T>& object) // non explicit
                : object_(object)
            { }

            const std::shared_ptr<T>&
            operator()(const boost::type<std::shared_ptr<T>>&) const {
                return object_;
            }

            std::shared_ptr<T> object_;
        };

    public:
        typedef wrapper result_type;

        void call(const entry&) {
            in_scope_ = true;
        }

        void call(const exit&) {
            in_scope_ = false;
        }

        result_type create(const boost::function<T*()>& f) {
            if (in_scope_) {
                return std::shared_ptr<T>(f());
            }

            return std::shared_ptr<T>();
        }

    private:
        bool in_scope_ = false;
    };
};

int main() {
    auto injector = di::make_injector(
        di::scope<custom_scope>::bind<int>()
    );

    assert(!injector.create<example>().sp_); // not in scope

    injector.call(custom_scope::entry());
    assert(injector.create<example>().sp_); // in scope

    injector.call(custom_scope::exit());
    assert(!injector.create<example>().sp_); // not in scope
}

//]

