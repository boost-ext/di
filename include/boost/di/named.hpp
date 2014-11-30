//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_NAMED_HPP
#define BOOST_DI_NAMED_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace boost {
namespace di {

struct no_name { };

template<class T, class TName = no_name, class = void>
class named {
    named& operator=(const named&) = delete;

public:
    using named_type = T;
    using name = TName;

    named(const T& object = T{}) noexcept // non explicit
        : object_(object)
    { }

    named(const named& other) noexcept
        : object_(other.object_)
    { }

    operator T() const noexcept {
        return object_;
    }

private:
    T object_;
};

template<class T, class TName>
class named<const T&, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = const T&;
    using name = TName;

    named(const T& object = T{}) noexcept // non explicit
        : object_(object)
    { }

    named(const named& other) noexcept
        : object_(other.object_)
    { }

    operator const T&() const noexcept {
        return object_;
    }

private:
    std::reference_wrapper<const T> object_;
};

template<class T, class TName>
class named<T&, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = T&;
    using name = TName;

    named(T& object) noexcept // non explicit
        : object_(object)
    { }

    named(const named& other) noexcept
        : object_(other.object_)
    { }

    operator T&() noexcept {
        return object_;
    }

private:
    std::reference_wrapper<T> object_;
};

template<class T, class TName>
class named<T&&, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = T&&;
    using name = TName;

    named(T&& object) noexcept // non explicit
        : object_(std::move(object))
    { }

    named(const named& other) noexcept
        : object_(other.object_)
    { }

    operator T&&() noexcept {
        return std::move(object_);
    }

private:
    T object_;
};

template<class T, class TName>
class named<aux::unique_ptr<T>, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = aux::unique_ptr<T>;
    using name = TName;

    named(aux::unique_ptr<T> object = aux::unique_ptr<T>(new T{})) noexcept // non explicit
        : object_(std::move(object))
    { }

    operator aux::unique_ptr<T>() noexcept {
        return std::move(object_);
    }

    named(const named& other) noexcept
        : object_(new T(*other.object_))
    { }

private:
    aux::unique_ptr<T> object_;
};

template<class T, class TName>
class named<T, TName, std::enable_if_t<std::is_polymorphic<aux::remove_accessors_t<T>>{}>> {
public:
    using named_type = T;
    using name = TName;
};

} // namespace di
} // namespace boost

#endif

