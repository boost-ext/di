
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include <memory>
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/wrappers/unique.hpp"
#include "boost/di/wrappers/shared.hpp"

namespace boost { namespace di { namespace scopes {

BOOST_DI_HAS_TYPE(result_type, result_type);
BOOST_DI_HAS_METHOD(call_operator, operator());

template<class T, class U>
using is_lambda_expr =
    std::integral_constant<
        bool
      , has_call_operator<T, U>::value &&
       !has_result_type<T>::value
    >;

template<class T, class TDeleter>
struct wrapper_traits {
    using type = wrappers::unique<T, TDeleter>;
};

template<class T, class TDeleter>
struct wrapper_traits<std::shared_ptr<T>, TDeleter> {
    using type = wrappers::shared<T>;
};

template<class T, class TDeleter>
using wrapper_traits_t =
    typename wrapper_traits<T, TDeleter>::type;

class external {
    struct injector {
        template<class T> T create() const;
    };

public:
    static constexpr auto priority = true;

    template<class TExpected, class, class = void>
    class scope {
    public:
        explicit scope(const TExpected& object) noexcept
            : object_(object)
        { }

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            using deleter = typename TProvider::deleter;
            return wrappers::unique<TExpected, deleter>{object_};
        }

    private:
        TExpected object_;
    };

    template<class TExpected, class TGiven>
    class scope<TExpected, std::reference_wrapper<TGiven>> {
    public:
        using is_ref = void;

        explicit scope(const std::reference_wrapper<TGiven>& object) noexcept
            : object_(object)
        { }

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return object_;
        }

    private:
        std::reference_wrapper<TGiven> object_;
    };

    template<class TExpected, class TGiven>
    class scope<TExpected, std::shared_ptr<TGiven>> {
    public:
        explicit scope(const std::shared_ptr<TGiven>& object) noexcept
            : object_(object)
        { }

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return wrappers::shared<TGiven>{object_};
        }

    private:
        std::shared_ptr<TGiven> object_;
    };

    template<class TExpected, class TGiven>
    class scope<
        TExpected
      , TGiven
      , std::enable_if_t<
            !is_lambda_expr<TGiven, const injector&>{} &&
            !has_call_operator<TExpected>{} &&
             has_call_operator<TGiven>{}
        >
    > {
    public:
        explicit scope(const TGiven& object) noexcept
            : object_(object)
        { }

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            using deleter = typename TProvider::deleter;
            using wrapper = wrapper_traits_t<decltype(std::declval<TGiven>()()), deleter>;
            return wrapper{object_()};
        }

    private:
        TGiven object_;
    };

    template<class TExpected, class TGiven>
    class scope<TExpected, TGiven, std::enable_if_t<is_lambda_expr<TGiven, const injector&>{}>> {
    public:
        explicit scope(const TGiven& object) noexcept
            : object_(object)
        { }

        template<class, class TProvider>
        auto create(const TProvider& provider) const noexcept {
            using deleter = typename TProvider::deleter;
            using wrapper = wrapper_traits_t<decltype((object_)(provider.injector_)), deleter>;
            return wrapper{(object_)(provider.injector_)};
        }

    private:
        TGiven object_;
    };
};

}}} // boost::di::scopes

#endif

