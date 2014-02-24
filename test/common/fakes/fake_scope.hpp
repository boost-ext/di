//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

#include <utility>
#include <boost/type.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include "boost/di/aux_/memory.hpp"
#include "boost/di/type_traits/ctor_traits.hpp"

namespace boost {
namespace di {

struct fake_scope_entry { };
struct fake_scope_exit { };

template<int Priority = 0>
struct fake_scope
{
    typedef mpl::int_<Priority> priority;

    template<typename T>
    class convertible
    {
    public:
        convertible(aux::shared_ptr<T> obj) // non explicit
            : obj_(obj)
        { }

        template<typename I>
        aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I>>&) const {
            return obj_;
        }

        template<typename I>
        I operator()(const type<I>&) const {
            if (!obj_) {
                return I();
            }
            return *obj_;
        }

        template<typename I>
        operator I() const {
            return (*this)(type<I>());
        }

    private:
        aux::shared_ptr<T> obj_;
    };

    template<typename T, typename>
    struct scope
    {
        typedef convertible<T> result_type;

        void call(const fake_scope_entry&) {
            entry_calls()++;
        }

        void call(const fake_scope_exit&) {
            exit_calls()++;
        }

        template<typename TPolicy, typename... Args>
        result_type create(const TPolicy&, Args&&... args) {
            if (entry_calls() > exit_calls()) {
                return aux::shared_ptr<T>(new T(std::forward<Args>(args)...));
            }

            return result_type(aux::shared_ptr<T>());
        }
    };

    static int& entry_calls() {
        static int calls;
        return calls;
    }

    static int& exit_calls() {
        static int calls;
        return calls;
    }
};

} // namespace di
} // namespace boost

#endif

