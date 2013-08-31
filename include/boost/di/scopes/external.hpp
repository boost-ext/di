//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/variant.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>

#include "boost/di/type_traits/value_type.hpp"
#include "boost/di/type_traits/has_traits.hpp"
#include "boost/di/named.hpp"
#include "boost/di/config.hpp"

namespace boost {
namespace di {
namespace scopes {

class external
{
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
                type_traits::has_name<T>
              , type_traits::has_value_type<T>
            >
        >::type
    >
    {
        typedef typename T::value_type type;
    };

    template<typename>
    class convertible_any_impl;

    template<typename TDest>
    class convertible_any_impl<TDest&>
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
    class convertible_any_impl<TDest*>
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
    class convertible_any_impl<shared_ptr<TDest> >
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
    template<
        typename TExpected
      , typename TGiven = TExpected
      , typename = void
    >
    class scope
    {
        typedef typename plain_type<TExpected>::type object_type;

        typedef variant<
            object_type&
          , const object_type&
          , shared_ptr<object_type>
        > value_type;

    public:
        typedef scope result_type;

        operator object_type&() const {
            return apply_visitor(
                convertible_any_impl<object_type&>(), object_);
        }

        operator object_type*() const {
            return apply_visitor(
                convertible_any_impl<object_type*>(), object_);
        }

        template<typename I>
        operator shared_ptr<I>() const {
            return apply_visitor(
                convertible_any_impl<shared_ptr<object_type> >(), object_);
        }

        template<typename I, typename TName>
        operator named<shared_ptr<I>, TName>() const {
            return apply_visitor(
                convertible_any_impl<shared_ptr<object_type> >(), object_);
        }

        template<typename T>
        explicit scope(const T& value)
            : object_(value)
        { }

        result_type& create() {
            return *this;
        }

    private:
        value_type object_;
    };

    template<
        typename TExpected
      , typename TGiven
    >
    class scope<
        TExpected
      , TGiven
      , typename enable_if<
            mpl::or_<
                is_same<typename type_traits::value_type<TExpected>::type, std::string>
              , is_arithmetic<typename type_traits::value_type<TExpected>::type>
              , type_traits::has_result_type<typename type_traits::value_type<TExpected>::type>
            >
        >::type
    >
    {
        typedef typename type_traits::value_type<TExpected>::type value_type;
        typedef typename plain_type<value_type>::type object_type;

    public:
        typedef scope result_type;

        operator object_type() const {
            return object_;
        }

        operator object_type*() const {
            return new object_type(object_);
        }

        operator shared_ptr<object_type>() const {
            return make_shared<object_type>(object_);
        }

        template<typename TName>
        operator named<shared_ptr<object_type>, TName>() const {
            return make_shared<object_type>(object_);
        }

        template<typename TName>
        operator named<object_type, TName>() const {
            return object_;
        }

        template<typename T>
        explicit scope(const T& value)
            : object_(value)
        { }

        result_type& create() {
            return *this;
        }

    private:
        value_type object_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

