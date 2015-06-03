//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_SHARED_HPP
#define BOOST_DI_WRAPPERS_SHARED_HPP

#include <memory>
#include "boost/di/aux_/utility.hpp"
#include "boost/di/fwd.hpp" // boost::shared_ptr

namespace boost { namespace di { inline namespace v1 { namespace wrappers {

template<class T>
struct shared {
    using type = std::conditional_t<std::is_same<T, void>::value, _, T>;

    template<class>
    struct is_referable
        : std::true_type
    { };

    template<class I>
    struct is_referable<std::shared_ptr<I>>
        : std::false_type
    { };

    template<class I>
    inline operator std::shared_ptr<I>() const noexcept {
        return object;
    }

    template<class TSharedPtr>
    struct sp_holder {
        TSharedPtr object;

        void operator()(...) noexcept {
            object.reset();
        }
    };

    template<class I>
    inline operator boost::shared_ptr<I>() const noexcept {
        using sp = sp_holder<boost::shared_ptr<T>>;
        if (auto* deleter = std::get_deleter<sp, T>(object)) {
            return deleter->object;
        } else {
            return {object.get(), sp_holder<std::shared_ptr<T>>{object}};
        }
    }

    template<class I>
    inline operator std::weak_ptr<I>() const noexcept {
        return object;
    }

    inline operator type&() noexcept {
        return *object;
    }

    inline operator const type&() const noexcept {
        return *object;
    }

    std::shared_ptr<T> object;
};

template<class T>
struct shared<T&> {
    template<class I>
    inline operator I() const noexcept {
        return object;
    }

    inline operator T&() const noexcept {
        return object;
    }

    template<class I>
    inline operator I*() const noexcept { // only for compilation clean
        return {};
    }

    T& object;
};

}}}} // boost::di::v1::wrappers

#endif

