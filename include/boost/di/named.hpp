//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_NAMED_HPP
#define BOOST_DI_NAMED_HPP

#include <type_traits>

namespace boost { namespace di {

struct no_name { };

namespace detail {

template<class T, class TName = no_name, class = void>
class named_impl : public T {
public:
    using named_type = T;
    using name = TName;

    using T::T;

    named_impl(const T& object = {}) noexcept // non explicit
        : T(object)
    { }

    operator const T&() const noexcept {
        return *this;;
    }
};

template<class T, class TName>
class named_impl<T, TName, std::enable_if_t<!std::is_class<T>{}>> {
public:
    using named_type = T;
    using name = TName;

    named_impl(const T& object = {}) noexcept // non explicit
        : object_(object)
    { }

    operator T&() noexcept {
        return object_;;
    }

    operator const T&() const noexcept {
        return object_;;
    }

private:
    T object_;
};

} // namespace detail

template<class T, class TName = no_name>
class named : public detail::named_impl<T, TName> {
    using detail::named_impl<T, TName>::named_impl;
};

}} // boost::di

#endif

