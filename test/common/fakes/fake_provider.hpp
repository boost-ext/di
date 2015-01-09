//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_PROVIDER_HPP
#define BOOST_DI_FAKE_PROVIDER_HPP

#include <new>
#include <memory>
#include "boost/di/type_traits/memory_traits.hpp"

namespace boost { namespace di {

template<class T>
class fake_provider {
public:
    using deleter = std::default_delete<T>;

    T get(const type_traits::stack&) const noexcept {
        ++provide_calls();
        return T{};
    }

    std::unique_ptr<T> get(const type_traits::unique&) const noexcept {
        ++provide_calls();
        return std::make_unique<T>();
    }

    std::shared_ptr<T> get(const type_traits::shared&) const noexcept {
        ++provide_calls();
        return std::make_shared<T>();
    }

    static int& provide_calls() {
        static int calls = 0;
        return calls;
    }
};

}} // boost::di

#endif

