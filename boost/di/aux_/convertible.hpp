//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_AUX_CONVERTIBLE_HPP
#define BOOST_DI_AUX_CONVERTIBLE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/variant.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>

#include "boost/di/aux_/value_type.hpp"
#include "boost/di/aux_/has_traits.hpp"
#include "boost/di/named.hpp"

namespace boost {
namespace di {
namespace aux_ {

namespace detail {

template<typename T, typename = void>
struct plain_type
{
    typedef T type;
};

template<typename T>
struct plain_type<
    T
  , typename enable_if<
        mpl::and_<
            has_name<T>
          , has_value_type<T>
        >
    >::type
>
{
    typedef typename T::value_type type;
};

} // namespace detail

template<
    typename T
  , typename TContext = mpl::vector0<>
  , typename = void
>
class convertible
{
    typedef typename detail::plain_type<T>::type object_type;

    typedef variant<
        object_type&
      , const object_type&
      , shared_ptr<object_type>
    > value_type;

    template<typename>
    class convertible_impl;

    template<typename TDest>
    class convertible_impl<TDest&>
        : public static_visitor<TDest&>
    {
    public:
        template<typename TSrc>
        TDest& operator()(const TSrc& object) const {
            return const_cast<TDest&>(object);
        }

        template<typename TSrc>
        TDest& operator()(TSrc& object) const {
            return object;
        }

        template<typename TSrc>
        TDest& operator()(const shared_ptr<TSrc>& object) const {
            return *object;
        }
    };

    template<typename TDest>
    class convertible_impl<TDest*>
        : public static_visitor<TDest*>
    {
    public:
        template<typename TSrc>
        TDest* operator()(const TSrc& object) const {
            return new TSrc(object);
        }

        template<typename TSrc>
        TDest* operator()(TSrc& object) const {
            return new TSrc(object);
        }

        template<typename TSrc>
        TDest* operator()(const shared_ptr<TSrc>& object) const {
            return new TSrc(*object);
        }
    };

    template<typename TDest>
    class convertible_impl<shared_ptr<TDest> >
        : public static_visitor<shared_ptr<TDest> >
    {
    public:
        template<typename TSrc>
        shared_ptr<TDest> operator()(const TSrc& object) const {
            //return shared_ptr<TSrc>(new TSrc(object));
            return shared_ptr<TDest>();
        }

        template<typename TSrc>
        shared_ptr<TDest> operator()(TSrc& object) const {
            //return shared_ptr<TSrc>(new TSrc(&object));
            return shared_ptr<TDest>();
        }

        template<typename TSrc>
        shared_ptr<TDest> operator()(const shared_ptr<TSrc>& object) const {
            return object;
        }
    };

public:
    typedef convertible type;
    typedef T element_type;
    typedef TContext context;

    convertible(const object_type& object) // non explicit
        : convertible_(object)
    { }

    convertible(object_type& object) // non explicit
        : convertible_(object)
    { }

    convertible(shared_ptr<object_type> object) // non explicit
        : convertible_(object)
    { }

    operator object_type&() const {
        return apply_visitor(
            convertible_impl<object_type&>(), convertible_);
    }

    operator object_type*() const {
        return apply_visitor(
            convertible_impl<object_type*>(), convertible_);
    }

    template<typename I>
    operator shared_ptr<I>() const {
        return apply_visitor(
            convertible_impl<shared_ptr<object_type> >(), convertible_);
    }

    template<typename I, typename TName>
    operator named<shared_ptr<I>, TName>() const {
        return apply_visitor(
            convertible_impl<shared_ptr<object_type> >(), convertible_);
    }

private:
    value_type convertible_;
};

template<
    typename T
  , typename TContext
>
class convertible<
    T
  , TContext
  , typename enable_if<
        mpl::or_<
            is_same<typename value_type<T>::type, std::string>
          , is_arithmetic<typename value_type<T>::type>
          , has_result_type<typename value_type<T>::type>
        >
    >::type
>
{
    typedef typename value_type<T>::type value_type;
    typedef typename detail::plain_type<value_type>::type object_type;

public:
    typedef convertible type;
    typedef T element_type;
    typedef TContext context;

    convertible(object_type object) // non explicit
        : convertible_(object)
    { }

    convertible(shared_ptr<object_type> object) // non explicit
        : convertible_(*object)
    { }

    operator object_type() const {
        return convertible_;
    }

    operator object_type*() const {
        return new object_type(convertible_);
    }

    operator shared_ptr<object_type>() const {
        return make_shared<object_type>(convertible_);
    }

    template<typename TName>
    operator named<shared_ptr<object_type>, TName>() const {
        return make_shared<object_type>(convertible_);
    }

    template<typename TName>
    operator named<object_type, TName>() const {
        return convertible_;
    }

private:
    object_type convertible_;
};

} // namespace aux_
} // namespace di
} // namespace boost

#endif

