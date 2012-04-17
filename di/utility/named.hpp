//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef DI_utILITY_NAMED_HPP
#define DI_utILITY_NAMED_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "di/back/aux/utility.hpp"
#include "di/config.hpp"

namespace di
{
namespace utility
{

/**
 * Example: named< int, mpl::string<'Port'> >
 */
template<typename T, typename TName = void>
class named
{
public:
    typedef typename back::aux::make_plain<T>::type value_type;
    typedef named<value_type, TName> element_type;

    named(T value = T()) // non explicit
        : value_(new T(value))
    { }

    named(const boost::shared_ptr<T>& value) // non explicit
        : value_(value)
    { }

    operator T() const { return *value_; }

private:
    boost::shared_ptr<T> value_;
};

template<typename T, typename TName>
class named< boost::shared_ptr<T>, TName>
{
public:
    typedef named<typename back::aux::make_plain<T>::type, TName> element_type;
    typedef typename back::aux::make_plain<T>::type value_type;

    named(boost::shared_ptr<T> value = boost::make_shared<T>()) // non explicit
        : value_(value)
    { }

    named(value_type value) // non explicit
       : value_(boost::make_shared<value_type>(value))
    { }

    operator boost::shared_ptr<T>() const { return value_; }
    operator value_type() const { return *value_; }
    T* operator->() const { return value_.get(); }
    T& operator*() const { return *value_; }
    T* get() const { return value_.get(); }

private:
    boost::shared_ptr<T> value_;
};

template<typename T, typename TName>
class named< const boost::shared_ptr<T>&, TName> : named< boost::shared_ptr<T>, TName>
{
public:
    typedef named<typename back::aux::make_plain<T>::type, TName> element_type;
    typedef typename back::aux::make_plain<T>::type value_type;

    named(const boost::shared_ptr<T>& value) // non explicit
        : named< boost::shared_ptr<T>, TName>(value)
    { }
};

} // namespace utility
} // namespace di

#endif

