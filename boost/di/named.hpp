//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_NAMED_HPP
#define BOOST_DI_NAMED_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "boost/di/aux/make_plain.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {

/**
 * example: named< int, mpl::string<'port'> >
 */
template<typename T, typename TName = void>
class named
{
public:
    typedef typename aux::make_plain<T>::type value_type;
    typedef named<value_type, TName> element_type;

    named(T value = T()) // non explicit
        : value_(new T(value))
    { }

    named(const shared_ptr<T>& value) // non explicit
        : value_(value)
    { }

    operator T() const { return *value_; }

private:
    shared_ptr<T> value_;
};

template<typename T, typename TName>
class named< shared_ptr<T>, TName>
{
public:
    typedef named<typename aux::make_plain<T>::type, TName> element_type;
    typedef typename aux::make_plain<T>::type value_type;

    named(shared_ptr<T> value = make_shared<T>()) // non explicit
        : value_(value)
    { }

    named(value_type value) // non explicit
       : value_(make_shared<value_type>(value))
    { }

    operator shared_ptr<T>() const { return value_; }
    operator value_type() const { return *value_; }
    T* operator->() const { return value_.get(); }
    T& operator*() const { return *value_; }
    T* get() const { return value_.get(); }

private:
    shared_ptr<T> value_;
};

template<typename T, typename TName>
class named< const shared_ptr<T>&, TName> : named< shared_ptr<T>, TName>
{
public:
    typedef named<typename aux::make_plain<T>::type, TName> element_type;
    typedef typename aux::make_plain<T>::type value_type;

    named(const shared_ptr<T>& value) // non explicit
        : named< shared_ptr<T>, TName>(value)
    { }
};

} // namespace boost
} // namespace di

#endif

