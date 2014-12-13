//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_NAMED_HPP
#define BOOST_DI_NAMED_HPP

#include <memory>

namespace boost { namespace di {

struct no_name { };

template<class T, class TName = no_name>
class named {
    named& operator=(const named&) = delete;

public:
    using named_type = T;
    using name = TName;

    named(const T& object = {}) noexcept // non explicit
        : object_(object)
    { }

    named(const named&) = default;

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

    named(const T& object) noexcept // non explicit
        : object_(object)
    { }

    operator const T&() const noexcept {
        return object_;
    }

private:
    //std::reference_wrapper<const T> object_;
    T object_;
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

    operator T&() noexcept {
        return object_;
    }

private:
    std::reference_wrapper<T> object_;
};

template<class T, class TName>
class named<T&&, TName> {
    named& operator=(const named&) = delete;
    named(const named&) noexcept = delete;

public:
    using named_type = T&&;
    using name = TName;

    named(T&& object) noexcept // non explicit
        : object_(std::move(object))
    { }

    operator T&&() noexcept {
        return std::move(object_);
    }

private:
    T object_;
};

template<class T, class TName>
class named<std::unique_ptr<T>, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = std::unique_ptr<T>;
    using name = TName;

    named(std::unique_ptr<T> object) noexcept // non explicit
        : object_(std::move(object))
    { }

    operator std::unique_ptr<T>() noexcept {
        return std::move(object_);
    }

private:
    std::unique_ptr<T> object_;
};

}} // boost::di

#endif

