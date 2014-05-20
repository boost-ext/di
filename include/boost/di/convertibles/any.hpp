//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_DETAIL_anyS_ANY_HPP
#define BOOST_DI_DETAIL_anyS_ANY_HPP

#include "boost/di/named.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/type_traits/is_convertible_to_ref.hpp"

#include <vector>
#include <boost/type.hpp>
#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace boost {
namespace di {
namespace convertibles {

namespace detail {

template<typename T>
class any_impl
{
public:
    template<typename TObject>
    explicit any_impl(std::vector<boost::any>& refs, const TObject& object)
        : refs_(refs), callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator T() const {
        //return callback_();
        throw 0;
    }

private:
    std::vector<boost::any>& refs_;
    function<T()> callback_;
};

template<typename T>
class any_impl<const T&>
{
public:
    template<typename TObject>
    explicit any_impl(std::vector<boost::any>& refs, const TObject& object, typename enable_if<di::type_traits::is_convertible_to_ref<TObject, const T&> >::type* = 0)
        //: refs_(refs), callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator const T&() const {
        //refs_.push_back(callback_());
        //return any_cast<const T&>(refs_.back());
        throw 0;
    }

private:
    std::vector<boost::any>& refs_;
    function<T()> callback_;
};

template<typename T, typename TName>
class any_impl<named<T, TName> >
{
public:
    template<typename TObject>
    any_impl(std::vector<boost::any>& refs, const TObject& object)
        : refs_(refs), callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator T() const {
        return callback_();
    }

    operator named<T, TName>() const {
        return callback_();
    }

private:
    std::vector<boost::any>& refs_;
    function<T()> callback_;
};

} // namespace detail

template<typename T>
class any : public detail::any_impl<T>
{
public:
    template<typename TObject>
    any(std::vector<boost::any>& refs, const TObject& object)
        : detail::any_impl<T>(refs, object)
    { }
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

