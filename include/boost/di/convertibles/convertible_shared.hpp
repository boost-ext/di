//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_CONVERTIBLES_CONVERTIBLE_SHARED_HPP
#define BOOST_DI_CONVERTIBLES_CONVERTIBLE_SHARED_HPP

#include <memory>
#include <boost/shared_ptr.hpp>

#include "boost/di/named.hpp"
#include "boost/config.hpp"

namespace boost {
namespace di {
namespace convertibles {

template<typename T>
class convertible_shared
{
    struct empty_deleter { void operator()(void*) { } };

public:
    bool operator!() const {
        return object_;
    }

    void reset(T* ptr) {
        object_.reset(ptr);
    }

    template<typename I, typename TName>
    operator named<I, TName>() const {
        return object_;
    }

    template<typename I>
    operator shared_ptr<I>() const {
        return object_;
    }

    template<typename I, typename TName>
    operator named<shared_ptr<I>, TName>() const {
        return object_;
    }

    template<typename I>
    operator weak_ptr<I>() const {
        return object_;
    }

    template<typename I, typename TName>
    operator named<weak_ptr<I>, TName>() const {
        return object_;
    }

#if !defined(BOOST_NO_CXX11_SMART_PTR)
    template<typename I>
    operator std::shared_ptr<I>() const {
        return std::shared_ptr<I>(object_.get(), empty_deleter());
    }

    template<typename I, typename TName>
    operator named<std::shared_ptr<I>, TName>() const {
        return named<std::shared_ptr<I>, TName>(
            std::shared_ptr<I>(object_.get(), empty_deleter())
        );
    }

    template<typename I>
    operator std::weak_ptr<I>() const {
        return std::weak_ptr<I>(
            std::shared_ptr<I>(object_.get(), empty_deleter())
        );
    }

    template<typename I, typename TName>
    operator named<std::weak_ptr<I>, TName>() const {
        return named<std::weak_ptr<I>, TName>(
            std::weak_ptr<I>(std::shared_ptr<I>(object_.get(), empty_deleter()))
        );
    }
#endif

private:
    shared_ptr<T> object_;
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

