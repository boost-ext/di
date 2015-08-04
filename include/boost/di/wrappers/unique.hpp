//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or unique at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_UNIQUE_HPP
#define BOOST_DI_WRAPPERS_UNIQUE_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace wrappers {

template<class T>
class unique {
public:
    explicit unique(const T& object)
        : object(object)
    { }

    template<class I>
    inline operator I() const noexcept {
        return object;
    }

    inline operator T&&() noexcept {
        return static_cast<T&&>(object);
    }

private:
    T object;
};

template<class T>
class unique<T*> {
public:
    explicit unique(T* object)
        : object(object)
    { }

    template<class I>
    inline operator I() const noexcept {
        struct scoped_ptr { T* ptr; ~scoped_ptr() { delete ptr; } };
        return *scoped_ptr{object}.ptr;
    }

    template<class I>
    inline operator I*() const noexcept {
        return object; // ownership transfer
    }

    template<class I>
    inline operator const I*() const noexcept {
        return object; // ownership transfer
    }

    template<class I>
    inline operator std::shared_ptr<I>() const noexcept {
        return std::shared_ptr<I>{object};
    }

    template<class I>
    inline operator boost::shared_ptr<I>() const noexcept {
        return boost::shared_ptr<I>{object};
    }

    template<class I, class D>
    inline operator std::unique_ptr<I, D>() const noexcept {
        return std::unique_ptr<I, D>{object};
    }

private:
    T* object = nullptr;
};

template<class T, class TDeleter>
class unique<std::unique_ptr<T, TDeleter>> {
public:
    explicit unique(std::unique_ptr<T, TDeleter> object)
        : object(static_cast<std::unique_ptr<T, TDeleter>&&>(object))
    { }

    template<class I>
    inline operator I() const noexcept {
        return *object;
    }

    template<class I>
    inline operator I*() noexcept {
        return object.release();
    }

    template<class I>
    inline operator const I*() noexcept {
        return object.release();
    }

    template<class I>
    inline operator std::shared_ptr<I>() noexcept {
        return {object.release(), object.get_deleter()};
    }

    template<class I>
    inline operator boost::shared_ptr<I>() noexcept {
        return {object.release(), object.get_deleter()};
    }

    template<class I, class D>
    inline operator std::unique_ptr<I, D>() noexcept {
        return static_cast<std::unique_ptr<T, TDeleter>&&>(object);
    }

private:
    std::unique_ptr<T, TDeleter> object;
};

}}}} // boost::di::v1::wrappers

#endif

