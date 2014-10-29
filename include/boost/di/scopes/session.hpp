//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SESSION_HPP
#define BOOST_DI_SCOPES_SESSION_HPP

#include "boost/di/wrappers/shared.hpp"

#include <boost/function.hpp>
#include <boost/mpl/int.hpp>

namespace boost {
namespace di {
namespace scopes {

class session_entry { };
class session_exit { };

template<template<typename> class TWrapper = wrappers::shared>
class session
{
public:
    typedef mpl::int_<0> priority;

    template<typename TExpected, typename>
    class scope
    {
    public:
        typedef TWrapper<TExpected> result_type;

        scope()
            : in_scope_(false)
        { }

        void call(const session_entry&) {
            in_scope_ = true;
        }

        void call(const session_exit&) {
            in_scope_ = false;
            object_.reset();
        }

        result_type create(const function<TExpected*()>& f) {
            if (in_scope_ && !object_) {
                object_.reset(f());
            }
            return object_;
        }

    private:
        result_type object_;
        bool in_scope_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

