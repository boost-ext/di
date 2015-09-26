// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/wrappers/shared.hpp"
#include "boost/di/wrappers/unique.hpp"

namespace boost { namespace di { inline namespace v1 { namespace scopes {

namespace detail {

struct callable_base_impl { void operator()(...) { } };

template<class T>
struct callable_base : callable_base_impl, std::conditional_t<std::is_class<T>::value, T, aux::none_type> { };

template<typename T>
std::false_type is_callable_impl(T*, aux::non_type<void(callable_base_impl::*)(...), &T::operator()>* = 0);
std::true_type is_callable_impl(...);

template<class T>
struct is_callable : decltype(is_callable_impl((callable_base<T>*)0)) { };

template<class T, class TExpected, class TGiven>
struct arg {
    using type = T;
    using expected = TExpected;
    using given = TGiven;
};

template<class T>
struct wrapper_traits {
    using type = wrappers::unique<T>;
};

template<class T>
struct wrapper_traits<std::shared_ptr<T>> {
    using type = wrappers::shared<T>;
};

template<class T>
using wrapper_traits_t = typename wrapper_traits<T>::type;

template<class T>
class no_implicit_conversions : public T {
    template<class U> operator U() const;
};

BOOST_DI_HAS_TYPE(has_result_type, result_type);

template<class TGiven, class TProvider, class... Ts>
struct is_call : std::integral_constant<bool,
    aux::is_callable_with<TGiven, no_implicit_conversions<
        aux::remove_specifiers_t<decltype(std::declval<TProvider>().injector_)>
    >, Ts...>::value && !has_result_type<TGiven>::value
> { };

} // detail

class external {
public:
    template<class TExpected, class, class = int>
    struct scope {
        template<class>
        using is_referable = std::false_type;

        explicit scope(const TExpected& object)
            : object_{object}
        { }

        template<class, class TProvider>
        static TExpected try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return wrappers::unique<TExpected>{object_};
        }

        TExpected object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, std::shared_ptr<TGiven>> {
        template<class T>
        using is_referable =
            typename wrappers::shared<TGiven>::template is_referable<aux::remove_specifiers_t<T>>;

        explicit scope(const std::shared_ptr<TGiven>& object)
            : object_{object}
        { }

        template<class, class TProvider>
        static wrappers::shared<TGiven> try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return wrappers::shared<TGiven>{object_};
        }

        std::shared_ptr<TGiven> object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, std::initializer_list<TGiven>> {
        template<class>
        using is_referable = std::false_type;

        explicit scope(const std::initializer_list<TGiven>& object)
            : object_(object)
        { }

        template<class, class TProvider>
        static std::initializer_list<TGiven> try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return wrappers::unique<std::initializer_list<TGiven>>{object_};
        }

        std::initializer_list<TGiven> object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, TGiven&, BOOST_DI_REQUIRES(!detail::is_callable<TGiven>::value)> {
        template<class>
        using is_referable = std::true_type;

        explicit scope(TGiven& object)
            : object_{object}
        { }

        template<class, class TProvider>
        static wrappers::shared<TGiven&> try_create(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return object_;
        }

        wrappers::shared<TGiven&> object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, TGiven, BOOST_DI_REQUIRES(detail::is_callable<TGiven>::value)> {
        template<class>
        using is_referable = std::false_type;

        explicit scope(const TGiven& object)
            : object_(object)
        { }

        template<class T, class TProvider>
        T static try_create(const TProvider&);

        template<class, class TProvider,
            BOOST_DI_REQUIRES(!detail::is_call<TGiven, TProvider>::value &&
                               detail::is_callable<TGiven>::value &&
                               detail::is_callable<TExpected>::value) = 0>
        auto create(const TProvider&) const noexcept {
            return wrappers::unique<TExpected>{object_};
        }

        template<class T, class TProvider,
            BOOST_DI_REQUIRES(!detail::is_call<TGiven, TProvider>::value &&
                               aux::is_callable_with<TGiven>::value &&
                              !detail::is_callable<TExpected>::value) = 0>
        auto create(const TProvider&) const noexcept {
            using wrapper = detail::wrapper_traits_t<decltype(std::declval<TGiven>()())>;
            return wrapper{object_()};
        }

        template<class, class TProvider, BOOST_DI_REQUIRES(detail::is_call<TGiven, TProvider>::value) = 0>
        auto create(const TProvider& provider) noexcept {
            using wrapper = detail::wrapper_traits_t<decltype((object_)(provider.injector_))>;
            return wrapper{(object_)(provider.injector_)};
        }

        template<class T, class TProvider, BOOST_DI_REQUIRES(detail::is_call<TGiven, TProvider, const detail::arg<T, TExpected, TGiven>&>::value) = 0>
        auto create(const TProvider& provider) noexcept {
            using wrapper = detail::wrapper_traits_t<decltype((object_)(provider.injector_, detail::arg<T, TExpected, TGiven>{}))>;
            return wrapper{(object_)(provider.injector_, detail::arg<T, TExpected, TGiven>{})};
        }

        TGiven object_;
    };
};

}}}} // boost::di::v1::scopes

#endif

