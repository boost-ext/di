//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

#include <utility>
#include <boost/type.hpp>
#include <boost/function.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost {
namespace di {

struct fake_scope_entry { };
struct fake_scope_exit { };

template<bool Priority = false>
struct fake_scope
{
    static constexpr bool priority = Priority;

    template<typename T>
    class wrapper {
    public:
        wrapper(const aux::shared_ptr<T>& obj) // non explicit
            : obj_(obj)
        { }

        template<typename I>
        aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I>>&) const {
            return obj_;
        }

        T operator()(const type<T>&) const {
            if (!obj_) {
                return T();
            }
            return *obj_;
        }

    private:
        aux::shared_ptr<T> obj_;
    };

    template<typename T>
    class scope {
    public:
        typedef scope type;
        typedef wrapper<T> result_type;

        scope() { }

        explicit scope(const T&) { }

        void call(const fake_scope_entry&) {
            ++entry_calls();
        }

        void call(const fake_scope_exit&) {
            ++exit_calls();
        }

        result_type create(const function<T*()>& f) {
            if (entry_calls() > exit_calls()) {
                return aux::shared_ptr<T>(f());
            }

            return result_type(aux::shared_ptr<T>());
        }
    };

    static int& entry_calls() {
        static int calls = 0;
        return calls;
    }

    static int& exit_calls() {
        static int calls = 0;
        return calls;
    }
};

} // namespace di
} // namespace boost

#endif

