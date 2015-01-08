//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_PROVIDER_HPP
#define BOOST_DI_FAKE_PROVIDER_HPP

namespace boost { namespace di {

template<class T>
class fake_provider {
    struct memory { };

public:
    using deleter = std::default_delete<T>;

    template<class TMemory = memory>
    T* get(const TMemory& = {}) const noexcept {
        ++provide_calls();
        return new (std::nothrow) T{};
    }

    static int& provide_calls() {
        static int calls = 0;
        return calls;
    }
};

}} // boost::di

#endif

