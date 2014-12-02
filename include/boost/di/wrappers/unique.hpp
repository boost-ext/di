//
// uniqueright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or unique at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_UNIQUE_HPP
#define BOOST_DI_WRAPPERS_UNIQUE_HPP

#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { namespace wrappers {

template<class T>
class unique {
public:
    unique(const T& value) noexcept // non explicit
        : value_(value)
    { }

    template<class I>
    inline operator I() const noexcept {
        return value_;
    }

private:
    T value_;
};

template<class T>
class unique<T*> {
public:
    unique(T* value) noexcept // non explicit
        : value_(value)
    { }

    template<class I, class = std::enable_if_t<!std::is_polymorphic<I>{}>>
    inline operator I() const noexcept {
        return *aux::unique_ptr<I>(value_);
    }

    template<class I>
    inline operator I*() const noexcept {
        return value_; // ownership transfer
    }

    template<class I>
    inline operator const I*() const noexcept {
        return value_; // ownership transfer
    }

    template<class I>
    inline operator aux::shared_ptr<I>() const noexcept {
        return aux::shared_ptr<I>{value_};
    }

    template<class I>
    inline operator aux_::shared_ptr<I>() const noexcept {
        return aux_::shared_ptr<I>{value_};
    }

    template<class I>
    inline operator aux::unique_ptr<I>() const noexcept {
        return aux::unique_ptr<I>{value_};
    }

private:
    T* value_ = nullptr;
};

}}} // namespace boost::di::wrappers

#endif

