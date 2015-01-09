//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or unique at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_UNIQUE_HPP
#define BOOST_DI_WRAPPERS_UNIQUE_HPP

#include <memory>
#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace wrappers {

template<class T>
class unique {
public:
    explicit unique(const T& value) noexcept // non explicit
        : value_(value)
    { }

    template<class I>
    inline operator I() const noexcept {
        return value_;
    }

    template<class I, class D>
    inline operator std::unique_ptr<I, D>() const noexcept { // only for compilation clean
        return {};
    }

    template<class I>
    inline operator std::shared_ptr<I>() const noexcept { // only for compilation clean
        return {};
    }

private:
    T value_;
};

template<class T, class TDeleter>
class unique<std::unique_ptr<T, TDeleter>> {
public:
    explicit unique(std::unique_ptr<T, TDeleter> value) noexcept // non explicit
        : value_(std::move(value))
    { }

    template<class I>
    inline operator I() const noexcept {
        return *value_;
    }

    template<class I>
    inline operator I*() noexcept {
        return value_.release();
    }

    template<class I>
    inline operator const I*() noexcept {
        return value_.release();
    }

    template<class I>
    inline operator std::shared_ptr<I>() noexcept {
        return {value_.release(), value_.get_deleter()};
    }

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class I>
    inline operator boost::shared_ptr<I>() noexcept {
        return {value_.release(), value_.get_deleter()};
    }
#endif

    template<class I, class D>
    inline operator std::unique_ptr<I, D>() noexcept {
        return std::move(value_);
    }

private:
    std::unique_ptr<T, TDeleter> value_;
};

}}} // boost::di::wrappers

#endif

