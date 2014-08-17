//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include "boost/di/aux_/ref.hpp"
#include "boost/di/wrappers/value.hpp"

namespace boost {
namespace di {

BOOST_DI_HAS_MEMBER_FUNCTION(call_operator, operator());

namespace scopes {

template<template<typename> class TWrapper = wrappers::value>
class external {
public:
    using priority = int_<1>;

    template<typename TExpected>
    class scope {
    public:
        using result_type = TWrapper<TExpected>;

    private:
        class result_type_holder {
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
        typename std::enable_if<!has_call_operator<TValueType>::value, std::function<result_type()>>::type
        convert_when_function(const T& object) {
            return object;
        }

        template<typename TValueType, typename T>
        typename std::enable_if<has_call_operator<TValueType>::value, result_type_holder>::type
        convert_when_function(const T& object) {
            return result_type_holder(object);
        }

    public:
        template<typename T>
        explicit scope(const T& object
                     , typename std::enable_if<has_call_operator<T>::value && !aux::is_reference_wrapper<T>::value>::type* = 0)
            : object_(convert_when_function<TExpected>(object))
        { }

        template<typename T>
        explicit scope(const T& object
                     , typename std::enable_if<!(has_call_operator<T>::value && !aux::is_reference_wrapper<T>::value)>::type* = 0)
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

