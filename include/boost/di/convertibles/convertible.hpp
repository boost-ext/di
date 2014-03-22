//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_CONVERTIBLES_CONVERTIBLE_HPP
#define BOOST_DI_DETAIL_CONVERTIBLES_CONVERTIBLE_HPP

#include "boost/di/named.hpp"

#include <boost/type.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace boost {
namespace di {
namespace convertibles {

namespace detail {

template<typename T>
class convertible_impl
{
public:
    template<typename TObject>
    convertible_impl(const TObject& object)
        : callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator T() const {
        return callback_();
    }

private:
    function<T()> callback_;
};

template<typename T, typename TName>
class convertible_impl<named<T, TName> >
{
public:
    template<typename TObject>
    convertible_impl(const TObject& object)
        : callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator T() const {
        return callback_();
    }

    operator named<T, TName>() const {
        return callback_();
    }

private:
    function<T()> callback_;
};

} // namespace detail

template<typename T>
class convertible : public detail::convertible_impl<T>
{
public:
    template<typename TObject>
    convertible(const TObject& object)
        : detail::convertible_impl<T>(object)
    { }
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

