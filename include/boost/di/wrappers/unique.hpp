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
#include "boost/di/aux_/type_traits.hpp"

namespace boost { namespace di { namespace wrappers {

template<class T>
class unique {
public:
    unique(const T& value) noexcept // non explicit
        : value_(value)
    { }

    //template<class I>
    inline operator T() const noexcept {
        return value_;
    }

    template<class TT>
    inline operator named<T, TT>() const noexcept {
        return value_;
    }

    template<class I>
    inline operator I*() const noexcept { // only for compilation clean
        return {};
    }

private:
    T value_;
};

/*template<class T>*/
//class unique<T*> {
//public:
    //unique(T* value) noexcept // non explicit
        //: value_(value)
    //{ }

    //template<class I, class = std::enable_if_t<!std::is_polymorphic<I>{}>>
    //inline operator I() const noexcept {
        //return *std::unique_ptr<I>(value_);
    //}

    //template<class I>
    //inline operator I*() const noexcept {
        //return value_; // ownership transfer
    //}

    //template<class I>
    //inline operator const I*() const noexcept {
        //return value_; // ownership transfer
    //}

    //template<class I>
    //inline operator std::shared_ptr<I>() const noexcept {
        //return std::shared_ptr<I>{value_};
    //}

//#if (__has_include(<boost/shared_ptr.hpp>))
    //template<class I>
    //inline operator boost::shared_ptr<I>() const noexcept {
        //return boost::shared_ptr<I>{value_};
    //}
//#endif

    //template<class I>
    //inline operator std::unique_ptr<I>() const noexcept {
        //return std::unique_ptr<I>{value_};
    //}

//private:
    //T* value_ = nullptr;
/*};*/

}}} // boost::di::wrappers

#endif

