//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[custom_scope
//<-
#include <cassert>
#include <memory>
//->
#include <boost/di.hpp>

namespace di = boost::di;

struct example {
    explicit example(const std::shared_ptr<int>& sp)
        : sp_(sp)
    { }

    std::shared_ptr<int> sp_;
};

/*<define scope class>*/
class custom_scope {
public:
    static constexpr auto priority = false;

    /*<define `entry`, `exit` actions>*/
    class entry { };
    class exit { };

    template<class T, class>
    class scope {
        /*<define wrapper for shared_ptr conversion>*/
        class custom_wrapper {
        public:
            custom_wrapper(const std::shared_ptr<T>& object) // non explicit
                : object_(object)
            { }

            /*<<conversion operator to shared_ptr>>*/
            inline operator std::shared_ptr<T>() const noexcept {
                return object_;
            }

            std::shared_ptr<T> object_;
        };

    public:
        /*<<define result_type>>*/
        using result_type = custom_wrapper;

        void call(const entry&) {
            in_scope_ = true;
        }

        void call(const exit&) {
            in_scope_ = false;
        }

        /*<<create shared_ptr when in scope out of provider pointer>>*/
        template<class, class TProvider>
        custom_wrapper create(const TProvider& provider) const noexcept {
            if (in_scope_) {
                return std::shared_ptr<T>{provider.get()};
            }

            return std::shared_ptr<T>{};
        }

    private:
        bool in_scope_ = false;
    };
};

int main() {
    /*<<create injector with `int` in `custom scope`>>*/
    auto injector = di::make_injector(
        di::bind<int>.in(custom_scope{})
    );

    /*<<not in scope>>*/
    assert(!injector.create<example>().sp_);

    /*<<in scope>>*/
    injector.call(custom_scope::entry());
    assert(injector.create<example>().sp_);

    /*<<not in scope>>*/
    injector.call(custom_scope::exit());
    assert(!injector.create<example>().sp_);
}

//]

