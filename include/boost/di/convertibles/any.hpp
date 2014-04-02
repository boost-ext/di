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

#include <vector>
#include <boost/type.hpp>
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
    explicit any_impl(std::vector<aux::shared_ptr<void> >& refs, const TObject& object)
        : refs_(refs), callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator T() const {
        return callback_();
    }

private:
    std::vector<aux::shared_ptr<void> >& refs_;
    function<T()> callback_;
};

template<typename T>
class any_impl<const T&>
{
public:
    template<typename TObject>
    explicit any_impl(std::vector<aux::shared_ptr<void> >& refs, const TObject& object)
        : refs_(refs), callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator const T&() const {
        return callback_();
    }

private:
    std::vector<aux::shared_ptr<void> >& refs_;
    function<reference_wrapper<const T>()> callback_;
};

template<typename T, typename TName>
class any_impl<named<T, TName> >
{
public:
    template<typename TObject>
    any_impl(std::vector<aux::shared_ptr<void> >& refs, const TObject& object)
        : refs_(refs), callback_(boost::bind<T>(object, boost::type<T>()))
    { }

    operator T() const {
        return callback_();
    }

    operator named<T, TName>() const {
        return callback_();
    }

private:
    std::vector<aux::shared_ptr<void> >& refs_;
    function<T()> callback_;
};

} // namespace detail

template<typename T>
class any : public detail::any_impl<T>
{
public:
    template<typename TObject>
    any(std::vector<aux::shared_ptr<void> >& refs, const TObject& object)
        : detail::any_impl<T>(refs, object)
    { }
};

} // namespace convertibles
} // namespace di
} // namespace boost

#endif

