//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/type_traits/wrapper_traits.hpp"

namespace boost { namespace di { inline namespace v1 {

BOOST_DI_HAS_METHOD(call_operator, operator());

namespace scopes {

BOOST_DI_HAS_TYPE(result_type);

class external {
    struct injector {
        template<class T> T create() const;
    };

    template<class T, class TExpected, class TGiven>
    struct arg {
        using type = T;
        using expected = TExpected;
        using given = TGiven;
    };

public:
    template<class TExpected, class, class = void>
    struct scope {
        template<class>
        using is_referable = std::false_type;

        explicit scope(const TExpected& object)
            : object_{object}
        { }

        template<class, class TProvider>
        TExpected try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return wrappers::unique<TExpected>{object_};
        }

        TExpected object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, TGiven&,
        BOOST_DI_REQUIRES_T(!has_call_operator<TGiven, const injector&>::value &&
                            !has_call_operator<TGiven, const injector&, const arg<aux::none_type, TExpected, TGiven>&>::value)
    > {
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

        explicit scope(const std::shared_ptr<TGiven>& object)
            : object_{object}
        { }

        template<class, class TProvider>
        wrappers::shared<TGiven> try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return wrappers::shared<TGiven>{object_};
        }

        std::shared_ptr<TGiven> object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, TGiven,
        BOOST_DI_REQUIRES_T(!has_call_operator<TGiven, const injector&>::value &&
                            !has_call_operator<TExpected>::value &&
                             has_call_operator<TGiven>::value)
    > {
        template<class>
        using is_referable = std::false_type;

        explicit scope(const TGiven& object)
            : object_(object)
        { }

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
    struct scope<TExpected, TGiven,
        BOOST_DI_REQUIRES_T(has_call_operator<TGiven, const injector&>::value &&
                           !has_result_type<TGiven>::value)
    > {
        template<class>
        using is_referable = std::false_type;

        explicit scope(const TGiven& object)
            : object_(object)
        { }

        template<class T, class TProvider>
        T try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider& provider) const noexcept {
            using wrapper = type_traits::wrapper_traits_t<decltype((object_)(provider.injector_))>;
            return wrapper{(object_)(provider.injector_)};
        }

        TGiven object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, TGiven,
        BOOST_DI_REQUIRES_T(has_call_operator<TGiven, const injector&, const arg<aux::none_type, TExpected, TGiven>&>::value &&
                           !has_result_type<TGiven>::value)
    > {
        template<class>
        using is_referable = std::false_type;

        explicit scope(const TGiven& object)
            : object_(object)
        { }

        template<class T, class TProvider>
        T try_create(const TProvider&);

        template<class T, class TProvider>
        auto create(const TProvider& provider) const noexcept {
            using wrapper = type_traits::wrapper_traits_t<decltype((object_)(provider.injector_, arg<T, TExpected, TGiven>{}))>;
            return wrapper{(object_)(provider.injector_, arg<T, TExpected, TGiven>{})};
        }

        TGiven object_;
    };
};

}}}} // boost::di::v1::scopes

#endif

