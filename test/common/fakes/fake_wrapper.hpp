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
class fake_wrapper_
{
    fake_wrapper_& operator=(const fake_wrapper_&);

public:
    fake_wrapper_(const T& object) // non explicit
        : object_(object)
    { }

    template<typename I>
    I operator()(const type<I>&) const {
        return object_;
    }

private:
    T object_;
};

template<typename T>
class fake_wrapper_<const T&>
{
    fake_wrapper_& operator=(const fake_wrapper_&);

public:
    fake_wrapper_(const T& object) // non explicit
        : object_(object)
    { }

    const T& operator()(const type<const T&>&) const {
        return object_;
    }

private:
    const T& object_;
};

template<typename T>
class fake_wrapper_<T&>
{
    fake_wrapper_& operator=(const fake_wrapper_&);

public:
    fake_wrapper_(T& object) // non explicit
        : object_(object)
    { }

    T& operator()(const type<T&>&) const {
        return object_;
    }

private:
    T& object_;
};

template<typename T>
class fake_wrapper
{
public:
    fake_wrapper(const T& object) // non explicit
        : wrapper_(object)
    { }

    template<typename I>
    operator I() const {
        return wrapper_(type<I>());
    }

private:
    fake_wrapper_<T> wrapper_;
};


} // namespace di
} // namespace boost

#endif

