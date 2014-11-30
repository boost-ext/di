//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include "boost/di/aux_/type_traits.hpp"

#include "boost/di/wrappers/value.hpp"
#include "boost/di/wrappers/shared.hpp"
#include "boost/di/wrappers/reference.hpp"
#include "boost/di/aux_/type_traits.hpp"

namespace boost {
namespace di {
namespace scopes {

BOOST_DI_HAS_TYPE(result_type);
BOOST_DI_HAS_METHOD(call_operator, operator());

template<class T, class U>
using has_lambda = aux::bool_<has_call_operator<T, U>::value && !has_result_type<T>::value>;

class external {
    struct injector {
        template<class T>
        T create() const;
    };

public:
    static constexpr auto priority = true;

    template<class TT, class T, class = void>
    class scope {
    public:
        explicit scope(const T& object) noexcept
            : object_(object)
        { }

        template<class, class TProvider>
        decltype(auto) create(const TProvider&) const noexcept {
            return object_;
        }

    private:
        wrappers::value<T> object_;
    };

    template<class TT, class T>
    class scope<TT, T
              , std::enable_if_t<
                     has_call_operator<T>::value &&
                    !has_lambda<T, const injector&>::value
                >>
    {
    public:
        explicit scope(const T& object) noexcept
            : object_(object)
        { }

        template<class, class TProvider>
        wrappers::value<TT> create(const TProvider&) const noexcept {
            return object_();
        }

    private:
        T object_;
    };

    template<class TT, class T>
    class scope<TT, T, std::enable_if_t<has_lambda<T, const injector&>{}>>
    {
        scope& operator=(const scope&) = delete;

    public:
        explicit scope(const T& object) noexcept
            : given_(object)
        { }

        scope(const scope& other) noexcept
            : given_(other.given_)
        { }

        template<class, class TProvider>
        wrappers::value<TT> create(const TProvider& provider) const noexcept {
            return (given_)(provider.injector_);
        }

    private:
        const T& given_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

#endif

