//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_FAKE_WRAPPER_HPP
#define BOOST_DI_FAKE_WRAPPER_HPP

#include <memory>
#include <boost/type.hpp>

#include "boost/di/named.hpp"

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

struct to_ref { };

template<>
class fake_wrapper_<const to_ref&>
{
    fake_wrapper_& operator=(const fake_wrapper_&);

public:
    fake_wrapper_(const to_ref& object) // non explicit
        : object_(object)
    { }

    const to_ref& operator()(const type<const to_ref&>&) const {
        return object_;
    }

private:
    const to_ref& object_;
};

struct not_to_ref { };

template<>
class fake_wrapper_<const not_to_ref&>
{
public:
    fake_wrapper_(const not_to_ref& object) // non explicit
        : object_(object)
    { }

    not_to_ref operator()(const type<not_to_ref>&) const {
        return object_;
    }

private:
    not_to_ref object_;
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
class fake_wrapper_<std::auto_ptr<T>>
{
public:
    fake_wrapper_(T* object) // non explicit
        : object_(object)
    { }

    std::auto_ptr<T> operator()(const type<std::auto_ptr<T>>&) const {
        return object_;
    }

private:
    mutable std::auto_ptr<T> object_;
};

template<typename T>
class fake_wrapper_<named<const T&>>
{
    fake_wrapper_& operator=(const fake_wrapper_&);

public:
    fake_wrapper_(const named<const T&>& object) // non explicit
        : object_(object)
    { }

    const named<const T&>& operator()(const type<const named<const T&>&>&) const {
        return object_;
    }

    const T& operator()(const type<const T&>&) const {
        return object_;
    }

private:
    const named<const T&>& object_;
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

