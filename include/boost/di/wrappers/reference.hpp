//
//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_REFERENCE_HPP
#define BOOST_DI_WRAPPERS_REFERENCE_HPP

#include "boost/di/aux_/utility.hpp"

namespace boost { namespace di { namespace wrappers {

template<class T>
class reference {
    reference& operator=(const reference&) = delete;

public:
    reference(T& value) noexcept // non explicit
        : value_(value)
    { }

    inline T& operator()(const aux::type<T&>&) const noexcept {
        return value_;
    }

private:
    std::reference_wrapper<T> value_;
};

}}} // namespace boost::di::wrappers

#endif

