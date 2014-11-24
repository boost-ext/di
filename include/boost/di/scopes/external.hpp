//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include "boost/di/wrappers/value.hpp"
#include "boost/di/aux_/type_traits.hpp"

#include <boost/ref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace di {

namespace fwd {
struct injector {
    template<typename T>
    T create();
};
} // namespace fwd

namespace scopes {

template<template<typename> class TWrapper = wrappers::value>
class external {
public:
    static constexpr auto priority = 1; // 0 - lowest, N - highest

    template<typename T, typename = void>
    class scope
    {
        using result_type = TWrapper<T>;

    public:
        explicit scope(const T& object)
            : object_(object)
        { }

        void create3();

        result_type create() {
            return object_;
        }

    private:
        result_type object_;
    };

    template<typename T>
    class scope<T
              , typename enable_if_c<
                     aux::has_call_operator<T>::value &&
                    !aux::is_reference_wrapper<T>::value &&
                    !aux::has_lambda<T, fwd::injector&>::value
                >::type>
    {
        using result_type = TWrapper<T>;

    public:
        explicit scope(const T& object)
            : object_(object)
        { }

        void create3();

        result_type create() {
            return object_();
        }

    private:
        T object_;
    };

    template<typename T>
    class scope<T, aux::enable_if_t<aux::has_lambda<T, fwd::injector&>{}>>
    {
        using result_type = TWrapper<T>;
        scope& operator=(const scope&) = delete;

    public:
        explicit scope(const T& object)
            : given_(object)
        { }

        scope(const scope& other)
            : given_(other.given_)
        { }

        typedef void create2;

        result_type create(double);
        void create3();

        template<typename TInjector>
        result_type create_(TInjector& injector) {
            return (given_)(injector);
        }

    private:
        const T& given_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

