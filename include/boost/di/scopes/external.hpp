//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include "boost/di/wrappers/value.hpp"
#include "boost/di/wrappers/shared.hpp"
#include "boost/di/wrappers/reference.hpp"
#include "boost/di/aux_/type_traits.hpp"

#include <boost/ref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace di {

namespace fwd {
struct injector {
    template<class T>
    T create();
};
} // namespace fwd

namespace scopes {

BOOST_DI_HAS_TYPE(result_type);
BOOST_DI_HAS_METHOD(call_operator, operator());

template<class T, class U>
using has_lambda = bool_<has_call_operator<T, U>::value && !has_result_type<T>::value>;

template<class TT>
class external {
public:
    static constexpr auto priority = 1; // 0 - lowest, N - highest

    template<class>
    struct rebind {
        using other = external;
    };

    template<class T, class = void>
    class scope
    {
        using result_type = wrappers::value<T>;

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

    template<class T>
    class scope<T
              , typename enable_if_c<
                     has_call_operator<T>::value &&
                    !has_lambda<T, fwd::injector&>::value
                >::type>
    {
        using result_type = wrappers::value<T>;

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

    template<class T>
    class scope<T, aux::enable_if_t<has_lambda<T, fwd::injector&>{}>>
    {
        using result_type = wrappers::value<T>;
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

        template<class TInjector>
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

