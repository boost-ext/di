//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include "boost/di/wrappers/value.hpp"
#include "boost/di/type_traits/has_call_operator.hpp"

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

BOOST_DI_FEATURE(CPP_11_FUNCTIONAL)(
    namespace boost {

    template<typename T>
    class is_reference_wrapper< ::std::reference_wrapper<T> >
        : public ::boost::di::aux::mpl::true_
    { };

    template<typename T>
    class unwrap_reference< ::std::reference_wrapper<T> >
    {
    public:
        typedef T type;
    };

    } // namespace
)

namespace boost {
namespace di {
namespace scopes {

template<template<typename> class TWrapper = wrappers::value>
class external
{
public:
    typedef aux::mpl::int_<1> priority;

    template<typename TExpected>
    class scope
    {
    public:
        typedef TWrapper<TExpected> result_type;

    private:
        class result_type_holder
        {
        public:
            template<typename T>
            explicit result_type_holder(const T& object)
                : object_(object)
            { }

            result_type operator()() const {
                return object_;
            }

        private:
            result_type object_;
        };

        template<typename TValueType, typename T>
        typename disable_if<type_traits::has_call_operator<TValueType>, std::function<result_type()> >::type
        convert_when_function(const T& object) {
            return object;
        }

        template<typename TValueType, typename T>
        typename enable_if<type_traits::has_call_operator<TValueType>, result_type_holder>::type
        convert_when_function(const T& object) {
            return result_type_holder(object);
        }

    public:
        template<typename T>
        explicit scope(const T& object
                     , typename enable_if_c<
                           type_traits::has_call_operator<T>::value &&
                           !is_reference_wrapper<T>::value
                       >::type* = 0)
            : object_(convert_when_function<TExpected>(object))
        { }

        template<typename T>
        explicit scope(const T& object
                     , typename disable_if_c<
                           type_traits::has_call_operator<T>::value &&
                           !is_reference_wrapper<T>::value
                       >::type* = 0)
            : object_(result_type_holder(object))
        { }

        result_type create() {
            return object_();
        }

    private:
        std::function<result_type()> object_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

