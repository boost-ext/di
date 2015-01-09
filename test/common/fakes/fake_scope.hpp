//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_SCOPE_HPP
#define BOOST_DI_FAKE_SCOPE_HPP

namespace boost { namespace di {

struct fake_scope_entry { };
struct fake_scope_exit { };

template<bool Priority = false>
struct fake_scope {
    static constexpr auto priority = Priority;

    template<class T, class>
    struct scope {
        explicit scope(const T& = {}) { }

        void call(const fake_scope_entry&) noexcept {
            ++entry_calls();
        }

        void call(const fake_scope_exit&) noexcept {
            ++exit_calls();
        }
    };

    static int& entry_calls() {
        static auto calls = 0;
        return calls;
    }

    static int& exit_calls() {
        static auto calls = 0;
        return calls;
    }
};

}} // boost::di

#endif

