//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_WRAPPER_HPP
#define BOOST_DI_FAKE_WRAPPER_HPP

#include <boost/type.hpp>

namespace boost {
namespace di {

template<typename T>
class fake_wrapper
{
    fake_wrapper& operator=(const fake_wrapper&);

public:
    fake_wrapper(const T& object) // non explicit
        : object_(object)
    { }

    template<typename I>
    I operator()(const type<I>&) const {
        return object_;
    }

    template<typename I>
    operator I() const {
        return (*this)(type<I>());
    }

private:
    T object_;
};

} // namespace di
} // namespace boost

#endif

