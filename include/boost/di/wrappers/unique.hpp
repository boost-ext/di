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
struct unique {
    template<class I>
    inline operator I() const noexcept {
        return object;
    }

    template<class I>
    inline operator I*() const noexcept { // only for compilation clean
        return {};
    }

    inline operator T&&() noexcept {
        return std::move(object);
    }

    T object;
};

template<class T>
struct unique<T*> {
    template<class I>
    inline operator I() const noexcept {
        return *std::unique_ptr<I>{object};
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

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class I>
    inline operator boost::shared_ptr<I>() const noexcept {
        return boost::shared_ptr<I>{object};
    }
#endif

    template<class I>
    inline operator std::unique_ptr<I>() const noexcept {
        return std::unique_ptr<I>{object};
    }

    T* object = nullptr;
};

template<class T, class TDeleter>
struct unique<std::unique_ptr<T, TDeleter>> {
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

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class I>
    inline operator boost::shared_ptr<I>() noexcept {
        return {object.release(), object.get_deleter()};
    }
#endif

    template<class I, class D>
    inline operator std::unique_ptr<I, D>() noexcept {
        return std::move(object);
    }

    std::unique_ptr<T, TDeleter> object;
};

}}} // boost::di::wrappers

#endif

