//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_SHARED_HPP
#define BOOST_DI_WRAPPERS_SHARED_HPP

#include <memory>
#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace wrappers {

template<class T>
struct shared {
    template<class I>
    inline operator std::shared_ptr<I>() const noexcept {
        return object;
    }

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class TSharedPtr>
    struct sp_holder {
        TSharedPtr object;

        void operator()(...) noexcept {
            object.reset();
        }
    };

    template<class I>
    inline operator boost::shared_ptr<I>() const noexcept {
        using sp = sp_holder<boost::shared_ptr<I>>;
        if (auto* deleter = std::get_deleter<sp, I>(object)) {
            return deleter->object;
        } else {
            return {object.get(), sp_holder<std::shared_ptr<T>>{object}};
        }
    }
#endif

    template<class I>
    inline operator std::weak_ptr<I>() const noexcept {
        return object;
    }

    std::shared_ptr<T> object;
};

}}} // boost::di::wrappers

#endif

