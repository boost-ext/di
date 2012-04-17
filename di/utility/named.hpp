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

    named(T p_value = T()) // non explicit
        : m_value(new T(p_value))
    { }

    named(const boost::shared_ptr<T>& p_value) // non explicit
        : m_value(p_value)
    { }

    operator T() const { return *m_value; }

private:
    boost::shared_ptr<T> m_value;
};

template<typename T, typename TName>
class named< boost::shared_ptr<T>, TName>
{
public:
    typedef named<typename back::aux::make_plain<T>::type, TName> element_type;
    typedef typename back::aux::make_plain<T>::type value_type;

    named(boost::shared_ptr<T> p_value = boost::make_shared<T>()) // non explicit
        : m_value(p_value)
    { }

    named(value_type p_value) // non explicit
       : m_value(boost::make_shared<value_type>(p_value))
    { }

    operator boost::shared_ptr<T>() const { return m_value; }
    operator value_type() const { return *m_value; }
    T* operator->() const { return m_value.get(); }
    T& operator*() const { return *m_value; }
    T* get() const { return m_value.get(); }

private:
    boost::shared_ptr<T> m_value;
};

template<typename T, typename TName>
class named< const boost::shared_ptr<T>&, TName> : named< boost::shared_ptr<T>, TName>
{
public:
    typedef named<typename back::aux::make_plain<T>::type, TName> element_type;
    typedef typename back::aux::make_plain<T>::type value_type;

    named(const boost::shared_ptr<T>& p_value) // non explicit
        : named< boost::shared_ptr<T>, TName>(p_value)
    { }
};

} // namespace utility
} // namespace di

#endif

