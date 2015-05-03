//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include <memory>
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/wrapper_traits.hpp"

namespace boost { namespace di { namespace scopes {

class external {
    struct injector {
        template<class T> T create() const;
    };

public:
    template<class TExpected, class, class = void>
    struct scope {
        template<class>
        using is_referable = std::false_type;

        template<class, class TProvider>
        TExpected try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return wrappers::unique<TExpected>{object_};
        }

        TExpected object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, TGiven&, std::enable_if_t<!aux::is_lambda_expr<TGiven, const injector&>::value && !aux::is_lambda_expr<TGiven, const injector&, const aux::type<aux::none_t>&>::value>> {
        template<class>
        using is_referable = std::true_type;

        explicit scope(TGiven& object)
            : object_{object}
        { }

        template<class, class TProvider>
        wrappers::shared<TGiven&> try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return object_;
        }

        wrappers::shared<TGiven&> object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, std::shared_ptr<TGiven>> {
        template<class T>
        using is_referable = typename wrappers::shared<TGiven>::template is_referable<aux::remove_accessors_t<T>>;

        template<class, class TProvider>
        wrappers::shared<TGiven> try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return wrappers::shared<TGiven>{object_};
        }

        std::shared_ptr<TGiven> object_;
    };

    template<class TExpected, class TGiven>
    struct scope<
        TExpected
      , TGiven
      , std::enable_if_t<
            !aux::is_lambda_expr<TGiven, const injector&>::value &&
            !aux::has_call_operator<TExpected>::value &&
             aux::has_call_operator<TGiven>::value
        >
    > {
        template<class>
        using is_referable = std::false_type;

        template<class T, class TProvider>
        auto try_create(const TProvider&) -> type_traits::wrapper_traits_t<decltype(std::declval<TGiven>()())>;

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            using wrapper = type_traits::wrapper_traits_t<decltype(std::declval<TGiven>()())>;
            return wrapper{object_()};
        }

        TGiven object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, TGiven, std::enable_if_t<aux::is_lambda_expr<TGiven, const injector&>::value>> {
        template<class>
        using is_referable = std::false_type;

        template<class T, class TProvider>
        T try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider& provider) const noexcept {
            using wrapper = type_traits::wrapper_traits_t<decltype((object_)(provider.injector_))>;
            return wrapper{(object_)(provider.injector_)};
        }

        TGiven object_;
    };

#if !defined(_MSC_VER)
    template<class TExpected, class TGiven>
    struct scope<TExpected, TGiven, std::enable_if_t<aux::is_lambda_expr<TGiven, const injector&, const aux::type<aux::none_t>&>::value>> {
        template<class>
        using is_referable = std::false_type;

        template<class T, class TProvider>
        T try_create(const TProvider&);

        template<class T, class TProvider>
        auto create(const TProvider& provider) const noexcept {
            using wrapper = type_traits::wrapper_traits_t<decltype((object_)(provider.injector_, aux::type<T>{}))>;
            return wrapper{(object_)(provider.injector_, aux::type<T>{})};
        }

        TGiven object_;
    };
#endif
};

}}} // boost::di::scopes

#endif

