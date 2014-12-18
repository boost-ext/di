//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_HPP
#define BOOST_DI_HPP

#if defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS)

// config
#include "boost/di/config.hpp"

// annotations
#include "boost/di/named.hpp"

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
#include <new>
#include <type_traits>

#define BOOST_DI_AUX_UTILITY_HPP

#define BOOST_DI_CAT_IMPL(a, b) a ## b
#define BOOST_DI_CAT(a, b) BOOST_DI_CAT_IMPL(a, b)
#define BOOST_DI_CALL(m, ...) m(__VA_ARGS__)

namespace boost { namespace di { namespace aux {

struct none_t { };

template<class T, T>
struct non_type { };

template<class, class>
struct pair { using type = pair; };

template<class T>
struct no_decay { using type = T; };

template<class... TArgs>
struct inherit : TArgs... { };

template<class...>
struct type_list { using type = type_list; };

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

template<class TDefault, class>
static no_decay<TDefault> lookup(...);

template<class, class TKey, class TValue>
static no_decay<TValue> lookup(pair<TKey, TValue>*);

template<class TDefault, class TKey, class... Ts>
using at_key = decltype(lookup<TDefault, TKey>((inherit<Ts...>*)0));

template<class TDefault, class TKey, class... Ts>
using at_key_t = typename at_key<TDefault, TKey, Ts...>::type;

}}} // boost::di::aux

#define BOOST_DI_NAMED_HPP

namespace boost { namespace di {

namespace detail {

template<class T, class TName, class = void>
class named_impl : public T {
public:
    using named_type = T;
    using name = TName;

    using T::T;

    named_impl(const T& object = {}) noexcept // non explicit
        : T(object)
    { }

    operator const T&() const noexcept {
        return *this;;
    }
};

template<class T, class TName>
class named_impl<T, TName, std::enable_if_t<!std::is_class<T>{}>> {
public:
    using named_type = T;
    using name = TName;

    named_impl(const T& object = {}) noexcept // non explicit
        : object_(object)
    { }

    operator T&() noexcept {
        return object_;;
    }

    operator const T&() const noexcept {
        return object_;;
    }

private:
    T object_;
};

} // namespace detail

struct no_name { };

template<class T, class TName = no_name>
class named : public detail::named_impl<T, TName> {
    using detail::named_impl<T, TName>::named_impl;
};

template<class T, class TName>
struct named_traits {
    using type = named<T, TName>;
};

template<class T, class TName>
struct named_traits<const T&, TName> {
	using type = const named<T, TName>&;
};

template<class T, class TName>
struct named_traits<T&, TName> {
	using type = named<T, TName>&;
};

template<class TName, class T>
using named_ = typename named_traits<T, TName>::type;

}} // boost::di

#define BOOST_DI_AUX_TYPE_TRAITS_HPP

#define BOOST_DI_HAS_TYPE(name)                                     \
    template<class>                                                 \
    std::false_type has_##name##_impl(...);                         \
                                                                    \
    template<class T>                                               \
    std::true_type has_##name##_impl(typename T::name*);            \
                                                                    \
    template<class T>                                               \
    using has_##name = decltype(has_##name##_impl<T>(0))

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

#define BOOST_DI_HAS_METHOD_CALL(name, call_name)                   \
    struct has_##name##_base {                                      \
        void call_name();                                           \
    };                                                              \
                                                                    \
    template<class T>                                               \
    std::false_type has_##name##_impl(                              \
        T*                                                          \
      , boost::di::aux::non_type<                                   \
            void (has_##name##_base::*)()                           \
          , &T::call_name                                           \
        >* = 0                                                      \
    );                                                              \
                                                                    \
    template<class>                                                 \
    std::true_type has_##name##_impl(...);                          \
                                                                    \
    template<class T, typename = void>                              \
    struct has_##name                                               \
        : decltype(has_##name##_impl<                               \
              boost::di::aux::inherit<T, has_##name##_base>         \
          >(0))                                                     \
    { };                                                            \
                                                                    \
    template<class T>                                               \
    struct has_##name<T, std::enable_if_t<!std::is_class<T>{}>>     \
        : std::false_type                                           \
    { }

namespace boost { namespace di { namespace aux {

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

template<class T, class TName>
struct deref_type<named<T, TName>> {
    using type = T;
};

BOOST_DI_HAS_TYPE(element_type);
BOOST_DI_HAS_TYPE(named_type);

template<class T>
struct deref_type<T, std::enable_if_t<has_element_type<T>{} && !has_named_type<T>{}>> {
    using type = typename T::element_type;
};

template<class T>
using make_plain =
    deref_type<remove_accessors_t<deref_type_t<remove_accessors_t<T>>>>;

template<class T>
using make_plain_t =
    typename make_plain<T>::type;

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

}}} // boost::di::aux

#define BOOST_DI_CORE_POOL_HPP

namespace boost { namespace di { namespace core {

struct init { };

template<class = aux::type_list<>>
class pool;

template<class... TArgs>
class pool<aux::type_list<TArgs...>> : public TArgs... {
public:
    template<class... Ts>
    explicit pool(const Ts&... args) noexcept
        : Ts(args)...
    { }

    template<class TPool>
    pool(const init&, const TPool& p) noexcept
        : pool(get<TArgs>(p)...)
    { }

    template<class T>
    inline const T& get() const noexcept {
        return static_cast<const T&>(*this);
    }

private:
    template<class T, class TPool>
    std::enable_if_t<std::is_base_of<T, pool>{} && std::is_base_of<T, TPool>{}, const T&>
    inline get(const TPool& p) const noexcept { return p.template get<T>(); }

    template<class T, class TPool>
    std::enable_if_t<std::is_base_of<T, pool>{} && !std::is_base_of<T, TPool>{}, T>
    inline get(const TPool&) const noexcept { return {}; }

    template<class T, class TPool>
    std::enable_if_t<!std::is_base_of<T, pool>{}, T>
    inline get(const TPool&) const noexcept { return {}; }
};

}}} // boost::di::core

#define BOOST_DI_WRAPPERS_UNIQUE_HPP

#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace wrappers {

template<class T>
class unique {
public:
    unique(const T& value) noexcept // non explicit
        : value_(value)
    { }

    template<class I>
    inline operator I() const noexcept {
        return value_;
    }

    template<class I>
    inline operator I*() const noexcept { // only for compilation clean
        return {};
    }

private:
    T value_;
};

template<class T>
class unique<T*> {
public:
    unique(T* value) noexcept // non explicit
        : value_(value)
    { }

    template<class I, class = std::enable_if_t<!std::is_polymorphic<I>{}>>
    inline operator I() const noexcept {
        return *std::unique_ptr<I>(value_);
    }

    template<class I>
    inline operator I*() const noexcept {
        return value_; // ownership transfer
    }

    template<class I>
    inline operator const I*() const noexcept {
        return value_; // ownership transfer
    }

    template<class I>
    inline operator std::shared_ptr<I>() const noexcept {
        return std::shared_ptr<I>{value_};
    }

#if (__has_include(<boost/shared_ptr.hpp>))
    template<class I>
    inline operator boost::shared_ptr<I>() const noexcept {
        return boost::shared_ptr<I>{value_};
    }
#endif

    template<class I>
    inline operator std::unique_ptr<I>() const noexcept {
        return std::unique_ptr<I>{value_};
    }

private:
    T* value_ = nullptr;
};

}}} // boost::di::wrappers

#define BOOST_DI_WRAPPERS_SHARED_HPP

#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace wrappers {

template<class T>
class shared {
public:
    shared() noexcept { }

    shared(const std::shared_ptr<T>& value) noexcept // non explicit
        : value_(value)
    { }

    inline bool operator!() const noexcept {
        return !value_;
    }

    inline void reset(T* ptr = 0) noexcept {
        return value_.reset(ptr);
    }

    template<class I>
    inline operator std::shared_ptr<I>() const noexcept {
        return value_;
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
        using sp = sp_holder<boost::shared_ptr<I>>;
        if (auto* deleter = std::get_deleter<sp, I>(value_)) {
            return deleter->object;
        } else {
            return {value_.get(), sp_holder<std::shared_ptr<T>>{value_}};
        }
    }
#endif

    template<class I>
    inline operator std::weak_ptr<I>() const noexcept {
        return value_;
    }

private:
    std::shared_ptr<T> value_;
};

}}} // boost::di::wrappers

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
using wrapper_traits_t = typename wrapper_traits<T>::type;

class external {
    struct injector {
        template<class T> T create() const;
    };

public:
    static constexpr auto priority = true;

    template<class TExpected, class TGiven, class = void>
    class scope {
    public:
        explicit scope(const TGiven& object) noexcept
            : object_(object)
        { }

        template<class, class TProvider>
        decltype(auto) create(const TProvider&) const noexcept {
            return object_;
        }

    private:
        wrappers::unique<TExpected> object_;
    };

    template<class TExpected, class TGiven>
    class scope<TExpected, std::reference_wrapper<TGiven>> {
    public:
        using is_ref = void;

        explicit scope(const std::reference_wrapper<TGiven>& object) noexcept
            : object_(object)
        { }

        template<class, class TProvider>
        decltype(auto) create(const TProvider&) const noexcept {
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
        decltype(auto) create(const TProvider&) const noexcept {
            return object_;
        }

    private:
        wrappers::shared<TGiven> object_;
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
        decltype(auto) create(const TProvider&) const noexcept {
            using wrapper = wrapper_traits_t<decltype(std::declval<TGiven>()())>;
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
        decltype(auto) create(const TProvider& provider) const noexcept {
            using wrapper = wrapper_traits_t<decltype((object_)(provider.injector_))>;
            return wrapper{(object_)(provider.injector_)};
        }

    private:
        TGiven object_;
    };
};

}}} // boost::di::scopes


namespace boost { namespace di { namespace core {

template<class TExpected, class TName>
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
  , bool  TPriority = TScope::priority
>
class dependency
    : public TScope::template scope<TExpected, TGiven>
    , public dependency_impl<
          dependency_concept<TExpected, TName>
        , dependency<TScope, TExpected, TGiven, TName, TPriority>
      > {
    using scope_t = typename TScope::template scope<TExpected, TGiven>;

public:
    using type = dependency;
    using scope = TScope;
    using expected = TExpected;
    using given = TGiven;
    using name = TName;

    dependency() noexcept { }

    template<class T>
    explicit dependency(T&& object) noexcept
        : scope_t(std::forward<T>(object))
    { }

    template<class T, class TInjector>
    dependency(T&& object, const TInjector& injector) noexcept
        : scope_t(std::forward<T>(object), injector)
    { }

    template<class T>
    auto named(const T&) const noexcept {
        return dependency<TScope, TExpected, TGiven, T>{};
    }

    template<class T>
    auto in(const T&) const noexcept {
        return dependency<T, TExpected, TGiven, TName>{};
    }

    template<class T>
    auto to(T&& object) const noexcept {
        using dependency = dependency<
            scopes::external, TExpected, std::remove_reference_t<T>, TName
        >;
        return dependency{std::forward<T>(object)};
    }
};

template<class>
struct is_dependency : std::false_type { };

template<
    class TScope
  , class TExpected
  , class TGiven
  , class TName
  , bool  TPriority
>
struct is_dependency<
    dependency<TScope, TExpected, TGiven, TName, TPriority>
> : std::true_type { };

}}} // boost::di::core

#define BOOST_DI_TYPE_TRAITS_MEMORY_TRAITS_HPP

#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace type_traits {

struct heap { };
struct stack { };

template<class T>
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
struct memory_traits<std::unique_ptr<T>> {
    using type = heap;
};

template<class T>
struct memory_traits<const std::unique_ptr<T>&> {
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

template<class T, class TName>
struct memory_traits<named<T, TName>> {
    using type = typename memory_traits<T>::type;
};

template<class T, class TName>
struct memory_traits<const named<T, TName>&> {
    using type = typename memory_traits<T>::type;
};

template<class T>
using memory_traits_t = typename memory_traits<T>::type;

}}} // boost::di::type_traits

#define BOOST_DI_SCOPES_UNIQUE_HPP

namespace boost { namespace di { namespace scopes {

class unique {
public:
    static constexpr auto priority = false;

    template<class, class>
    class scope {
    public:
        template<class T, class TProvider>
        decltype(auto) create(const TProvider& provider) const noexcept {
            using memory = type_traits::memory_traits_t<T>;
            using wrapper = wrappers::unique<decltype(provider.get(memory{}))>;
            return wrapper{provider.get(memory{})};
        }
    };
};

}}} // boost::di::scopes

#define BOOST_DI_SCOPES_SINGLETON_HPP

namespace boost { namespace di { namespace scopes {

class singleton {
public:
    static constexpr auto priority = false;

    template<class, class T>
    class scope {
    public:
        template<class, class TProvider>
        decltype(auto) create(const TProvider& provider) const noexcept {
            if (!get_instance()) {
                get_instance().reset(provider.get());
            }
            return get_instance();
        }

    private:
        static wrappers::shared<T>& get_instance() noexcept {
            static wrappers::shared<T> object;
            return object;
        }
    };
};

}}} // boost::di::scopes

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
struct scope_traits<std::unique_ptr<T>> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<const std::unique_ptr<T>&> {
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

template<class T, class TName>
struct scope_traits<named<T, TName>> {
    using type = typename scope_traits<T>::type;
};

template<class T, class TName>
struct scope_traits<const named<T, TName>&> {
    using type = typename scope_traits<T>::type;
};

template<class T>
using scope_traits_t = typename scope_traits<T>::type;

}}} // boost::di::type_traits


namespace boost { namespace di { namespace scopes {

class deduce {
public:
    static constexpr auto priority = false;

    template<class TExpected, class TGiven>
    class scope {
    public:
        template<class T, class TProvider>
        decltype(auto) create(const TProvider& provider) const noexcept {
            using scope_traits = type_traits::scope_traits_t<T>;
            using scope = typename scope_traits::template scope<TExpected, TGiven>;
            return scope{}.template create<T>(provider);
        }
    };
};

}}} // boost::di::scopes


namespace boost { namespace di { namespace core {

class binder {
    template<class>
    struct get_name {
        using type = no_name;
    };

    template<class T, class TName>
    struct get_name<named<T, TName>> {
        using type = TName;
    };

    template<class T>
    using get_name_t = typename get_name<T>::type;

    template<class TDefault, class>
    static TDefault resolve_impl(...) noexcept {
        return {};
    }

    template<class, class TConcept, class TDependency>
    static TDependency&
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
               , dependency<TScope, TExpected, TGiven, TName, true>>* dep) noexcept {
        return static_cast<dependency<TScope, TExpected, TGiven, TName, true>&>(*dep);
    }

public:
    template<
        class T
      , class TDefault = dependency<scopes::deduce, aux::make_plain_t<T>>
      , class TDeps = void
    > static decltype(auto) resolve(TDeps* deps) noexcept {
        using dependency = dependency_concept<
            aux::make_plain_t<T>
          , get_name_t<aux::remove_accessors_t<T>>
        >;
        return resolve_impl<TDefault, dependency>(deps);
    }
};

}}} // boost::di::core


namespace boost { namespace di { namespace core {

BOOST_DI_HAS_TYPE(is_ref);

template<class TParent = aux::none_t, class TInjector = aux::none_t>
struct any_type {
    template<class T>
    struct is_not_same_impl {
        static constexpr auto value =
            std::is_same<aux::make_plain_t<T>, aux::make_plain_t<TParent>>::value ||
            std::is_base_of<aux::make_plain_t<TParent>, aux::make_plain_t<T>>::value;
    };

    template<class T>
    using is_not_same = std::enable_if_t<!is_not_same_impl<T>::value>;

    template<class T>
    struct is_ref_impl {
        static constexpr auto value =
            std::is_same<TInjector, aux::none_t>::value ||
            has_is_ref<std::remove_reference_t<decltype(binder::resolve<T>((TInjector*)nullptr))>>::value;
    };

    template<class T>
    using is_ref = std::enable_if_t<is_ref_impl<T>::value>;

    template<class T, class = is_not_same<T>>
    operator T() noexcept {
        return injector_.template create<T, TParent>();
    }

    template<class T, class = is_not_same<T>, class = is_ref<T>>
    operator T&() const noexcept {
        return injector_.template create<T&, TParent>();
    }

    template<class T, class = is_not_same<T>, class = is_ref<T>>
    operator const T&() const noexcept {
        return injector_.template create<const T&, TParent>();
    }

    const TInjector& injector_;
};

template<class>
struct is_any_type : std::false_type { };

template<class... TArgs>
struct is_any_type<any_type<TArgs...>> : std::true_type { };

}}} // boost::di::core


#define BOOST_DI_INJECT_HPP

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

#if !defined(BOOST_DI_INJECT_TRAITS)
    #define BOOST_DI_INJECT_TRAITS(...)             \
        static void BOOST_DI_INJECTOR(__VA_ARGS__)
#endif

#if !defined(BOOST_DI_INJECT)
    #define BOOST_DI_INJECT(type, ...)              \
        BOOST_DI_INJECT_TRAITS(__VA_ARGS__);        \
        type(__VA_ARGS__)
#endif

#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

namespace boost { namespace di { namespace type_traits {

struct direct { };
struct aggregate { };

BOOST_DI_CALL(BOOST_DI_HAS_METHOD_CALL, BOOST_DI_INJECTOR, BOOST_DI_INJECTOR);

template<class T, std::size_t>
struct get_type {
    using type = T;
};

template<template<class...> class, class, class, class>
struct ctor_args;

template<
    template<class...> class TIsConstructible
  , class T
  , class TAny
  , std::size_t... TArgs
> struct ctor_args<TIsConstructible, T, TAny, std::index_sequence<TArgs...>>
    : aux::pair<
          typename TIsConstructible<T, typename get_type<TAny, TArgs>::type...>::type
        , aux::type_list<typename get_type<TAny, TArgs>::type...>
      >
{ };

template<template<class...> class, class, class>
struct ctor_impl;

template<
    template<class...> class TIsConstructible
  , class T
  , std::size_t... TArgs
> struct ctor_impl<TIsConstructible, T, std::index_sequence<TArgs...>>
    : aux::at_key_t<
          aux::type_list<>
        , std::true_type
        , ctor_args<
              TIsConstructible
            , T
            , core::any_type<T>
            , std::make_index_sequence<TArgs>
          >...
      >
{ };

template<template<class...> class TIsConstructible, class T>
using ctor_impl_t =
    typename ctor_impl<
        TIsConstructible
      , T
      , std::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE + 1>
    >::type;

template<class...>
struct ctor;

template<class T>
struct ctor<T, aux::type_list<>>
    : aux::pair<aggregate, ctor_impl_t<aux::is_braces_constructible, T>>
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
    : aux::pair<
          direct
        , typename aux::function_traits<
              decltype(&T::BOOST_DI_INJECTOR)
          >::args
      >
{ };

template<class T>
struct ctor_traits<T, std::false_type>
    : ctor_traits_impl<T>
{ };

template<class T>
struct ctor_traits_impl<T, std::true_type>
    : aux::pair<
          direct
        , typename aux::function_traits<
              decltype(&di::ctor_traits<T>::BOOST_DI_INJECTOR)
          >::args
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

#define BOOST_DI_PROVIDERS_NOTHROW_REDUCE_HEAP_USAGE_HPP

namespace boost { namespace di { namespace providers {

class nothrow_reduce_heap_usage {
public:
    template<class T, class... TArgs>
    auto* get(const type_traits::direct&
            , const type_traits::heap&
            , TArgs&&... args) const noexcept {
        return new (std::nothrow) T(std::forward<TArgs>(args)...);
    }

    template<class T, class... TArgs>
    auto* get(const type_traits::aggregate&
            , const type_traits::heap&
            , TArgs&&... args) const noexcept {
        return new (std::nothrow) T{std::forward<TArgs>(args)...};
    }

    template<class T, class... TArgs>
    auto get(const type_traits::direct&
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T(std::forward<TArgs>(args)...);
    }

    template<class T, class... TArgs>
    auto get(const type_traits::aggregate
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T{std::forward<TArgs>(args)...};
    }
};

}}} // boost::di::providers

#define BOOST_DI_CONFIG_HPP

#if defined(BOOST_DI_CFG)
    class BOOST_DI_CFG;
#else
    #define BOOST_DI_CFG boost::di::config
#endif

namespace boost { namespace di {

template<class... TArgs>
inline auto make_policies(const TArgs&... args) noexcept {
    return core::pool<aux::type_list<TArgs...>>(args...);
}

class config {
public:
    auto provider() const noexcept {
        return providers::nothrow_reduce_heap_usage{};
    }

    auto policies() const noexcept {
        return make_policies();
    }
};

}} // boost::di

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
        decltype(auto) create(const TProvider& provider) noexcept {
            if (in_scope_ && !object_) {
                object_.reset(provider.get());
            }
            return object_;
        }

    private:
        wrappers::shared<T> object_;
        bool in_scope_ = false;
    };
};

}}} // boost::di::scopes

#define BOOST_DI_SCOPES_SHARED_HPP

namespace boost { namespace di { namespace scopes {

class shared {
public:
    static constexpr auto priority = false;

    template<class, class T>
    class scope {
    public:
        template<class, class TProvider>
        decltype(auto) create(const TProvider& provider) noexcept {
            if (!object_) {
                object_.reset(provider.get());
            }
            return object_;
        }

    private:
        wrappers::shared<T> object_;
    };
};

}}} // boost::di::scopes

#define BOOST_DI_BINDINGS_HPP

namespace boost { namespace di {

template<class... Ts>
using any_of = aux::type_list<Ts...>;

template<class TExpected, class TGiven = TExpected>
core::dependency<scopes::deduce, TExpected, TGiven> bind{};

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


namespace boost { namespace di { namespace core {

template<class...>
struct provider;

template<
    class T
  , class TParent
  , class TInjector
  , class TInitialization
  , class... TArgs
> struct provider<
    T
  , TParent
  , aux::pair<TInitialization, aux::type_list<TArgs...>>
  , TInjector
> {
    template<class TMemory = type_traits::heap>
    decltype(auto) get(const TMemory& memory = {}) const noexcept {
        auto&& config = injector_.config_;
        return config.provider().template get<T>(
            TInitialization{}
          , memory
          , injector_.template create<TArgs, TParent>()...
        );
    }

    const TInjector& injector_;
};

}}} // boost::di::core

#define BOOST_DI_WRAPPERS_UNIVERSAL_HPP

namespace boost { namespace di { namespace wrappers {

template<class T, class TWrapper>
struct universal {
    TWrapper wrapper_;

    inline operator T() const noexcept {
        return wrapper_;
    }
};

template<class TWrapper, class T, class TName>
struct universal<di::named<T, TName>, TWrapper> {
    TWrapper wrapper_;

    inline operator T() const noexcept {
        return wrapper_;
    }

    inline operator named<T, TName>() const noexcept {
       return static_cast<const T&>(wrapper_);
    }
};

template<class TWrapper, class T, class TName>
struct universal<const di::named<T, TName>&, TWrapper> {
    TWrapper wrapper_;

    inline operator const T&() const noexcept {
        return wrapper_;
    }

    inline operator const named<T, TName>&() const noexcept {
        return reinterpret_cast<const named<T, TName>&>(static_cast<const T&>(wrapper_));
    }
};

template<class TWrapper, class T, class TName>
struct universal<di::named<T, TName>&, TWrapper> {
    TWrapper wrapper_;

    inline operator T&() const noexcept {
        return wrapper_;
    }

    inline operator named<T, TName>&() const noexcept {
        return reinterpret_cast<named<T, TName>&>(static_cast<T&>(wrapper_));
    }
};

}}} // boost::di::wrappers

#define BOOST_DI_CORE_INJECTOR_HPP

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(configure, configure);
BOOST_DI_HAS_METHOD(call, call);

template<class TDeps, class TConfig>
class injector : public pool<TDeps> {
    template<class, class> friend struct any_type;
    template<class...> friend struct provider;

    using pool_t = pool<TDeps>;

    template<class T, class TDependency>
    struct data {
        using type = T;

        TDependency&& dep;
        pool_t& deps;

        template<class U, class TDefault>
        decltype(auto) resolve() const noexcept {
            return binder::template resolve<U, TDefault>(&deps);
        }
    };

public:
    using deps = TDeps;

    // bind<...>, etc.  -> ignore
    // injector<...>    -> get all dependencies from the injector
    // dependency<...>  -> pass

    template<class... TArgs>
    explicit injector(const TArgs&... args) noexcept
        : injector(init{}, pass_arg(args)...)
    { }

    template<class... TArgs>
    injector(const injector<TArgs...>& injector) noexcept // non explicit
        : pool_t{init{}, create_from_injector(injector, TDeps{})}
    { }

    template<class T>
    T create() const noexcept {
        return create<T, aux::none_t>();
    }

    template<class TAction>
    void call(const TAction& action) noexcept {
        call_impl(action, deps{});
    }

private:
    template<class... TArgs>
    injector(const init&, const TArgs&... args) noexcept
        : pool_t{init{}, pool<aux::type_list<TArgs...>>{args...}}
    { }

    template<class T, class TParent>
    decltype(auto) create(std::enable_if_t<is_any_type<T>{}>* = 0) const noexcept {
        return any_type<TParent, injector>{*this};
    }

    template<class T, class TParent>
    decltype(auto) create(std::enable_if_t<!is_any_type<T>{}>* = 0) const noexcept {
        return create_impl<T>();
    }

    template<class T>
    decltype(auto) create_impl() const noexcept {
        auto&& dependency = binder::resolve<T>((injector*)this);
        using dependency_t = typename std::remove_reference_t<decltype(dependency)>;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t>::type;
        using provider_type = provider<given_t, T, ctor_t, injector>;
        const auto& ctor_provider = provider_type{*this};
        using wrapper_t = decltype(dependency.template create<T>(ctor_provider));
        using type = std::conditional_t<
            std::is_reference<T>{} && has_is_ref<dependency_t>{}
          , T
          , std::remove_reference_t<T>
        >;
        call_policies<T>(config_.policies(), dependency);
        return wrappers::universal<type, wrapper_t>{
            dependency.template create<T>(ctor_provider)
        };
    }

    template<
        class T
      , class TDependency
      , class... TPolicies
    > void call_policies(const pool<aux::type_list<TPolicies...>>& policies
                       , TDependency&& dependency) const noexcept {
        void(call_policies_impl<TPolicies, T>(
            policies, std::forward<TDependency>(dependency))...
        );
    }

    template<
        class TPolicy
      , class T
      , class TPolicies
      , class TDependency
    > void call_policies_impl(const TPolicies& policies
                            , TDependency&& dependency) const noexcept {
        auto&& injections = data<T, TDependency>{
            std::forward<TDependency>(dependency)
          , (injector&)*this
        };
        (static_cast<const TPolicy&>(policies))(injections);
    }

    template<class TAction, class... Ts>
    void call_impl(const TAction& action, const aux::type_list<Ts...>&) noexcept {
        void(call_impl<Ts>(action)...);
    }

    template<class T, class TAction>
    std::enable_if_t<has_call<T, const TAction&>{}>
    call_impl(const TAction& action) noexcept {
        static_cast<T&>(*this).call(action);
    }

    template<class T, class TAction>
    std::enable_if_t<!has_call<T, const TAction&>{}>
    call_impl(const TAction&) noexcept { }

    template<class T>
    decltype(auto) pass_arg(const T& arg
                          , std::enable_if_t<!has_configure<T>{}>* = 0) const noexcept {
        return arg;
    }

    template<class T>
    decltype(auto) pass_arg(const T& arg
                          , std::enable_if_t<has_configure<T>{}>* = 0) const noexcept {
        return arg.configure();
    }

    template<class... TArgs, class... Ts>
    decltype(auto)
    create_from_injector(const injector<TArgs...>& injector
                       , const aux::type_list<Ts...>&) const noexcept {
        return pool<TDeps>(create_dep<Ts>(injector)...);
    }

    template<class TDependency, class TInjector>
    decltype(auto) create_dep(const TInjector& injector) const noexcept {
        return TDependency{injector};
    }

    TConfig config_;
};

}}} // boost::di::core


namespace boost { namespace di { namespace scopes {

class exposed {
public:
    static constexpr auto priority = false;

    template<class TExpected, class TGiven>
    class scope {
        struct iprovider {
            virtual ~iprovider() = default;
            virtual TGiven* get(const type_traits::heap& = {}) const noexcept = 0;
            virtual TGiven  get(const type_traits::stack&) const noexcept = 0;
        };

        template<typename TInjector>
        class provider : public iprovider {
        public:
            explicit provider(const TInjector& injector)
                : injector_(injector)
            { }

            TGiven* get(const type_traits::heap&) const noexcept override {
                return injector_.template create<TGiven*>();
            }

            TGiven get(const type_traits::stack&) const noexcept override {
                return injector_.template create<TGiven>();
            }

        private:
            TInjector injector_;
        };

    public:
        template<class TInjector>
        explicit scope(const TInjector& injector) noexcept
            : provider_{std::make_shared<provider<TInjector>>(injector)}
        { }

        template<class T, class TProvider>
        decltype(auto) create(const TProvider&) const noexcept {
            using scope_traits = type_traits::scope_traits_t<T>;
            using scope = typename scope_traits::template scope<TExpected, TGiven>;
            return scope{}.template create<T>(*provider_);
        }

    private:
        std::shared_ptr<iprovider> provider_;
    };
};

}}} // boost::di::scopes

#define BOOST_DI_INJECTOR_HPP

namespace boost { namespace di { namespace detail {

BOOST_DI_HAS_TYPE(deps);

template<class T>
using is_injector =
    std::integral_constant<bool, has_deps<T>{} || core::has_configure<T>{}>;

template<class T, class = void>
struct get_deps {
    using type = typename T::deps;
};

template<class T>
struct get_deps<T, std::enable_if_t<core::has_configure<T>{}>> {
    using type = typename aux::function_traits<
        decltype(&T::configure)
    >::result_type::deps;
};

template<
    class T
  , class = typename is_injector<T>::type
  , class = typename core::is_dependency<T>::type
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
    using type = aux::type_list<core::dependency<scopes::exposed, T>>;
};

template<class... Ts>
using bindings_t = aux::join_t<typename add_type_list<Ts>::type...>;

template<class TConfig, class... TArgs>
using injector = core::injector<detail::bindings_t<TArgs...>, TConfig>;

} // detail

template<class... TArgs>
class injector : public core::injector<detail::bindings_t<TArgs...>, config> {
    using core::injector<detail::bindings_t<TArgs...>, config>::injector;
};

}} // boost::di

#define BOOST_DI_MAKE_INJECTOR_HPP

namespace boost { namespace di {

template<class TConfig = ::BOOST_DI_CFG, class... TArgs>
inline auto make_injector(const TArgs&... args) noexcept {
    return detail::injector<TConfig, TArgs...>(args...);
}

}} // boost::di

#endif

#endif

