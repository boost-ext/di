//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_HPP
#define BOOST_DI_HPP


#if (__cplusplus < 201305L)
   #error "C++14 is required by Boost.DI"
#endif

#if defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS)

// config
#include "boost/di/config.hpp"

// bindings
#include "boost/di/bindings.hpp"

// injections
#include "boost/di/inject.hpp"
#include "boost/di/injector.hpp"
#include "boost/di/make_injector.hpp"

// scopes
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/unique.hpp"

#else

#include <memory>
#include <type_traits>

#ifndef BOOST_DI_AUX_UTILITY_HPP
#define BOOST_DI_AUX_UTILITY_HPP

namespace boost { namespace di { namespace aux {

struct none_t { };

template<class...>
struct type { };

template<class T, T>
struct non_type { };

template<class...>
using void_t = void;

#if defined(__clang__)
    template<class...>
    using always = std::true_type;

    template<class...>
    using never = std::false_type;
#else
    template<class...>
    struct always : std::true_type { };

    template<class...>
    struct never : std::false_type { };
#endif

template<class T>
struct identity {
    using type = T;
};

template<class, class>
struct pair { using type = pair; };

template<bool...>
struct bool_list { using type = bool_list; };

template<class...>
struct type_list { using type = type_list; };

template<class... TArgs>
struct inherit : TArgs... { using type = inherit; };

template<class T>
struct no_decay { using type = T; };

template<class TDefault, class>
static no_decay<TDefault> lookup(...);

template<class, class TKey, class TValue>
static no_decay<TValue> lookup(pair<TKey, TValue>*);

template<class TDefault, class TKey, class... Ts>
using at_key = decltype(lookup<TDefault, TKey>((inherit<Ts...>*)0));

template<class TDefault, class TKey, class... Ts>
using at_key_t = typename at_key<TDefault, TKey, Ts...>::type;

template<class...>
struct join;

template<>
struct join<> { using type = type_list<>; };

template<class... TArgs>
struct join<type_list<TArgs...>> {
    using type = type_list<TArgs...>;
};

template<class... TArgs1, class... TArgs2>
struct join<type_list<TArgs1...>, type_list<TArgs2...>> {
    using type = type_list<TArgs1..., TArgs2...>;
};

template<class... TArgs1, class... TArgs2, class... Ts>
struct join<type_list<TArgs1...>, type_list<TArgs2...>, Ts...> {
    using type = typename join<type_list<TArgs1..., TArgs2...>, Ts...>::type;
};

template<class... TArgs>
using join_t = typename join<TArgs...>::type;

template<class, class...>
struct is_unique_impl;

template<class...>
struct not_unique : std::false_type {
    using type = not_unique;
};

template<>
struct not_unique<> : std::true_type {
    using type = not_unique;
};

template<class T>
struct is_unique_impl<T> : not_unique<> { };

template<class T1, class T2, class... Ts>
struct is_unique_impl<T1, T2, Ts...>
	: std::conditional_t<
		  std::is_base_of<type<T2>, T1>{}
		, not_unique<T2>
		, is_unique_impl<inherit<T1, type<T2>>, Ts...>
 	  >
{ };

template<class... Ts>
using is_unique = is_unique_impl<none_t, Ts...>;

} // aux

struct _ { _(...) { } };

}} // boost::di

#endif

#ifndef BOOST_DI_AUX_TYPE_TRAITS_HPP
#define BOOST_DI_AUX_TYPE_TRAITS_HPP

#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

#define BOOST_DI_HAS_TYPE(name)                                     \
    template<class, class = void>                                   \
    struct has_##name : std::false_type { };                        \
                                                                    \
    template<class T>                                               \
    struct has_##name<T, aux::void_t<typename T::name>>             \
        : std::true_type                                            \
    { }

#define BOOST_DI_HAS_METHOD(name, call_name)                        \
    template<class T, class... TArgs>                               \
    decltype(std::declval<T>().call_name(std::declval<TArgs>()...)  \
           , std::true_type())                                      \
    has_##name##_impl(int);                                         \
                                                                    \
    template<class, class...>                                       \
    std::false_type has_##name##_impl(...);                         \
                                                                    \
    template<class T, class... TArgs>                               \
    using has_##name = decltype(has_##name##_impl<T, TArgs...>(0))

#if defined(__GNUC__) || defined(__clang__)
    #define BOOST_DI_UNUSED __attribute__((unused))
#else
    #define BOOST_DI_UNUSED
#endif

#if defined(__clang__)
    #define BOOST_DI_CFG_ERRORS_DESC_BEGIN \
        _Pragma("clang diagnostic push") \
        _Pragma("clang diagnostic error \"-Wundefined-inline\"") \
        _Pragma("clang diagnostic error \"-Wundefined-internal\"")

    #define BOOST_DI_CFG_ERRORS_DESC_END \
        _Pragma("clang diagnostic pop")

    #define BOOST_DI_ATTR_ERROR(...) [[deprecated(__VA_ARGS__)]]
#else
    #define BOOST_DI_CFG_ERRORS_DESC_BEGIN \
        _Pragma("GCC diagnostic push") \
        _Pragma("GCC diagnostic error \"-Werror\"")

    #define BOOST_DI_CFG_ERRORS_DESC_END \
        _Pragma("GCC diagnostic pop")

    #define BOOST_DI_ATTR_ERROR(...) __attribute__ ((error(__VA_ARGS__)))
#endif

namespace boost { namespace di { namespace aux {

template<class...>
using is_valid_expr = std::true_type;

template<class>
struct is_smart_ptr : std::false_type { };

template<class T, class TDeleter>
struct is_smart_ptr<std::unique_ptr<T, TDeleter>>
    : std::true_type
{ };

template<class T>
struct is_smart_ptr<std::shared_ptr<T>>
    : std::true_type
{ };

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class T>
    struct is_smart_ptr<boost::shared_ptr<T>>
        : std::true_type
    { };
#endif

template<class T>
struct is_smart_ptr<std::weak_ptr<T>>
    : std::true_type
{ };

template<class T, class... TArgs>
decltype(void(T{std::declval<TArgs>()...}), std::true_type{})
test_is_braces_constructible(int);

template<class, class...>
std::false_type test_is_braces_constructible(...);

template<class T, class... TArgs>
using is_braces_constructible =
    decltype(test_is_braces_constructible<T, TArgs...>(0));

template<class T, class... TArgs>
using is_braces_constructible_t =
    typename is_braces_constructible<T, TArgs...>::type;

template<class T>
using remove_accessors =
    std::remove_cv<std::remove_pointer_t<std::remove_reference_t<T>>>;

template<class T>
using remove_accessors_t = typename remove_accessors<T>::type;

template<class, class = void>
struct deref_type;

template<typename T>
using deref_type_t = typename deref_type<T>::type;

template<class T, class>
struct deref_type {
    using type = T;
};

template<class T>
struct deref_type<T, std::enable_if_t<is_smart_ptr<T>{}>> {
    using type = typename T::element_type;
};

template<class T>
using decay =
    deref_type<remove_accessors_t<deref_type_t<remove_accessors_t<T>>>>;

template<class T>
using decay_t = typename decay<T>::type;

template<class T1, class T2>
struct is_same_or_base_of {
    static constexpr auto value =
        std::is_same<aux::decay_t<T1>, aux::decay_t<T2>>::value ||
        std::is_base_of<aux::decay_t<T2>, aux::decay_t<T1>>::value;
};

template<class T>
struct function_traits
    : function_traits<decltype(&T::operator())>
{ };

template<class R, class... TArgs>
struct function_traits<R(*)(TArgs...)> {
    using result_type = R;
    using base_type = none_t;
    using args = type_list<TArgs...>;
};

template<class R, class... TArgs>
struct function_traits<R(TArgs...)> {
    using result_type = R;
    using base_type = none_t;
    using args = type_list<TArgs...>;
};

template<class R, class T, class... TArgs>
struct function_traits<R(T::*)(TArgs...)> {
    using result_type = R;
    using base_type = T;
    using args = type_list<TArgs...>;
};

template<class R, class T, class... TArgs>
struct function_traits<R(T::*)(TArgs...) const> {
    using result_type = R;
    using base_type = T;
    using args = type_list<TArgs...>;
};

} // aux

template<class, class, bool>
struct constraint_not_satisfied { };

template<class TError, class TReturn>
struct constraint_not_satisfied<TError, TReturn, true> {
    using type = TReturn;
};

template<bool B, class TError = void, class TReturn = int>
using REQUIRES = typename constraint_not_satisfied<TError, TReturn, B>::type;

template<class, class>
struct errors_impl;

template<class T>
struct errors_impl<std::false_type, T> {
    using type = T;
};

template<class T>
struct errors_impl<T, std::false_type> {
    using type = T;
};

template<>
struct errors_impl<std::false_type, std::false_type> {
    using type = int;
};

template<class... Ts>
using errors = typename errors_impl<Ts...>::type;

}} // boost::di

#endif

#ifndef BOOST_DI_CORE_POOL_HPP
#define BOOST_DI_CORE_POOL_HPP

namespace boost { namespace di { namespace core {

struct init { };

template<class = aux::type_list<>>
class pool;

template<class... TArgs>
class pool<aux::type_list<TArgs...>> : public TArgs... {
    template<class T, class TPool>
    using is_base_of_pool = std::integral_constant<
        bool
      , std::is_base_of<T, pool>{} && std::is_base_of<T, TPool>{}
    >;

public:
    template<class... Ts>
    explicit pool(const Ts&... args) noexcept
        : Ts(args)...
    { }

    template<class TPool>
    pool(const init&, const TPool& p) noexcept
        : pool(get_impl<TArgs>(p, is_base_of_pool<TArgs, TPool>{})...)
    { }

    template<class T>
    inline const T& get() const noexcept {
        return static_cast<const T&>(*this);
    }

private:
    template<class T, class TPool>
    inline const T& get_impl(const TPool& p, const std::true_type&) const noexcept {
        return static_cast<const T&>(p);
    }

    template<class T, class TPool>
    inline T get_impl(const TPool&, const std::false_type&) const noexcept {
        return {};
    }
};

}}} // boost::di::core

#endif

#ifndef BOOST_DI_WRAPPERS_UNIQUE_HPP
#define BOOST_DI_WRAPPERS_UNIQUE_HPP

#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace wrappers {

template<class T>
struct unique {
    template<class I>
    inline operator I() const noexcept {
        return object;
    }

    template<class I>
    inline operator I*() const noexcept { // only for compilation clean
        return {};
    }

    inline operator T&&() noexcept {
        return std::move(object);
    }

    T object;
};

template<class T>
struct unique<T*> {
    template<class I>
    inline operator I() const noexcept {
        return *std::unique_ptr<I>{object};
    }

    template<class I>
    inline operator I*() const noexcept {
        return object; // ownership transfer
    }

    template<class I>
    inline operator const I*() const noexcept {
        return object; // ownership transfer
    }

    template<class I>
    inline operator std::shared_ptr<I>() const noexcept {
        return std::shared_ptr<I>{object};
    }

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class I>
    inline operator boost::shared_ptr<I>() const noexcept {
        return boost::shared_ptr<I>{object};
    }
#endif

    template<class I>
    inline operator std::unique_ptr<I>() const noexcept {
        return std::unique_ptr<I>{object};
    }

    T* object = nullptr;
};

template<class T, class TDeleter>
struct unique<std::unique_ptr<T, TDeleter>> {
    template<class I>
    inline operator I() const noexcept {
        return *object;
    }

    template<class I>
    inline operator I*() noexcept {
        return object.release();
    }

    template<class I>
    inline operator const I*() noexcept {
        return object.release();
    }

    template<class I>
    inline operator std::shared_ptr<I>() noexcept {
        return {object.release(), object.get_deleter()};
    }

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class I>
    inline operator boost::shared_ptr<I>() noexcept {
        return {object.release(), object.get_deleter()};
    }
#endif

    template<class I, class D>
    inline operator std::unique_ptr<I, D>() noexcept {
        return std::move(object);
    }

    std::unique_ptr<T, TDeleter> object;
};

}}} // boost::di::wrappers

#endif

#ifndef BOOST_DI_TYPE_TRAITS_MEMORY_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_MEMORY_TRAITS_HPP

#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace type_traits {

struct stack { };
struct heap { };

template<class T, class = void>
struct memory_traits {
    using type = stack;
};

template<class T>
struct memory_traits<T&> {
    using type = stack;
};

template<class T>
struct memory_traits<const T&> {
    using type = stack;
};

template<class T>
struct memory_traits<T*> {
    using type = heap;
};

template<class T>
struct memory_traits<const T*> {
    using type = heap;
};

template<class T>
struct memory_traits<T&&> {
    using type = stack;
};

template<class T>
struct memory_traits<const T&&> {
    using type = stack;
};

template<class T, class TDeleter>
struct memory_traits<std::unique_ptr<T, TDeleter>> {
    using type = heap;
};

template<class T, class TDeleter>
struct memory_traits<const std::unique_ptr<T, TDeleter>&> {
    using type = heap;
};

template<class T>
struct memory_traits<std::shared_ptr<T>> {
    using type = heap;
};

template<class T>
struct memory_traits<const std::shared_ptr<T>&> {
    using type = heap;
};

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class T>
    struct memory_traits<boost::shared_ptr<T>> {
        using type = heap;
    };

    template<class T>
    struct memory_traits<const boost::shared_ptr<T>&> {
        using type = heap;
    };
#endif

template<class T>
struct memory_traits<std::weak_ptr<T>> {
    using type = heap;
};

template<class T>
struct memory_traits<const std::weak_ptr<T>&> {
    using type = heap;
};

template<class T>
struct memory_traits<T, std::enable_if_t<std::is_polymorphic<T>{}>> {
    using type = heap;
};

template<class T>
using memory_traits_t = typename memory_traits<T>::type;

}}} // boost::di::type_traits

#endif

#ifndef BOOST_DI_SCOPES_UNIQUE_HPP
#define BOOST_DI_SCOPES_UNIQUE_HPP

namespace boost { namespace di { namespace scopes {

class unique {
public:
    static constexpr auto priority = false;

    template<class, class>
    class scope {
    public:
        template<class T, class TProvider>
        auto create_(const TProvider& provider) const -> decltype(
            wrappers::unique<decltype(provider.get_(type_traits::memory_traits_t<T>{}))>{
                provider.get_(type_traits::memory_traits_t<T>{})
            }
        );

        template<class T, class TProvider>
        auto create(const TProvider& provider) const {
            using memory = type_traits::memory_traits_t<T>;
            using wrapper = wrappers::unique<decltype(provider.get(memory{}))>;
            return wrapper{provider.get(memory{})};
        }
    };
};

}}} // boost::di::scopes

#endif

#ifndef BOOST_DI_WRAPPERS_SHARED_HPP
#define BOOST_DI_WRAPPERS_SHARED_HPP

#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace wrappers {

template<class T>
struct shared {
    template<class I>
    inline operator std::shared_ptr<I>() const noexcept {
        return object;
    }

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class TSharedPtr>
    struct sp_holder {
        TSharedPtr object;

        void operator()(...) noexcept {
            object.reset();
        }
    };

    template<class I>
    inline operator boost::shared_ptr<I>() const noexcept {
        using sp = sp_holder<boost::shared_ptr<T>>;
        if (auto* deleter = std::get_deleter<sp, T>(object)) {
            return deleter->object;
        } else {
            return {object.get(), sp_holder<std::shared_ptr<T>>{object}};
        }
    }
#endif

    template<class I>
    inline operator std::weak_ptr<I>() const noexcept {
        return object;
    }

    std::shared_ptr<T> object;
};

}}} // boost::di::wrappers

#endif

#ifndef BOOST_DI_SCOPES_SINGLETON_HPP
#define BOOST_DI_SCOPES_SINGLETON_HPP

namespace boost { namespace di { namespace scopes {

class singleton {
public:
    static constexpr auto priority = false;

    template<class, class T>
    class scope {
    public:
        template<class, class TProvider>
        auto create_(const TProvider& provider) -> decltype(wrappers::shared<T>{std::shared_ptr<T>{provider.get_()}});

        template<class, class TProvider>
        auto create(const TProvider& provider) {
            if (!get_instance()) {
                get_instance() = std::shared_ptr<T>{provider.get()};
            }
            return wrappers::shared<T>{get_instance()};
        }

    private:
        static std::shared_ptr<T>& get_instance() noexcept {
            static std::shared_ptr<T> object;
            return object;
        }
    };
};

}}} // boost::di::scopes

#endif

#ifndef BOOST_DI_SCOPES_EXTERNAL_HPP
#define BOOST_DI_SCOPES_EXTERNAL_HPP

namespace boost { namespace di { namespace scopes {

BOOST_DI_HAS_TYPE(result_type);
BOOST_DI_HAS_METHOD(call_operator, operator());

template<class T, class U>
using is_lambda_expr =
    std::integral_constant<
        bool
      , has_call_operator<T, U>::value &&
       !has_result_type<T>::value
    >;

template<class T>
struct wrapper_traits {
    using type = wrappers::unique<T>;
};

template<class T>
struct wrapper_traits<std::shared_ptr<T>> {
    using type = wrappers::shared<T>;
};

template<class T>
using wrapper_traits_t =
    typename wrapper_traits<T>::type;

class external {
    struct injector {
        template<class T> T create() const;
    };

public:
    static constexpr auto priority = true;

    template<class TExpected, class, class = void>
    struct scope {
        template<class, class TProvider>
        TExpected create_(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return wrappers::unique<TExpected>{object_};
        }

        TExpected object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, std::reference_wrapper<TGiven>> {
        using is_ref = void;

        template<class, class TProvider>
        std::reference_wrapper<TGiven> create_(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return object_;
        }

        std::reference_wrapper<TGiven> object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, std::shared_ptr<TGiven>> {
        template<class, class TProvider>
        wrappers::shared<TGiven> create_(const TProvider&);

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
            !is_lambda_expr<TGiven, const injector&>{} &&
            !has_call_operator<TExpected>{} &&
             has_call_operator<TGiven>{}
        >
    > {
        template<class T, class TProvider>
        auto create_(const TProvider&) -> wrapper_traits_t<decltype(std::declval<TGiven>()())>;

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            using wrapper = wrapper_traits_t<decltype(std::declval<TGiven>()())>;
            return wrapper{object_()};
        }

        TGiven object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, TGiven, std::enable_if_t<is_lambda_expr<TGiven, const injector&>{}>> {
        template<class T, class TProvider>
        T create_(const TProvider&);

        template<class, class TProvider>
        auto create(const TProvider& provider) const noexcept {
            using wrapper = wrapper_traits_t<decltype((object_)(provider.injector_))>;
            return wrapper{(object_)(provider.injector_)};
        }

        TGiven object_;
    };
};

}}} // boost::di::scopes

#endif

#ifndef BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_SCOPE_TRAITS_HPP

#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace type_traits {

template<class T>
struct scope_traits {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<T&> {
    using type = scopes::external;
};

template<class T>
struct scope_traits<const T&> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<T*> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<const T*> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<T&&> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<const T&&> {
    using type = scopes::unique;
};

template<class T, class TDeleter>
struct scope_traits<std::unique_ptr<T, TDeleter>> {
    using type = scopes::unique;
};

template<class T, class TDeleter>
struct scope_traits<const std::unique_ptr<T, TDeleter>&> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<std::shared_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const std::shared_ptr<T>&> {
    using type = scopes::singleton;
};

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class T>
    struct scope_traits<boost::shared_ptr<T>> {
        using type = scopes::singleton;
    };

    template<class T>
    struct scope_traits<const boost::shared_ptr<T>&> {
        using type = scopes::singleton;
    };
#endif

template<class T>
struct scope_traits<std::weak_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const std::weak_ptr<T>&> {
    using type = scopes::singleton;
};

template<class T>
using scope_traits_t = typename scope_traits<T>::type;

}}} // boost::di::type_traits

#endif

#ifndef BOOST_DI_SCOPES_DEDUCE_HPP
#define BOOST_DI_SCOPES_DEDUCE_HPP

namespace boost { namespace di { namespace scopes {

class deduce {
public:
    static constexpr auto priority = false;

    template<class TExpected, class TGiven>
    class scope {
    public:
        template<class T, class TProvider>
        auto create_(const TProvider& provider) -> decltype(
            typename type_traits::scope_traits_t<T>::template
                scope<TExpected, TGiven>{}.template create_<T>(provider)
        );

        template<class T, class TProvider>
        auto create(const TProvider& provider) {
            using scope_traits = type_traits::scope_traits_t<T>;
            using scope = typename scope_traits::template scope<TExpected, TGiven>;
            return scope{}.template create<T>(provider);
        }
    };
};

}}} // boost::di::scopes

#endif

#ifndef BOOST_DI_SCOPES_EXPOSED_HPP
#define BOOST_DI_SCOPES_EXPOSED_HPP

namespace boost { namespace di { namespace scopes {

template<class TScope = scopes::deduce>
class exposed {
public:
    static constexpr auto priority = false;

    template<class TExpected, class TGiven>
    class scope {
        template<class T, class = void>
        struct iprovider {
            virtual ~iprovider() noexcept = default;
            virtual T* get(const type_traits::heap& = {}) const noexcept = 0;
            virtual T  get(const type_traits::stack&) const noexcept = 0;
        };

        template<class T>
        struct iprovider<T, std::enable_if_t<!std::is_copy_constructible<T>{}>> {
            virtual ~iprovider() noexcept = default;
            virtual T* get(const type_traits::heap& = {}) const noexcept { return nullptr; };
        };

        template<class T, class TInjector, class = void>
        class provider_impl : public iprovider<T> {
        public:
            explicit provider_impl(const TInjector& injector)
                : injector_(injector)
            { }

            T* get(const type_traits::heap&) const noexcept override {
                return injector_.template create_impl<T*>();
            }

            T get(const type_traits::stack&) const noexcept override {
                return injector_.template create_impl<T>();
            }

        private:
            TInjector injector_;
        };

        template<class T, class TInjector>
        class provider_impl<T, TInjector, std::enable_if_t<!std::is_copy_constructible<T>{}>>
            : public iprovider<T> {
        public:
            explicit provider_impl(const TInjector& injector)
                : injector_(injector)
            { }

            T* get(const type_traits::heap&) const noexcept override {
                return injector_.template create_impl<T*>();
            }

        private:
            TInjector injector_;
        };

    public:
        template<class TInjector>
        explicit scope(const TInjector& injector) noexcept
            : provider_{std::make_shared<provider_impl<TExpected, TInjector>>(injector)}
        { }

        template<class T, class TProvider>
        T create_(const TProvider&);

        template<class T, class TProvider>
        auto create(const TProvider&) {
            return scope_.template create<T>(*provider_);
        }

    private:
        std::shared_ptr<iprovider<TExpected>> provider_;
        typename TScope::template scope<TExpected, TGiven> scope_;
    };
};

}}} // boost::di::scopes

#endif

#ifndef BOOST_DI_FWD_HPP
#define BOOST_DI_FWD_HPP

namespace boost { namespace di {
namespace aux { struct none_t; };
namespace core {
template<class = void, class = aux::none_t, class = std::false_type>
struct any_type;
} // core
namespace providers {
class heap;
class stack_over_heap;
} // providers

struct no_name {
    const char* operator()() const noexcept { return nullptr; }
};

template<class = void>
class config;

template<class...>
class injector;

}} // boost::di

#endif

#ifndef BOOST_DI_CORE_DEPENDENCY_HPP
#define BOOST_DI_CORE_DEPENDENCY_HPP

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(configure, configure);
BOOST_DI_HAS_TYPE(deps);

template<class T, class U = std::remove_reference_t<T>>
using is_injector =
    std::integral_constant<bool, has_deps<U>{} || has_configure<U>{}>;

template<class, class>
struct dependency_concept { };

template<class T, class TDependency>
struct dependency_impl : aux::pair<T, TDependency>
{ };

template<class... Ts, class TName, class TDependency>
struct dependency_impl<
    dependency_concept<aux::type_list<Ts...>, TName>
  , TDependency
> : aux::pair<dependency_concept<Ts, TName>, TDependency>...
{ };

template<
    class TScope
  , class TExpected
  , class TGiven = TExpected
  , class TName = no_name
  , class TPriority = std::integral_constant<bool, TScope::priority>
> class dependency;

struct dependency_ { };

template<class I, class Impl>
struct bind : dependency<scopes::deduce, I, Impl> {
    template<class T>
    struct named_ : dependency<scopes::deduce, I, Impl, T> {
        using dependency<scopes::deduce, I, Impl, T>::dependency;
    };
};

template<
    class TScope
  , class TExpected
  , class TGiven
  , class TName
  , class TPriority
> class dependency
    : public TScope::template scope<TExpected, TGiven>
    , public dependency_
    , public dependency_impl<
          dependency_concept<TExpected, TName>
        , dependency<TScope, TExpected, TGiven, TName, TPriority>
      > {
    using scope_t = typename TScope::template scope<TExpected, TGiven>;

public:
    using scope = TScope;
    using expected = TExpected;
    using given = TGiven;
    using name = TName;

    dependency() noexcept { }

    template<class T>
    explicit dependency(T&& object) noexcept
        : scope_t{std::forward<T>(object)}
    { }

    template<class... Ts>
    dependency(const dependency<Ts...>& other) noexcept
        : scope_t(other)
    { }

    template<class T>
    auto named(const T&) const noexcept {
        //return typename bind<TExpected, TGiven>::template named_<T>{*this};
        return dependency<TScope, TExpected, TGiven, T>{*this};
    }

    template<class T>
    auto in(const T&) const noexcept {
        return dependency<T, TExpected, TGiven, TName>{};
    }

    template<class T
           , REQUIRES<!is_injector<T>{} &&
                      std::is_same<TExpected, TGiven>{} &&
                      std::is_same<TScope, scopes::deduce>{}> = 0
    > auto to(T&& object) const noexcept {
        using dependency = dependency<
            scopes::external, TExpected, std::remove_reference_t<T>, TName
        >;
        return dependency{std::forward<T>(object)};
    }

    template<class T, REQUIRES<has_configure<T>{}> = 0>
    auto to(const T& object) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>, TExpected, decltype(std::declval<T>().configure()), TName
        >;
        return dependency{object.configure()};
    }

    template<class T, REQUIRES<has_deps<T>{}> = 0>
    auto to(const T& object) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>, TExpected, T, TName
        >;
        return dependency{object};
    }
};

template<class T>
struct is_dependency : std::is_base_of<dependency_, T> { };

}}} // boost::di::core

#endif

#ifndef BOOST_DI_CONCEPTS_CALLABLE_HPP
#define BOOST_DI_CONCEPTS_CALLABLE_HPP

namespace boost { namespace di { namespace concepts {

struct arg {
    using type = void;
    using name = no_name;
    using is_root = std::false_type;

    template<class, class, class>
    struct resolve;
};

struct ctor { };

std::false_type callable_impl(...);

template<class T, class TArg>
auto callable_impl(T&& t, TArg&& arg) -> aux::is_valid_expr<
    decltype(t(arg))
>;

template<class T, class TArg, class TDependency, class... TCtor>
auto callable_impl(T&& t, TArg&& arg, TDependency&& dep, TCtor&&... ctor) -> aux::is_valid_expr<
    decltype(t(arg, dep, ctor...))
>;

template<class... T>
constexpr auto callable() {
    return std::is_same<
        aux::bool_list<aux::always<T>{}...>
      , aux::bool_list<(
            decltype(callable_impl(std::declval<T>(), arg{})){} ||
            decltype(callable_impl(std::declval<T>(), arg{}, core::dependency<scopes::deduce, T>{}, ctor{})){})...
        >
    >{};
}

}}} // boost::di::concepts

#endif

#ifndef BOOST_DI_AUX_PREPROCESSOR_HPP
#define BOOST_DI_AUX_PREPROCESSOR_HPP

// Based on:
//    Boost.Preprocessor: http://www.boost.org/doc/libs/1_57_0/libs/preprocessor
//    Cloak: https://github.com/pfultz2/Cloak

#define BOOST_DI_CAT(a, ...) BOOST_DI_PRIMITIVE_CAT(a, __VA_ARGS__)
#define BOOST_DI_CALL(m, ...) m(__VA_ARGS__)
#define BOOST_DI_EMPTY()
#define BOOST_DI_COMMA() ,
#define BOOST_DI_EAT(...)
#define BOOST_DI_EXPAND(...) __VA_ARGS__
#define BOOST_DI_NARG_(...) BOOST_DI_ARG_N(__VA_ARGS__)
#define BOOST_DI_NARG(...) BOOST_DI_NARG_(__VA_ARGS__, BOOST_DI_RSEQ_N())
#define BOOST_DI_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define BOOST_DI_IBP_SPLIT(i, ...) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_IBP_SPLIT_, i)(__VA_ARGS__)
#define BOOST_DI_IBP_SPLIT_0(a, ...) a
#define BOOST_DI_IBP_SPLIT_1(a, ...) __VA_ARGS__
#define BOOST_DI_IBP_IS_VARIADIC_C(...) 1
#define BOOST_DI_IBP_IS_VARIADIC_R_1 1,
#define BOOST_DI_IBP_IS_VARIADIC_R_BOOST_DI_IBP_IS_VARIADIC_C 0,
#define BOOST_DI_IBP(...) \
    BOOST_DI_IBP_SPLIT(0, BOOST_DI_CAT( \
        BOOST_DI_IBP_IS_VARIADIC_R_, BOOST_DI_IBP_IS_VARIADIC_C __VA_ARGS__) \
    )
#define BOOST_DI_TRIGGER_PARENTHESIS(...) ,
#define BOOST_DI_IS_EMPTY(...)  \
    BOOST_DI_IS_EMPTY_IMPL(BOOST_DI_HAS_COMMA(__VA_ARGS__) \
           , BOOST_DI_HAS_COMMA(BOOST_DI_TRIGGER_PARENTHESIS __VA_ARGS__) \
           , BOOST_DI_HAS_COMMA(__VA_ARGS__ ()) \
           , BOOST_DI_HAS_COMMA(BOOST_DI_TRIGGER_PARENTHESIS __VA_ARGS__ ()))
#define BOOST_DI_IS_EMPTY_IMPL(_0, _1, _2, _3) \
    BOOST_DI_HAS_COMMA(BOOST_DI_JOIN_5(BOOST_DI_IS_EMPTY_IMPL_, _0, _1, _2, _3))
#define BOOST_DI_DEFER(id) id BOOST_DI_EMPTY()
#define BOOST_DI_OBSTRUCT(...) __VA_ARGS__ BOOST_DI_DEFER(BOOST_DI_EMPTY)()
#define BOOST_DI_EVAL(...) BOOST_DI_EVAL1(BOOST_DI_EVAL1(BOOST_DI_EVAL1(__VA_ARGS__)))
#define BOOST_DI_EVAL1(...) BOOST_DI_EVAL2(BOOST_DI_EVAL2(BOOST_DI_EVAL2(__VA_ARGS__)))
#define BOOST_DI_EVAL2(...) __VA_ARGS__
#define BOOST_DI_CHECK_N(x, n, ...) n
#define BOOST_DI_CHECK(...) BOOST_DI_CHECK_N(__VA_ARGS__, 0,)
#define BOOST_DI_NOT(x) BOOST_DI_CHECK(BOOST_DI_PRIMITIVE_CAT(BOOST_DI_NOT_, x))
#define BOOST_DI_NOT_0 ~, 1,
#define BOOST_DI_COMPL(b) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_COMPL_, b)
#define BOOST_DI_COMPL_0 1
#define BOOST_DI_COMPL_1 0
#define BOOST_DI_BOOL(x) BOOST_DI_COMPL(BOOST_DI_NOT(x))
#define BOOST_DI_IF_IMPL(c) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_IF_IMPL_, c)
#define BOOST_DI_IF_IMPL_0(t, ...) __VA_ARGS__
#define BOOST_DI_IF_IMPL_1(t, ...) t
#define BOOST_DI_IF(c) BOOST_DI_IF_IMPL(BOOST_DI_BOOL(c))
#define BOOST_DI_JOIN_5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define BOOST_DI_IS_EMPTY_IMPL_0001 ,
#define BOOST_DI_REPEAT(n, m, ...) \
    BOOST_DI_IF(n)(BOOST_DI_EXPAND, BOOST_DI_EAT)( \
        BOOST_DI_OBSTRUCT(BOOST_DI_REPEAT_INDIRECT)()( \
            BOOST_DI_DEC(n), m, __VA_ARGS__ \
        ) \
    BOOST_DI_OBSTRUCT(m)( \
        BOOST_DI_DEC(n), __VA_ARGS__ ) \
    )
#define BOOST_DI_REPEAT_INDIRECT() BOOST_DI_REPEAT
#define BOOST_DI_VARARG_IMPL2(m, n, ...) m##n(__VA_ARGS__)
#define BOOST_DI_VARARG_IMPL(m, n, ...) BOOST_DI_VARARG_IMPL2(m, n, __VA_ARGS__)
#define BOOST_DI_ARG_N( _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, N, ...) N
#define BOOST_DI_RSEQ_N() 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define BOOST_DI_HAS_COMMA(...) BOOST_DI_ARG_N(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0)
#define BOOST_DI_ARG0(p1, ...) p1
#define BOOST_DI_ARG1(p1, p2, ...) p2
#define BOOST_DI_ARG2(p1, p2, p3, ...) p3
#define BOOST_DI_ARG3(p1, p2, p3, p4, ...) p4
#define BOOST_DI_ARG4(p1, p2, p3, p4, p5, ...) p5
#define BOOST_DI_ARG5(p1, p2, p3, p4, p5, p6, ...) p6
#define BOOST_DI_ARG6(p1, p2, p3, p4, p5, p6, p7, ...) p7
#define BOOST_DI_ARG7(p1, p2, p3, p4, p5, p6, p7, p8, ...) p8
#define BOOST_DI_ARG8(p1, p2, p3, p4, p5, p6, p7, p8, p9, ...) p9
#define BOOST_DI_ARG9(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, ...) p10
#define BOOST_DI_DEC(x) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_DEC_, x)
#define BOOST_DI_DEC_0 0
#define BOOST_DI_DEC_1 0
#define BOOST_DI_DEC_2 1
#define BOOST_DI_DEC_3 2
#define BOOST_DI_DEC_4 3
#define BOOST_DI_DEC_5 4
#define BOOST_DI_DEC_6 5
#define BOOST_DI_DEC_7 6
#define BOOST_DI_DEC_8 7
#define BOOST_DI_DEC_9 8
#define BOOST_DI_DEC_10 9

#endif

#ifndef BOOST_DI_INJECT_HPP
#define BOOST_DI_INJECT_HPP

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

#define BOOST_DI_GEN_ARGS(i, ...) \
    BOOST_DI_GEN_ARGS_IMPL(BOOST_DI_VARARG_IMPL(BOOST_DI_ARG, i, __VA_ARGS__,), i)

#define BOOST_DI_GEN_ARGS_IMPL(p, i) \
    struct arg##i { \
        BOOST_DI_IF(BOOST_DI_IBP(p))( \
            BOOST_DI_GEN_NAME_ARG p BOOST_DI_GEN_NAME_ARG_END() \
          , BOOST_DI_GEN_ARG(p)) \
    };

#define BOOST_DI_GEN_NAME_ARG(p) \
    static auto BOOST_DI_CAT(BOOST_DI_INJECTOR, name)() { \
        auto p; return named; \
    } static void BOOST_DI_CAT(BOOST_DI_INJECTOR, arg)(

#define BOOST_DI_GEN_NAME_ARG_END() );

#define BOOST_DI_GEN_ARG(p) \
    static void BOOST_DI_CAT(BOOST_DI_INJECTOR, arg)(p);

#define BOOST_DI_GEN_CTOR(i, ...) \
    BOOST_DI_GEN_CTOR_IMPL( \
        BOOST_DI_VARARG_IMPL(BOOST_DI_ARG, i, __VA_ARGS__,) \
      , i \
    )

#define BOOST_DI_GEN_CTOR_IMPL(p, i) \
    BOOST_DI_IF(i)(BOOST_DI_COMMA(),) \
    BOOST_DI_IF(BOOST_DI_IBP(p))(BOOST_DI_EAT p, p)

#define BOOST_DI_GEN_TYPE_LIST(i, ...) \
    BOOST_DI_GEN_TYPE_LIST_IMPL( \
        BOOST_DI_VARARG_IMPL(BOOST_DI_ARG, i, __VA_ARGS__,) \
      , i \
    )

#define BOOST_DI_GEN_TYPE_LIST_IMPL(p, n) \
    BOOST_DI_IF(n)(BOOST_DI_COMMA(),) \
    BOOST_DI_IF(BOOST_DI_IBP(p))( \
        const ::boost::di::aux::type<arg##n BOOST_DI_COMMA() ::std::true_type>& \
      , BOOST_DI_IF(BOOST_DI_IS_EMPTY(p))( \
            , const ::boost::di::aux::type<arg##n BOOST_DI_COMMA() ::std::false_type>& \
        ) \
    )

#if !defined(BOOST_DI_INJECT_TRAITS)
    #define BOOST_DI_INJECT_TRAITS(...) \
        struct BOOST_DI_INJECTOR { \
            BOOST_DI_EVAL( \
                BOOST_DI_REPEAT( \
                    BOOST_DI_NARG(__VA_ARGS__) \
                  , BOOST_DI_GEN_ARGS \
                  , __VA_ARGS__ \
                ) \
            ) \
            using type BOOST_DI_UNUSED = ::boost::di::aux::type_list< \
                BOOST_DI_EVAL( \
                    BOOST_DI_REPEAT( \
                        BOOST_DI_NARG(__VA_ARGS__) \
                      , BOOST_DI_GEN_TYPE_LIST \
                      , __VA_ARGS__ \
                    ) \
                ) \
            >; \
            static_assert( \
                BOOST_DI_NARG(__VA_ARGS__) <= BOOST_DI_CFG_CTOR_LIMIT_SIZE \
              , "Number of constructor arguments is out of range - see BOOST_DI_CFG_CTOR_LIMIT_SIZE" \
            ); \
        }
#endif

#if !defined(BOOST_DI_INJECT_TRAITS_NO_LIMITS)
    #define BOOST_DI_INJECT_TRAITS_NO_LIMITS(...) \
        struct BOOST_DI_INJECTOR { \
            static void inject(__VA_ARGS__); \
            using type BOOST_DI_UNUSED = typename ::boost::di::aux::function_traits<decltype(inject)>::args; \
        }
#endif

#if !defined(BOOST_DI_INJECT)
    #define BOOST_DI_INJECT(type, ...) \
        BOOST_DI_INJECT_TRAITS(__VA_ARGS__); \
        type(BOOST_DI_EVAL( \
            BOOST_DI_REPEAT( \
                BOOST_DI_NARG(__VA_ARGS__) \
              , BOOST_DI_GEN_CTOR \
              , __VA_ARGS__) \
            ) \
        )
#endif

#endif

#ifndef BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

namespace boost { namespace di { namespace type_traits {

template<class, class>
struct named { };
struct direct { };
struct uniform { };

BOOST_DI_CALL(BOOST_DI_HAS_TYPE, BOOST_DI_INJECTOR);

template<class T, std::size_t>
using type = T;

template<template<class...> class, class, class>
struct ctor_impl;

template<
    template<class...> class TIsConstructible
  , class T
  , std::size_t... TArgs
> struct ctor_impl<TIsConstructible, T, std::index_sequence<TArgs...>>
    : std::conditional<
          TIsConstructible<T, type<core::any_type<T>, TArgs>...>::value
        , aux::type_list<type<core::any_type<T>, TArgs>...>
        , typename ctor_impl<
              TIsConstructible
            , T
            , std::make_index_sequence<sizeof...(TArgs) - 1>
          >::type
      >
{ };

template<template<class...> class TIsConstructible, class T>
struct ctor_impl<TIsConstructible, T, std::index_sequence<>>
    : aux::type_list<>
{ };

template<template<class...> class TIsConstructible, class T>
using ctor_impl_t =
    typename ctor_impl<
        TIsConstructible
      , T
      , std::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE>
    >::type;

template<class...>
struct ctor;

template<class T>
struct ctor<T, aux::type_list<>>
    : aux::pair<uniform, ctor_impl_t<aux::is_braces_constructible, T>>
{ };

template<class T, class... TArgs>
struct ctor<T, aux::type_list<TArgs...>>
    : aux::pair<direct, aux::type_list<TArgs...>>
{ };

} // type_traits

template<class T>
struct ctor_traits
    : type_traits::ctor<T, type_traits::ctor_impl_t<std::is_constructible, T>>
{ };

namespace type_traits {

template<class>
struct parse_args;

template<class T>
struct arg {
    using type = T;
};

template<class>
struct arg_impl;

template<class T>
struct arg_impl<aux::type_list<T>> {
	using type = T;
};

template<class T>
using arg_impl_t = typename arg_impl<T>::type;

template<class T>
struct arg<const aux::type<T, std::true_type>&> {
	using type = named<
        typename aux::function_traits<
            decltype(T::BOOST_DI_CAT(BOOST_DI_INJECTOR, name))
        >::result_type
      , arg_impl_t<typename aux::function_traits<
            decltype(T::BOOST_DI_CAT(BOOST_DI_INJECTOR, arg))
        >::args>
    >;
};

template<class T>
struct arg<const aux::type<T, std::false_type>&> {
    using type = arg_impl_t<typename aux::function_traits<
        decltype(T::BOOST_DI_CAT(BOOST_DI_INJECTOR, arg))
    >::args>;
};

template<class... Ts>
struct parse_args<aux::type_list<Ts...>>
    : aux::type_list<typename arg<Ts>::type...>
{ };

template<class... Ts>
using parse_args_t = typename parse_args<Ts...>::type;

template<
    class T
  , class = typename BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<T>::type
> struct ctor_traits;

template<
    class T
  , class = typename BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T>>::type
> struct ctor_traits_impl;

template<class T>
struct ctor_traits<T, std::true_type>
    : aux::pair<direct, parse_args_t<typename T::BOOST_DI_INJECTOR::type>>
{ };

template<class T>
struct ctor_traits<T, std::false_type>
    : ctor_traits_impl<T>
{ };

template<class T>
struct ctor_traits_impl<T, std::true_type>
    : aux::pair<
          direct
        , parse_args_t<typename di::ctor_traits<T>::BOOST_DI_INJECTOR::type>
      >
{ };

template<class T>
struct ctor_traits_impl<T, std::false_type>
    : di::ctor_traits<T>
{ };

}}} // boost::di::type_traits

#if (__has_include(<string>))
    #include <string>

    namespace boost { namespace di {
        template<
            class T
          , class Traits
          , class TAllocator
        > struct ctor_traits<std::basic_string<T, Traits, TAllocator>> {
            BOOST_DI_INJECT_TRAITS();
        };
    }} // boost::di

#endif

#if (__has_include(<initializer_list>))
    #include <initializer_list>

    namespace boost { namespace di {
        template<class T>
        struct ctor_traits<std::initializer_list<T>> {
            BOOST_DI_INJECT_TRAITS();
        };
    }} // boost::di

#endif

#endif

#ifndef BOOST_DI_CONCEPTS_CREATABLE_HPP
#define BOOST_DI_CONCEPTS_CREATABLE_HPP

namespace boost { namespace di {

BOOST_DI_CFG_ERRORS_DESC_BEGIN

template<class T>
struct polymorphic_type {
struct is_not_bound {
    constexpr operator T*() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr T*
    constraint_not_satisfied(_ = "type not bound, did you forget to add: 'di::bind<interface, implementation>'?")
    const;
};

template<class TName>
struct named {
struct is_not_bound {
    constexpr operator T*() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr T*
    constraint_not_satisfied(_ = "type not bound, did you forget to add: 'di::bind<interface, implementation>.named(name)'?")
    const;
};};};

template<class T, class TParent>
using is_not_same = std::enable_if_t<!aux::is_same_or_base_of<T, TParent>::value>;

template<class TParent>
struct type_ {
    template<class T, class = is_not_same<T, TParent>>
    constexpr operator T(){
        return {};
    }

    template<class T, class = is_not_same<T, TParent>>
    constexpr operator T&() const{
        return
            constraint_not_satisfied_for
        <T&>();
    }

    template<class T> constexpr T
    constraint_not_satisfied_for(_ = "reference type not bound, did you forget to add `di::bind<T>.to([c]ref(value))`, notice that `di::bind<T>.to(value)` won't work!")
    const;
};

template<class T, class>
struct Any {
    using type = type_<T>;
};

template<class T, class... TCtor>
struct type {
struct is_not_creatable {
    constexpr operator T*() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr T*
    constraint_not_satisfied(_ = "type is not creatable'?")
    const;
};

template<class, class = void>
struct args;

template<class TDummy>
struct args<type_traits::direct, TDummy> {
    constexpr operator T*() const {
        return impl<T>();
    }

    template<class T_>
    T_* impl() const {
        return new T{typename Any<T_, TCtor>::type{}...};
    }
};

template<class TDummy>
struct args<type_traits::uniform, TDummy> {
    constexpr operator T*() const {
        return impl<T>();
    }

    template<class T_, std::enable_if_t<aux::is_braces_constructible<T_, TCtor...>{}, int> = 0>
    T_* impl() const {
        return new T_{typename Any<T_, TCtor>::type{}...};
    }

    template<class T_, std::enable_if_t<!aux::is_braces_constructible<T_, TCtor...>{}, int> = 0>
    T_* impl() const {
        return nullptr;
    }
};

template<class To>
struct is_not_convertible_to {
    constexpr operator To() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr To
    constraint_not_satisfied(_ = "type not convertible, missing 'di::bind<type>.to(ref(value))'")
    const;
};};

template<class T>
struct number_of_constructor_arguments_doesnt_match_for {
template<int Given> struct given {
template<int Expected> struct expected {
    constexpr operator T*() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr T*
    constraint_not_satisfied(_ = "verify BOOST_DI_INJECT_TRAITS or di::ctor_traits")
    const;
};};};

template<class T>
struct number_of_constructor_arguments_is_out_of_range_for {
template<int TMax> struct max {
    constexpr operator T*() const {
        return
            constraint_not_satisfied
        ();
    }

    constexpr T*
    constraint_not_satisfied(_ = "increase BOOST_DI_CFG_CTOR_LIMIT_SIZE value or reduce number of constructor parameters")
    const;
};};

template<class>
struct allocating_an_object_of_abastract_class_type {
    struct has_unimplemented_pure_virtual_methods { };
};

BOOST_DI_CFG_ERRORS_DESC_END

namespace concepts {

template<class>
struct ctor_size;

template<class TInit, class... TCtor>
struct ctor_size<aux::pair<TInit, aux::type_list<TCtor...>>>
    : std::integral_constant<int, sizeof...(TCtor)>
{ };

template<class, class, class, class>
struct creatable_error_impl;

template<class T>
using ctor_size_t = ctor_size<
    typename type_traits::ctor<
        aux::decay_t<T>
      , type_traits::ctor_impl_t<std::is_constructible, aux::decay_t<T>>
    >::type
>;

template<class TInitialization, class TName, class T, class... TCtor>
struct creatable_error_impl<TInitialization, TName, T, aux::type_list<TCtor...>>
    : std::conditional_t<
          std::is_polymorphic<aux::decay_t<T>>{}
        , std::conditional_t<std::is_same<TName, no_name>{}, typename polymorphic_type<aux::decay_t<T>>::is_not_bound, typename polymorphic_type<aux::decay_t<T>>::template named<TName>::is_not_bound>
        , std::conditional_t<
              ctor_size_t<T>{} == sizeof...(TCtor)
            , std::conditional_t<
                  !sizeof...(TCtor)
                , typename number_of_constructor_arguments_is_out_of_range_for<aux::decay_t<T>>::
                      template max<BOOST_DI_CFG_CTOR_LIMIT_SIZE>
                , typename type<aux::decay_t<T>, TCtor...>::template args<TInitialization>
              >
            , typename number_of_constructor_arguments_doesnt_match_for<aux::decay_t<T>>::
                  template given<sizeof...(TCtor)>::
                  template expected<ctor_size_t<T>{}>
          >
      >
{ };

template<class TInitialization, class T, class... Ts>
constexpr auto creatable() {
    return std::conditional_t<
        std::is_same<TInitialization, type_traits::uniform>{}
      , aux::is_braces_constructible<T, Ts...>
      , std::is_constructible<T, Ts...>
    >{};
}

template<class TInitialization, class TName, class T, class... Ts>
constexpr T creatable_error() {
    return creatable_error_impl<TInitialization, TName, T, aux::type_list<Ts...>>{};
}

auto creatable_impl_(...) -> std::false_type;

template<class T, class B, class N, class TIsRoot>
auto creatable_impl_(T&& t, B&&, N&&, TIsRoot&&) -> aux::is_valid_expr<decltype(t.template create_impl_<B, N, TIsRoot>())>;

template<class T, class B, class N = no_name, class TIsRoot = std::false_type>
constexpr auto creatable_() {
    return decltype(creatable_impl_(std::declval<T>(), std::declval<B>(), std::declval<N>(), std::declval<TIsRoot>())){};
}

}}} // boost::di::concepts

#endif

#ifndef BOOST_DI_PROVIDERS_STACK_OVER_HEAP_HPP
#define BOOST_DI_PROVIDERS_STACK_OVER_HEAP_HPP

namespace boost { namespace di { namespace providers {

class stack_over_heap {
public:
    template<class T, class TInitialization, class, class... TArgs>
    static constexpr auto is_creatable() {
        return concepts::creatable<TInitialization, T, TArgs...>();
    }

    template<class, class T, class... TArgs>
    auto get(const type_traits::direct&
           , const type_traits::heap&
           , TArgs&&... args) {
        return new T(std::forward<TArgs>(args)...);
    }

    template<class, class T, class... TArgs>
    auto get(const type_traits::uniform&
           , const type_traits::heap&
           , TArgs&&... args) {
        return new T{std::forward<TArgs>(args)...};
    }

    template<class, class T, class... TArgs>
    auto get(const type_traits::direct&
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T(std::forward<TArgs>(args)...);
    }

    template<class, class T, class... TArgs>
    auto get(const type_traits::uniform&
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T{std::forward<TArgs>(args)...};
    }
};

}}} // boost::di::providers

#endif


#ifndef BOOST_DI_CONFIG_HPP
#define BOOST_DI_CONFIG_HPP

#if defined(BOOST_DI_CFG)
    template<class = void>
    class BOOST_DI_CFG;
#else
    #define BOOST_DI_CFG boost::di::config
#endif

namespace boost { namespace di {

template<class... TPolicies, REQUIRES<concepts::callable<TPolicies...>()> = 0>
inline auto make_policies(const TPolicies&... args) noexcept {
    return core::pool<aux::type_list<TPolicies...>>(args...);
}

template<class>
class config {
public:
    auto provider() const noexcept {
        return providers::stack_over_heap{};
    }

    auto policies() noexcept {
        return make_policies();
    }
};

}} // boost::di

#endif

#ifndef BOOST_DI_SCOPES_SESSION_HPP
#define BOOST_DI_SCOPES_SESSION_HPP

namespace boost { namespace di { namespace scopes {

template<class = no_name>
class session_entry { };

template<class = no_name>
class session_exit { };

template<class TName = no_name>
class session {
public:
    static constexpr auto priority = false;

    template<class, class T>
    class scope {
    public:
        void call(const session_entry<TName>&) noexcept {
            in_scope_ = true;
        }

        void call(const session_exit<TName>&) noexcept {
            in_scope_ = false;
            object_.reset();
        }

        template<class, class TProvider>
        auto create_(const TProvider& provider) -> decltype(wrappers::shared<T>{std::shared_ptr<T>{provider.get_()}});

        template<class, class TProvider>
        auto create(const TProvider& provider) {
            if (in_scope_ && !object_) {
                object_ = std::shared_ptr<T>{provider.get()};
            }
            return wrappers::shared<T>{object_};
        }

    private:
        std::shared_ptr<T> object_;
        bool in_scope_ = false;
    };
};

}}} // boost::di::scopes

#endif

#ifndef BOOST_DI_SCOPES_SHARED_HPP
#define BOOST_DI_SCOPES_SHARED_HPP

namespace boost { namespace di { namespace scopes {

class shared {
public:
    static constexpr auto priority = false;

    template<class, class T>
    class scope {
    public:
        template<class, class TProvider>
        auto create_(const TProvider& provider) -> decltype(wrappers::shared<T>{std::shared_ptr<T>{provider.get_()}});

        template<class, class TProvider>
        auto create(const TProvider& provider) {
            if (!object_) {
                object_ = std::shared_ptr<T>{provider.get()};
            }
            return wrappers::shared<T>{object_};
        }

    private:
        std::shared_ptr<T> object_;
    };
};

}}} // boost::di::scopes

#endif

#ifndef BOOST_DI_CORE_BINDER_HPP
#define BOOST_DI_CORE_BINDER_HPP

namespace boost { namespace di { namespace core {

class binder {
    template<class TDefault, class>
    static TDefault resolve_impl(...) noexcept {
        return {};
    }

    template<class, class TConcept, class TDependency>
    static decltype(auto)
    resolve_impl(aux::pair<TConcept, TDependency>* dep) noexcept {
        return static_cast<TDependency&>(*dep);
    }

    template<
        class
      , class TConcept
      , class TScope
      , class TExpected
      , class TGiven
      , class TName
    > static decltype(auto) // priority scope
    resolve_impl(aux::pair<TConcept
               , dependency<TScope, TExpected, TGiven, TName, std::true_type>>* dep) noexcept {
        return static_cast<dependency<TScope, TExpected, TGiven, TName, std::true_type>&>(*dep);
    }

public:
    template<
        class T
      , class TName = no_name
      , class TDefault = dependency<scopes::deduce, aux::decay_t<T>>
      , class TDeps = void
    > static decltype(auto) resolve(TDeps* deps) noexcept {
        using dependency = dependency_concept<aux::decay_t<T>, TName>;
        return resolve_impl<TDefault, dependency>(deps);
    }
};

}}} // boost::di::core

#endif

#ifndef BOOST_DI_CORE_ANY_TYPE_HPP
#define BOOST_DI_CORE_ANY_TYPE_HPP

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_TYPE(is_ref);

template<class TParent, class TInjector, class TError>
struct any_type {
    template<class T>
    struct is_ref_impl {
        static constexpr auto value =
            std::is_same<TInjector, aux::none_t>::value ||
            has_is_ref<
                std::remove_reference_t<decltype(binder::resolve<T>((TInjector*)nullptr))>
            >::value;
    };

    template<class T>
    using is_ref = std::enable_if_t<is_ref_impl<T>::value>;

    template<class T>
    using is_not_same = std::enable_if_t<!aux::is_same_or_base_of<T, TParent>::value>;

    template<class T, class TError_>
    struct is_creatable_impl {
        static constexpr auto value =
            std::is_same<TInjector, aux::none_t>::value || concepts::creatable_<TInjector, T>();
    };

    template<class T>
    struct is_creatable_impl<T, std::false_type> {
        static constexpr auto value = true;
    };

    template<class T, class TError_>
    using is_creatable = std::enable_if_t<is_creatable_impl<T, TError_>::value>;

    template<class T, class = is_not_same<T>, class = is_creatable<T, TError>>
    operator T() {
        return injector_.template create_impl<T>();
    }

    template<class T, class = is_not_same<T>, class = is_ref<T>, class = is_creatable<T&, TError>>
    operator T&() const {
        return injector_.template create_impl<T&>();
    }

#if !defined(__clang__)
    template<class T, class = is_not_same<T>, class = is_ref<T>, class = is_creatable<T&&, TError>>
    operator T&&() const {
        return injector_.template create_impl<T&&>();
    }
#endif

    template<class T, class = is_not_same<T>, class = is_ref<T>, class = is_creatable<const T&, TError>>
    operator const T&() const {
        return injector_.template create_impl<const T&>();
    }

    const TInjector& injector_;
};

template<class>
struct is_any_type : std::false_type { };

template<class... TArgs>
struct is_any_type<any_type<TArgs...>> : std::true_type { };

}}} // boost::di::core

#endif

#ifndef BOOST_DI_CORE_POLICY_HPP
#define BOOST_DI_CORE_POLICY_HPP

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(call_operator, operator());

template<
    class T
  , class TName
  , class TIsRoot
  , class TDeps
> struct arg_wrapper {
    using type BOOST_DI_UNUSED = T;
    using name BOOST_DI_UNUSED = TName;
    using is_root BOOST_DI_UNUSED = TIsRoot;

    template<class T_, class TName_, class TDefault_>
    using resolve =
        decltype(core::binder::resolve<T_, TName_, TDefault_>((TDeps*)nullptr));
};

template<class TDeps>
class policy {
public:
    template<
        class T
      , class TName
      , class TIsRoot
      , class TInitialization
      , class TDependency
      , class... TCtor
      , class... TPolicies
    > static auto call(const pool<aux::type_list<TPolicies...>>& policies
                     , TDependency dependency
                     , aux::pair<TInitialization, aux::type_list<TCtor...>>) noexcept {

        int _[]{0, (call_impl<TPolicies, T, TName, TIsRoot, TPolicies, TDependency, TCtor...>(
            policies, dependency), 0)...}; (void)_;

       return std::is_same<
            aux::bool_list<aux::always<TPolicies>{}...>
          , aux::bool_list<decltype(call_impl<TPolicies, T, TName, TIsRoot, TPolicies, TDependency, TCtor...>(policies, dependency)){}...>
        >{};
    }

private:
    template<
        class TPolicy
      , class T
      , class TName
      , class TIsRoot
      , class TPolicies
      , class TDependency
      , class... TCtor
    > static auto call_impl(const TPolicies& policies, TDependency dependency) noexcept {
        return call_impl_type<arg_wrapper<T, TName, TIsRoot, TDeps>, TDependency, TPolicy, TCtor...>(
            static_cast<const TPolicy&>(policies), dependency
        );
    }

    template<class TArg, class TDependency, class TPolicy, class... TCtor>
    static auto call_impl_type(const TPolicy& policy, TDependency dependency) noexcept {
        call_impl_args<TArg, TDependency, TPolicy, TCtor...>(policy, dependency);
        using type = decltype(call_impl_args<TArg, TDependency, TPolicy, TCtor...>(policy, dependency));
        return std::conditional_t<std::is_same<type, void>{}, std::true_type, type>{};
    }

    template<class TArg, class TDependency, class TPolicy, class... TCtor
           , REQUIRES<!has_call_operator<TPolicy, TArg, TDependency, TCtor...>{}> = 0>
    static auto call_impl_args(const TPolicy& policy, TDependency) noexcept {
        return (policy)(TArg{});
    }

    template<class TArg, class TDependency, class TPolicy, class... TCtor
           , REQUIRES<has_call_operator<TPolicy, TArg, TDependency, TCtor...>{}> = 0>
    static auto call_impl_args(const TPolicy& policy, TDependency dependency) noexcept {
        return (policy)(TArg{}, dependency, aux::type<TCtor>{}...);
    }
};

}}} // boost::di::core

#endif

#ifndef BOOST_DI_CORE_PROVIDER_HPP
#define BOOST_DI_CORE_PROVIDER_HPP

namespace boost { namespace di { namespace core {

template<class...>
struct provider;

template<
    class TExpected
  , class TGiven
  , class TName
  , class TParent
  , class TInjector
  , class TInitialization
  , class... TArgs
> struct provider<
    TExpected
  , TGiven
  , TName
  , TParent
  , aux::pair<TInitialization, aux::type_list<TArgs...>>
  , TInjector
> {
    template<class TMemory, class... Ts>
    static constexpr auto is_creatable() {
        using provider_t = decltype(injector_.provider());
        return provider_t::template is_creatable<TGiven, TInitialization, TMemory, Ts...>();
    }

    template<class T>
    struct get_arg_ {
        using type = std::conditional_t<concepts::creatable_<TInjector, T>(), T, void>;
    };

    template<class... Ts>
    struct get_arg_<any_type<Ts...>> {
        using type = any_type<TParent, TInjector, std::true_type>;
    };

    template<class TName_, class T>
    struct get_arg_<type_traits::named<TName_, T>> {
        using type = std::conditional_t<concepts::creatable_<TInjector, T, TName_>(), T, void>;
    };

    template<class TMemory = type_traits::heap>
    auto get_(const TMemory& memory = {}) const -> std::enable_if_t<
        is_creatable<TMemory, typename get_arg_<TArgs>::type...>()
      , std::conditional_t<std::is_same<TMemory, type_traits::stack>{}, TGiven, TGiven*>
    >;

    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        return get_impl(memory, get_arg(aux::type<TArgs>{})...);
    }

    template<class TMemory, class... Ts, REQUIRES<is_creatable<TMemory, Ts...>()> = 0>
    auto get_impl(const TMemory& memory, Ts&&... args) const {
        return injector_.provider().template get<TExpected, TGiven>(
            TInitialization{}
          , memory
          , args...
        );
    };

    template<class TMemory, class... Ts, REQUIRES<!is_creatable<TMemory, Ts...>()> = 0>
    auto get_impl(const TMemory& memory, Ts&&... args) const {
        return concepts::creatable_error<TInitialization, TName, TGiven*, Ts...>();
    };

    template<class T>
    auto get_arg(const aux::type<T>&) const {
        return injector_.template create_impl<T>();
    }

    template<class... Ts>
    auto get_arg(const aux::type<any_type<Ts...>>&) const {
        return any_type<TParent, TInjector>{injector_};
    }

    template<class T, class TName_>
    auto get_arg(const aux::type<type_traits::named<TName_, T>>&) const {
        return injector_.template create_impl<T, TName_>();
    }

    const TInjector& injector_;
};

}}} // boost::di::core

#endif

#ifndef BOOST_DI_CORE_INJECTOR_HPP
#define BOOST_DI_CORE_INJECTOR_HPP

namespace boost { namespace di { namespace core {

template<class T, class = void>
struct get_deps {
    using type = typename T::deps;
};

template<class T>
struct get_deps<T, std::enable_if_t<has_configure<T>{}>> {
    using type = typename aux::function_traits<
        decltype(&T::configure)
    >::result_type::deps;
};

template<
    class T
  , class = typename is_injector<T>::type
  , class = typename is_dependency<T>::type
> struct add_type_list;

template<class T, class TAny>
struct add_type_list<T, std::true_type, TAny> {
    using type = typename get_deps<T>::type;
};

template<class T>
struct add_type_list<T, std::false_type, std::true_type> {
    using type = aux::type_list<T>;
};

template<class T>
struct add_type_list<T, std::false_type, std::false_type> {
    using type = aux::type_list<dependency<scopes::exposed<>, T>>;
};

template<class... Ts>
using bindings_t = aux::join_t<typename add_type_list<Ts>::type...>;

template<class T>
decltype(auto) arg(const T& arg, std::enable_if_t<!has_configure<T>{}>* = 0) noexcept {
    return arg;
}

template<class T>
decltype(auto) arg(const T& arg, std::enable_if_t<has_configure<T>{}>* = 0) noexcept {
    return arg.configure();
}

template<class T, class TWrapper, class = void>
struct wrapper {
    using element_type = T;

    inline operator T() const noexcept {
        return wrapper_;
    }

    inline operator T() noexcept {
        return wrapper_;
    }

    TWrapper wrapper_;
};

template<class T, class TWrapper>
struct wrapper<T, TWrapper, REQUIRES<!std::is_convertible<TWrapper, T>{}, void, void>> {
    using element_type = T;

    inline operator T() const noexcept {
        return typename type<TWrapper>::template is_not_convertible_to<T>{};
    }

    inline operator T() noexcept {
        return typename type<TWrapper>::template is_not_convertible_to<T>{};
    }

    TWrapper wrapper_;
};

BOOST_DI_HAS_METHOD(call, call);

template<template<class> class TConfig, class... TDeps>
class injector : public pool<bindings_t<TDeps...>>
               , public TConfig<injector<TConfig, TDeps...>>
               , _ {
    template<class...> friend struct provider;
    template<class, class, class> friend struct any_type;
    template<class> friend class scopes::exposed;

    using pool_t = pool<bindings_t<TDeps...>>;
    using is_root_t = std::true_type;
    using config = std::conditional_t<
        std::is_default_constructible<TConfig<injector>>{}
      , _
      , TConfig<injector>
    >;

public:
    using deps = bindings_t<TDeps...>;

    template<class... TArgs>
    explicit injector(const init&, const TArgs&... args) noexcept
        : pool_t{init{}, pool<aux::type_list<std::remove_reference_t<decltype(arg(args))>...>>{arg(args)...}}
        , config{*this}
    { }

    template<template<class> class TConfig_, class... TDeps_>
    explicit injector(const injector<TConfig_, TDeps_...>& injector) noexcept
        : pool_t{init{}, create_from_injector(injector, deps{})}
        , config{*this}
    { }

    template<
        class T
      , class TName = no_name
      , class TIsRoot = std::false_type
      , class TDependency = std::remove_reference_t<decltype(binder::resolve<T, TName>((injector*)0))>
      , class TCtor = typename type_traits::ctor_traits<typename TDependency::given>::type
    > auto create_impl_() const -> std::enable_if_t<std::is_convertible<
       decltype(
           std::declval<TDependency>().template create_<T>(
               provider<
                   typename TDependency::expected
                 , typename TDependency::given
                 , TName
                 , T
                 , TCtor
                 , injector
               >{std::declval<injector>()}
           )
       ), T>{} && decltype(policy<pool_t>::template
           call<T, TName, TIsRoot>(((TConfig<injector>&)*this).policies(), std::declval<TDependency>(), TCtor{})){}
    >;

    template<class T, REQUIRES<concepts::creatable_<injector, T, no_name, is_root_t>()> = 0>
    T create() const {
        return create_impl<T, no_name, is_root_t>();
    }

    template<class T, REQUIRES<!concepts::creatable_<injector, T, no_name, is_root_t>()> = 0>
    BOOST_DI_ATTR_ERROR("creatable constraint not satisfied")
    T create() const {
        return create_impl<T, no_name, is_root_t>();
    }

    template<class TAction>
    void call(const TAction& action) {
        call_impl(action, deps{});
    }

private:
    template<class T, class TName = no_name, class TIsRoot = std::false_type>
    auto create_impl() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t>::type;
        using provider_t = provider<expected_t, given_t, TName, T, ctor_t, injector>;
        policy<pool_t>::template call<T, TName, TIsRoot>(((TConfig<injector>&)*this).policies(), dependency, ctor_t{});
        using wrapper_t = decltype(dependency.template create<T>(provider_t{*this}));
        using type = std::conditional_t<
            std::is_reference<T>{} && has_is_ref<dependency_t>{}
          , T
          , std::remove_reference_t<T>
        >;
        return wrapper<type, wrapper_t>{dependency.template create<T>(provider_t{*this})};
    }

    template<class TAction, class... Ts>
    void call_impl(const TAction& action, const aux::type_list<Ts...>&) {
        int _[]{0, (call_impl<Ts>(action), 0)...}; (void)_;
    }

    template<class T, class TAction>
    std::enable_if_t<has_call<T, const TAction&>{}>
    call_impl(const TAction& action) {
        static_cast<T&>(*this).call(action);
    }

    template<class T, class TAction>
    std::enable_if_t<!has_call<T, const TAction&>{}>
    call_impl(const TAction&) { }

    template<class TInjector, class... Ts>
    auto create_from_injector(const TInjector& injector
                            , const aux::type_list<Ts...>&) const noexcept {
        return pool_t{Ts{injector}...};
    }
};

}}} // boost::di::core

#endif

#ifndef BOOST_DI_CONCEPTS_BOUNDABLE_HPP
#define BOOST_DI_CONCEPTS_BOUNDABLE_HPP

namespace boost { namespace di {

template<class...>
struct bound_type {
    struct is_bound_more_than_once { };
    struct is_neither_a_dependency_nor_an_injector { };

    template<class>
    struct is_not_base_of { };

    template<class>
    struct is_not_convertible_to { };
};

namespace concepts {

template<class... TDeps>
using is_supported =
    std::is_same<
       aux::bool_list<aux::always<TDeps>{}...>
     , aux::bool_list<(core::is_injector<TDeps>{} || core::is_dependency<TDeps>{})...>
    >;

template<class...>
struct get_not_supported;

template<class T>
struct get_not_supported<T> {
    using type = T;
};

template<class T, class... TDeps>
struct get_not_supported<T, TDeps...>
    : std::conditional<
          core::is_injector<T>{} || core::is_dependency<T>{}
        , typename get_not_supported<TDeps...>::type
        , T
      >
{ };

template<class>
struct is_unique;

template<class T>
using unique_dependency = aux::pair<
    aux::pair<typename T::expected, typename T::name>
  , std::integral_constant<bool, T::scope::priority>
>;

template<class... TDeps>
struct is_unique<aux::type_list<TDeps...>>
    : aux::is_unique<unique_dependency<TDeps>...>
{ };

template<class... TDeps>
auto boundable_impl(aux::type_list<TDeps...>&&) ->
    std::integral_constant<bool,
        is_supported<TDeps...>{} && is_unique<core::bindings_t<TDeps...>>{}
    >;

template<class I, class T>
auto boundable_impl(I&&, T&&) ->
    std::integral_constant<bool,
        std::is_convertible<T, I>{} || std::is_base_of<I, T>{}
    >;

template<class T, class... Ts> // any_of
auto boundable_impl(aux::type_list<Ts...>&&, T&&) ->
    std::integral_constant<
        bool
      , !std::is_same<
            aux::bool_list<aux::never<Ts>{}...>
          , aux::bool_list<std::is_base_of<Ts, T>{}...>
        >{}
    >;

template<class... TDeps> // di::injector
auto boundable_impl(aux::type<TDeps...>&&) ->
    is_unique<core::bindings_t<TDeps...>>;

std::false_type boundable_impl(...);

template<class... TDeps>
constexpr auto boundable() {
    return decltype(boundable_impl(std::declval<TDeps>()...)){};
}

template<class>
struct get_is_unique_error_impl
    : std::true_type
{ };

template<class T, class TName, class TPriority>
struct get_is_unique_error_impl<aux::not_unique<aux::pair<aux::pair<T, TName>, TPriority>>> {
    using type = typename bound_type<T, TName>::is_bound_more_than_once;
};

template<class>
struct get_is_unique_error;

template<class... TDeps>
struct get_is_unique_error<aux::type_list<TDeps...>>
    : get_is_unique_error_impl<typename aux::is_unique<unique_dependency<TDeps>...>::type>
{ };

template<class... TDeps>
using get_error =
    std::conditional_t<
        is_supported<TDeps...>{}
      , typename get_is_unique_error<core::bindings_t<TDeps...>>::type
      , typename bound_type<typename get_not_supported<TDeps...>::type>::
            is_neither_a_dependency_nor_an_injector
    >;

template<class... TDeps>
auto boundable_error_impl(aux::type_list<TDeps...>&&) -> get_error<TDeps...>;

template<class I, class T>
auto boundable_error_impl(I&&, T&&) ->
    std::conditional_t<
        std::is_base_of<I, T>{}
      , typename bound_type<T>::template is_not_base_of<I>
      , typename bound_type<T>::template is_not_convertible_to<I>
    >;

template<class T, class... Ts> // any_of
auto boundable_error_impl(aux::type_list<Ts...>&&, T&&) -> int;

template<class... TDeps> // di::injector
auto boundable_error_impl(aux::type<TDeps...>&&) ->
    get_is_unique_error_impl<typename aux::is_unique<unique_dependency<TDeps>...>::type>;

std::false_type boundable_error_impl(...);

template<class... TDeps>
constexpr auto boundable_error() {
    return decltype(boundable_error_impl(std::declval<TDeps>()...)){};
}

}}} // boost::di::concepts

#endif

#ifndef BOOST_DI_BINDINGS_HPP
#define BOOST_DI_BINDINGS_HPP

namespace boost { namespace di {

template<class... Ts>
using any_of = aux::type_list<Ts...>;

template<
    class TExpected
  , class TGiven = TExpected
  , REQUIRES<
        concepts::boundable<TExpected, TGiven>()
      , decltype(concepts::boundable_error<TExpected, TGiven>())
    > = 0
> core::bind<TExpected, TGiven> bind{};

constexpr scopes::deduce deduce{};
constexpr scopes::unique unique{};
constexpr scopes::shared shared{};
constexpr scopes::singleton singleton{};

template<class TName>
constexpr auto session(const TName&) noexcept {
    return scopes::session<TName>{};
}

template<class TName>
constexpr auto session_entry(const TName&) noexcept {
    return scopes::session_entry<TName>{};
}

template<class TName>
constexpr auto session_exit(const TName&) noexcept {
    return scopes::session_exit<TName>{};
}

}} // boost::di

#endif

#ifndef BOOST_DI_INJECTOR_HPP
#define BOOST_DI_INJECTOR_HPP

namespace boost { namespace di {

template<class... TDeps>
class injector
    : public REQUIRES<
          concepts::boundable<aux::type<TDeps...>>()
        , decltype(concepts::boundable_error<aux::type<TDeps...>>())
        , core::injector<::BOOST_DI_CFG, TDeps...>> {
public:
    template<template<class> class TConfig, class... TArgs>
    injector(const core::injector<TConfig, TArgs...>& injector) noexcept // non explicit
        : core::injector<::BOOST_DI_CFG, TDeps...>{injector}
    { }
};

}} // boost::di

#endif

#ifndef BOOST_DI_CONCEPTS_CONFIGURABLE_HPP
#define BOOST_DI_CONCEPTS_CONFIGURABLE_HPP

namespace boost { namespace di {

template<class>
struct config_type {
    struct is_not_configurable { };
};

namespace concepts {

std::false_type configurable_impl(...);
std::false_type configurable_error_impl(...);

template<class T>
auto configurable_impl(T&& t) -> aux::is_valid_expr<
    decltype(t.provider())
  , decltype(t.policies())
>;

template<class T>
auto configurable_error_impl(T&&) -> std::conditional_t<
    decltype(configurable_impl(std::declval<T>())){}
  , std::false_type
  , typename config_type<T>::is_not_configurable
>;

template<class T>
constexpr auto configurable() {
    return decltype(configurable_impl(std::declval<T>())){};
}

template<class T>
constexpr auto configurable_error() {
    return decltype(configurable_error_impl(std::declval<T>())){};
}

}}} // boost::di::concepts

#endif

#ifndef BOOST_DI_MAKE_INJECTOR_HPP
#define BOOST_DI_MAKE_INJECTOR_HPP

namespace boost { namespace di {

template<
     template<class> class TConfig = ::BOOST_DI_CFG
   , class... TDeps
   , REQUIRES<concepts::configurable<TConfig<_>>() &&
              concepts::boundable<aux::type_list<TDeps...>>()
            , errors<decltype(concepts::boundable_error<aux::type_list<TDeps...>>())
                   , decltype(concepts::configurable_error<TConfig<_>>())
              >
     > = 0
> inline auto make_injector(const TDeps&... args) noexcept {
    return core::injector<TConfig, TDeps...>{core::init{}, args...};
}

}} // boost::di

#endif

#endif

#endif

