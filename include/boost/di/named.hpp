//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_NAMED_HPP
#define BOOST_DI_NAMED_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/type_traits/make_plain.hpp"
#include "boost/di/type_traits/remove_accessors.hpp"

#include <utility>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace di {

//struct no_name { };

template<
    typename T
  , typename TName = void
  , typename = void
>
class named
{
    named& operator=(const named&);

public:
    typedef T named_type;
    typedef TName name;

    named(T object) // non explicit
        : object_(object)
    { }

    named(const named& other)
        : object_(other.object_)
    { }

    operator T() const {
        return object_;
    }

private:
    T object_;
};

template<
    typename T
  , typename TName
>
class named<const T&, TName>
{
    named& operator=(const named&);

public:
    typedef const T& named_type;
    typedef TName name;

    named(const T& object) // non explicit
        : object_(object)
    { }

    named(const named& other)
        : object_(other.object_)
    { }

    operator const T&() const {
        return object_;
    }

private:
    const T& object_;
};

template<
    typename T
  , typename TName
>
class named<T&, TName>
{
    named& operator=(const named&);

public:
    typedef T& named_type;
    typedef TName name;

    named(T& object) // non explicit
        : object_(object)
    { }

    named(const named& other)
        : object_(other.object_)
    { }

    operator T&() {
        return object_;
    }

private:
    T& object_;
};

BOOST_DI_FEATURE(RVALUE_REFERENCES)(
    template<
        typename T
      , typename TName
    >
    class named<T&&, TName>
    {
        named& operator=(const named&);

    public:
        typedef T&& named_type;
        typedef TName name;

        named(T&& object) // non explicit
            : object_(std::move(object))
        { }

        named(const named& other)
            : object_(other.object_)
        { }

        operator T&&() {
            return std::move(object_);
        }

    private:
        T object_;
    };
)

template<
    typename T
  , typename TName
>
class named<aux::unique_ptr<T>, TName>
{
    named& operator=(const named&);

public:
    typedef aux::unique_ptr<T> named_type;
    typedef TName name;

    BOOST_DI_FEATURE(RVALUE_REFERENCES)(
        named(aux::unique_ptr<T> object) // non explicit
            : object_(std::move(object))
        { }

        operator aux::unique_ptr<T>() {
            return std::move(object_);
        }
    )

    named(const named& other)
        : object_(new T(*other.object_))
    { }

private:
    aux::unique_ptr<T> object_;
};

template<
    typename T
  , typename TName
>
class named<
    T
  , TName
  , typename enable_if<
        is_polymorphic<typename type_traits::remove_accessors<T>::type>
    >::type
>
{
public:
    typedef T named_type;
    typedef TName name;
};

} // namespace di
} // namespace boost

#endif

