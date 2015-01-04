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

#define BOOST_DI_AUX_UTILITY_HPP

namespace boost { namespace di { namespace aux {

struct none_t { };

template<class...>
struct type { };

template<class T, T>
struct non_type { };

template<class...>
using void_t = void;

template<class, class>
struct pair { using type = pair; };

template<class...>
struct type_list { using type = type_list; };

template<class... TArgs>
struct inherit : TArgs... { };

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

}}} // boost::di::aux

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
    { };

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

namespace boost { namespace di { namespace aux {

template<class>
struct is_smart_ptr : std::false_type { };

template<class T>
struct is_smart_ptr<std::unique_ptr<T>>
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
    explicit unique(const T& value) noexcept // non explicit
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
    explicit unique(T* value) noexcept // non explicit
        : value_(value)
    { }

    template<class I>
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

#define BOOST_DI_TYPE_TRAITS_MEMORY_TRAITS_HPP

#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace type_traits {

struct heap { };
struct stack { };

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

template<class T>
struct memory_traits<std::unique_ptr<T>> {
    using type = heap;
};

template<class T>
struct memory_traits<const std::unique_ptr<T>&> {
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

#define BOOST_DI_SCOPES_UNIQUE_HPP

namespace boost { namespace di { namespace scopes {

class unique {
public:
    static constexpr auto priority = false;

    template<class, class>
    class scope {
    public:
        template<class T, class TProvider>
        auto create(const TProvider& provider) const {
            using memory = type_traits::memory_traits_t<T>;
            using wrapper = wrappers::unique<decltype(provider.get(memory{}))>;
            return wrapper{provider.get(memory{})};
        }
    };
};

}}} // boost::di::scopes

#define BOOST_DI_WRAPPERS_SHARED_HPP

#if (__has_include(<boost/shared_ptr.hpp>))
    #include <boost/shared_ptr.hpp>
#endif

namespace boost { namespace di { namespace wrappers {

template<class T>
class shared {
public:
    explicit shared(const std::shared_ptr<T>& value) noexcept
        : value_(value)
    { }

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

#define BOOST_DI_SCOPES_SINGLETON_HPP

namespace boost { namespace di { namespace scopes {

class singleton {
public:
    static constexpr auto priority = false;

    template<class, class T>
    class scope {
    public:
        template<class, class TProvider>
        auto create(const TProvider& provider) {
            if (!get_instance()) {
                get_instance().reset(provider.get());
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
        auto create(const TProvider&) const noexcept {
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
        auto create(const TProvider&) const noexcept {
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
        auto create(const TProvider& provider) const noexcept {
            using wrapper = wrapper_traits_t<decltype((object_)(provider.injector_))>;
            return wrapper{(object_)(provider.injector_)};
        }

    private:
        TGiven object_;
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
struct scope_traits<T&&> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<const T&&> {
    using type = scopes::unique;
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


namespace boost { namespace di { namespace scopes {

class deduce {
public:
    static constexpr auto priority = false;

    template<class TExpected, class TGiven>
    class scope {
    public:
        template<class T, class TProvider>
        auto create(const TProvider& provider) {
            using scope_traits = type_traits::scope_traits_t<T>;
            using scope = typename scope_traits::template scope<TExpected, TGiven>;
            return scope{}.template create<T>(provider);
        }
    };
};

}}} // boost::di::scopes


namespace boost { namespace di { namespace core {

template<class>
class requires_unique_bindings;

template<class... Ts>
class requires_unique_bindings<aux::type_list<Ts...>> {
    template<class T>
    struct expected {
        using type = aux::pair<
            aux::pair<typename T::expected, typename T::name>
          , std::integral_constant<bool, T::scope::priority>
        >;
    };

    pool<aux::type_list<typename expected<Ts>::type...>> bindings;
};

template<class TExpected, class TGiven, class TScope>
class requires_external_concepts {
    static_assert(std::is_same<TExpected, TGiven>{}, "Explicit implementation type doesn't have sens for external scope");
    static_assert(std::is_same<TScope, scopes::deduce>{}, "External scope can not be bound explicitly in a different scope");
};

}}} // boost::di::core


namespace boost { namespace di { namespace scopes {

template<class TScope = scopes::deduce>
class exposed {
public:
    static constexpr auto priority = false;

    template<class TExpected, class TGiven>
    class scope {
        struct iprovider {
            virtual ~iprovider() = default;
            virtual TExpected* get(const type_traits::heap& = {}) const noexcept = 0;
            virtual TExpected  get(const type_traits::stack&) const noexcept = 0;
        };

        template<typename TInjector>
        class provider : public iprovider {
        public:
            explicit provider(const TInjector& injector)
                : injector_(injector)
            { }

            TExpected* get(const type_traits::heap&) const noexcept override {
                return injector_.template create<TExpected*>();
            }

            TExpected get(const type_traits::stack&) const noexcept override {
                return injector_.template create<TExpected>();
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
        auto create(const TProvider&) {
            return scope_.template create<T>(*provider_);
        }

    private:
        std::shared_ptr<iprovider> provider_;
        typename TScope::template scope<TExpected, TGiven> scope_;
    };
};

}}} // boost::di::scopes

#define BOOST_DI_FWD_HPP

namespace boost { namespace di {
namespace providers {
class heap;
class stack_over_heap;
} // providers

struct no_name {
    const char* operator()() const noexcept { return "no_name"; }
};

class config;
template<class...> class injector;

}} // boost::di


namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(configure, configure);
BOOST_DI_HAS_TYPE(deps);

template<class T>
using is_injector =
    std::integral_constant<bool, has_deps<T>{} || has_configure<T>{}>;

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
    auto to(T&& object, std::enable_if_t<!is_injector<std::remove_reference_t<T>>{}>* = 0) const noexcept {
        void(requires_external_concepts<TExpected, TGiven, TScope>{});
        using dependency = dependency<
            scopes::external, TExpected, std::remove_reference_t<T>, TName
        >;
        return dependency{std::forward<T>(object)};
    }

    template<class T>
    auto to(const T& object, std::enable_if_t<has_configure<T>{}>* = 0) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>, TExpected, decltype(std::declval<T>().configure()), TName
        >;
        return dependency{object.configure()};
    }

    template<class T>
    auto to(const T& object, std::enable_if_t<has_deps<T>{}>* = 0) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>, TExpected, T, TName
        >;
        return dependency{object};
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


namespace boost { namespace di { namespace core {

class binder {
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
      , class TName = no_name
      , class TDefault = dependency<scopes::deduce, aux::decay_t<T>>
      , class TDeps = void
    > static decltype(auto) resolve(TDeps* deps) noexcept {
        using dependency = dependency_concept<aux::decay_t<T>, TName>;
        return resolve_impl<TDefault, dependency>(deps);
    }
};

}}} // boost::di::core


namespace boost { namespace di { namespace core {

BOOST_DI_HAS_TYPE(is_ref);

template<class TParent = void, class TInjector = aux::none_t>
struct any_type {
    template<class T>
    struct is_not_same_impl {
        static constexpr auto value =
            std::is_same<aux::decay_t<T>, aux::decay_t<TParent>>::value ||
            std::is_base_of<aux::decay_t<TParent>, aux::decay_t<T>>::value;
    };

    template<class T>
    using is_not_same = std::enable_if_t<!is_not_same_impl<T>::value>;

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

    template<class T, class = is_not_same<T>>
    operator T() {
        return injector_.template create_impl<T>();
    }

    template<class T, class = is_not_same<T>, class = is_ref<T>>
    operator T&() const {
        return injector_.template create_impl<T&>();
    }

    template<class T, class = is_not_same<T>, class = is_ref<T>>
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

#define BOOST_DI_AUX_PREPROCESSOR_HPP

// mostly from the web!

#define PP_ARG_N( \
          _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
         _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
         _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
         _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
         _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
         _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
         _61, _62, _63, N, ...) N

#define PP_RSEQ_N()                                        \
         63, 62, 61, 60,                                   \
         59, 58, 57, 56, 55, 54, 53, 52, 51, 50,           \
         49, 48, 47, 46, 45, 44, 43, 42, 41, 40,           \
         39, 38, 37, 36, 35, 34, 33, 32, 31, 30,           \
         29, 28, 27, 26, 25, 24, 23, 22, 21, 20,           \
         19, 18, 17, 16, 15, 14, 13, 12, 11, 10,           \
          9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define PP_NARG_(...)    PP_ARG_N(__VA_ARGS__)
#define PP_NARG(...)     PP_NARG_(__VA_ARGS__, PP_RSEQ_N())

#define HAS_COMMA(...)                                    \
        PP_ARG_N(__VA_ARGS__, \
          1,  1,  1,  1,                                   \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  1,  1,           \
          1,  1,  1,  1,  1,  1,  1,  1,  0,  0)

#define DEC(x) BOOST_DI_PRIMITIVE_CAT(DEC_, x)
#define DEC_0 0
#define DEC_1 0
#define DEC_2 1
#define DEC_3 2
#define DEC_4 3
#define DEC_5 4
#define DEC_6 5
#define DEC_7 6
#define DEC_8 7
#define DEC_9 8
#define DEC_10 9

#define ARG0(p1, ...) p1
#define ARG1(p1, p2, ...) p2
#define ARG2(p1, p2, p3, ...) p3
#define ARG3(p1, p2, p3, p4, ...) p4
#define ARG4(p1, p2, p3, p4, p5, ...) p5
#define ARG5(p1, p2, p3, p4, p5, p6, ...) p6
#define ARG6(p1, p2, p3, p4, p5, p6, p7, ...) p7
#define ARG7(p1, p2, p3, p4, p5, p6, p7, p8, ...) p8
#define ARG8(p1, p2, p3, p4, p5, p6, p7, p8, p9, ...) p9
#define ARG9(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, ...) p10

#define BOOST_DI_COMMA() ,
#define BOOST_DI_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define BOOST_DI_CAT(a, ...) BOOST_DI_PRIMITIVE_CAT(a, __VA_ARGS__)
#define BOOST_DI_CALL(m, ...) m(__VA_ARGS__)
#define BOOST_DI_IBP_SPLIT(i, ...) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_IBP_SPLIT_, i)(__VA_ARGS__)
#define BOOST_DI_IBP_SPLIT_0(a, ...) a
#define BOOST_DI_IBP_SPLIT_1(a, ...) __VA_ARGS__
#define BOOST_DI_IBP_IS_VARIADIC_C(...) 1
#define BOOST_DI_IBP_IS_VARIADIC_R_1 1,
#define BOOST_DI_IBP_IS_VARIADIC_R_BOOST_DI_IBP_IS_VARIADIC_C 0,
#define BOOST_DI_IBP(...) BOOST_DI_IBP_SPLIT(0, BOOST_DI_CAT(BOOST_DI_IBP_IS_VARIADIC_R_, BOOST_DI_IBP_IS_VARIADIC_C __VA_ARGS__))
#define EMPTY()
#define _TRIGGER_PARENTHESIS_(...) ,
#define ISEMPTY(...) _ISEMPTY(HAS_COMMA(__VA_ARGS__), HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__), HAS_COMMA(__VA_ARGS__ ()), HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__ ()))
#define DEFER(id) id EMPTY()
#define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
#define EVAL(...)  EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) __VA_ARGS__
#define CHECK_N(x, n, ...) n
#define CHECK(...) CHECK_N(__VA_ARGS__, 0,)
#define NOT(x) CHECK(BOOST_DI_PRIMITIVE_CAT(NOT_, x))
#define NOT_0 ~, 1,
#define COMPL(b) BOOST_DI_PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0
#define BOOL(x) COMPL(NOT(x))
#define IIF(c) BOOST_DI_PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t
#define BOOST_DI_IF(c) IIF(BOOL(c))
#define PASTE5(_0, _1, _2, _3, _4) _0 ## _1 ## _2 ## _3 ## _4
#define _ISEMPTY(_0, _1, _2, _3) HAS_COMMA(PASTE5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,
#define EAT(...)
#define EXPAND(...) __VA_ARGS__
#define WHEN(c) BOOST_DI_IF(c)(EXPAND, EAT)
#define REPEAT(count, macro, ...) WHEN(count)(OBSTRUCT(REPEAT_INDIRECT)()(DEC(count), macro, __VA_ARGS__) OBSTRUCT(macro) (DEC(count), __VA_ARGS__ ))
#define REPEAT_INDIRECT() REPEAT
#define BOOST_DI_VARARG_IMPL2(m, count, ...) m##count(__VA_ARGS__)
#define BOOST_DI_VARARG_IMPL(m, count, ...) BOOST_DI_VARARG_IMPL2(m, count, __VA_ARGS__)

#define BOOST_DI_INJECT_HPP

#if !defined(BOOST_DI_INJECTOR)
    #define BOOST_DI_INJECTOR boost_di_injector__
#endif

#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
    #define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif

#define BOOST_DI_GEN_ARGS(i, ...) BOOST_DI_GEN_ARGS_IMPL(BOOST_DI_VARARG_IMPL(ARG, i, __VA_ARGS__,), i)
#define BOOST_DI_GEN_ARGS_IMPL(p, i) struct arg##i { BOOST_DI_IF(BOOST_DI_IBP(p))(BOOST_DI_GEN_NAME_ARG p BOOST_DI_GEN_NAME_ARG_END(), BOOST_DI_GEN_ARG(p)) };
#define BOOST_DI_GEN_NAME_ARG(p) static auto BOOST_DI_CAT(BOOST_DI_INJECTOR, name)() { auto p; return named; } static void BOOST_DI_CAT(BOOST_DI_INJECTOR, arg)(
#define BOOST_DI_GEN_NAME_ARG_END() );
#define BOOST_DI_GEN_ARG(p) static void BOOST_DI_CAT(BOOST_DI_INJECTOR, arg)(p);
#define BOOST_DI_GEN_CTOR(i, ...) BOOST_DI_GEN_CTOR_IMPL(BOOST_DI_VARARG_IMPL(ARG, i, __VA_ARGS__,), i)
#define BOOST_DI_GEN_CTOR_IMPL(p, i) BOOST_DI_IF(i)(BOOST_DI_COMMA(),) BOOST_DI_IF(BOOST_DI_IBP(p))(EAT p, p)
#define BOOST_DI_GEN_TYPE_LIST(i, ...) BOOST_DI_GEN_TYPE_LIST_IMPL(BOOST_DI_VARARG_IMPL(ARG, i, __VA_ARGS__,), i)
#define BOOST_DI_GEN_TYPE_LIST_IMPL(p, n)  BOOST_DI_IF(n)(BOOST_DI_COMMA(),) BOOST_DI_IF(BOOST_DI_IBP(p))( \
    const ::boost::di::aux::type<arg##n BOOST_DI_COMMA() ::std::true_type>& \
  , BOOST_DI_IF(ISEMPTY(p))(,const ::boost::di::aux::type<arg##n BOOST_DI_COMMA() ::std::false_type>&))

#if !defined(BOOST_DI_INJECT_TRAITS)
    #define BOOST_DI_INJECT_TRAITS(...) \
        struct BOOST_DI_INJECTOR { \
            EVAL(REPEAT(PP_NARG(__VA_ARGS__), BOOST_DI_GEN_ARGS, __VA_ARGS__)) \
            using type = ::boost::di::aux::type_list<EVAL(REPEAT(PP_NARG(__VA_ARGS__), BOOST_DI_GEN_TYPE_LIST, __VA_ARGS__))>; \
            static_assert(PP_NARG(__VA_ARGS__) <= BOOST_DI_CFG_CTOR_LIMIT_SIZE, "Number of constructor arguments is out of range - see BOOST_DI_CFG_CTOR_LIMIT_SIZE"); \
        }
#endif

#if !defined(BOOST_DI_INJECT)
    #define BOOST_DI_INJECT(type, ...) \
        BOOST_DI_INJECT_TRAITS(__VA_ARGS__); \
        type(EVAL(REPEAT(PP_NARG(__VA_ARGS__), BOOST_DI_GEN_CTOR, __VA_ARGS__)))
#endif

#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

namespace boost { namespace di { namespace type_traits {

template<class, class>
struct named { };
struct direct { };
struct uniform { };

BOOST_DI_CALL(BOOST_DI_HAS_TYPE, BOOST_DI_INJECTOR);

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

template<class>
struct arg;

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

#define BOOST_DI_PROVIDERS_STACK_OVER_HEAP_HPP

namespace boost { namespace di { namespace providers {

class stack_over_heap {
public:
    template<class T, class... TArgs>
    auto* get(const type_traits::direct&
            , const type_traits::heap&
            , TArgs&&... args) {
        return new T(std::forward<TArgs>(args)...);
    }

    template<class T, class... TArgs>
    auto* get(const type_traits::uniform&
            , const type_traits::heap&
            , TArgs&&... args) const {
        return new T{std::forward<TArgs>(args)...};
    }

    template<class T, class... TArgs>
    auto get(const type_traits::direct&
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T(std::forward<TArgs>(args)...);
    }

    template<class T, class... TArgs>
    auto get(const type_traits::uniform&
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
        return providers::stack_over_heap{};
    }

    auto policies() noexcept {
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
        auto create(const TProvider& provider) {
            if (in_scope_ && !object_) {
                object_.reset(provider.get());
            }
            return wrappers::shared<T>{object_};
        }

    private:
        std::shared_ptr<T> object_;
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
        auto create(const TProvider& provider) {
            if (!object_) {
                object_.reset(provider.get());
            }
            return wrappers::shared<T>{object_};
        }

    private:
        std::shared_ptr<T> object_;
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

#define BOOST_DI_CORE_POLICY_HPP

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(call_operator, operator());

template<class TDeps>
class policy {
    template<
        class TPolicy
      , class T
      , class TName
      , class TPolicies
      , class TDependency
      , class... TArgs
    > static void call_impl(const TPolicies& policies, TDependency&& dependency) noexcept {
        struct arg {
            using type = T;
            using name = TName;
            template<class T_, class TName_, class TDefault_>
            using resolve = decltype(binder::resolve<T_, TName_, TDefault_>((TDeps*)nullptr));
        };

        call_impl_args<arg, TDependency, TPolicy, TArgs...>(
            static_cast<const TPolicy&>(policies), dependency
        );
    }

    template<class TArg, class TDependency, class TPolicy, class... TArgs>
    static std::enable_if_t<has_call_operator<TPolicy, TArg>{}>
    call_impl_args(const TPolicy& policy, TDependency&&) noexcept {
        (policy)(TArg{});
    }

    template<class TArg, class TDependency, class TPolicy, class... TArgs>
    static std::enable_if_t<has_call_operator<TPolicy, TArg, TDependency, TArgs...>{}>
    call_impl_args(const TPolicy& policy, TDependency&& dependency) noexcept {
        (policy)(TArg{}, dependency, aux::type<TArgs>{}...);
    }

public:
    template<
        class T
      , class TName
      , class TInitialization
      , class TDependency
      , class... TArgs
      , class... TPolicies
    > static void call(const pool<aux::type_list<TPolicies...>>& policies
                     , TDependency&& dependency
                     , aux::pair<TInitialization, aux::type_list<TArgs...>>) noexcept {
        int _[]{0, (call_impl<TPolicies, T, TName, TPolicies, TDependency, TArgs...>(
            policies, dependency), 0)...}; (void)_;
    }
};

}}} // boost::di::core


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
    auto get(const TMemory& memory = {}) const {
        auto&& config = injector_.config_;
        return config.provider().template get<T>(
            TInitialization{}
          , memory
          , injector_.template create_t<TParent>(aux::type<TArgs>{})...
        );
    }

    const TInjector& injector_;
};

}}} // boost::di::core

#define BOOST_DI_CORE_INJECTOR_HPP

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(call, call);

template<class T, class TWrapper>
struct wrapper {
    TWrapper wrapper_;

    inline operator T() const noexcept {
        return wrapper_;
    }
};

template<class TDeps, class TConfig>
class injector : requires_unique_bindings<TDeps>, public pool<TDeps> {
    template<class, class> friend struct any_type;
    template<class...> friend struct provider;

    using pool_t = pool<TDeps>;

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
    T create() const {
        return create_impl<T>();
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

    template<class, class T>
    auto create_t(const aux::type<T>&) const {
        return create_impl<T>();
    }

    template<class TParent, class... Ts>
    auto create_t(const aux::type<any_type<Ts...>>&) const {
        return any_type<TParent, injector>{*this};
    }

    template<class, class T, class TName>
    auto create_t(const aux::type<type_traits::named<TName, T>>&) const {
        return create_impl<T, TName>();
    }

    template<class T, class TName = no_name>
    auto create_impl() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t>::type;
        using provider_t = provider<given_t, T, ctor_t, injector>;
        policy<pool_t>::template call<T, TName>(config_.policies(), dependency, ctor_t{});
        using wrapper_t = decltype(dependency.template create<T>(provider_t{*this}));
        using type = std::conditional_t<
            std::is_reference<T>{} && has_is_ref<dependency_t>{}
          , T
          , std::remove_reference_t<T>
        >;
        return wrapper<type, wrapper_t>{dependency.template create<T>(provider_t{*this})};
    }

    template<class TAction, class... Ts>
    void call_impl(const TAction& action, const aux::type_list<Ts...>&) noexcept {
        int _[]{0, (call_impl<Ts>(action), 0)...}; (void)_;
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
    decltype(auto) pass_arg(const T& arg, std::enable_if_t<!has_configure<T>{}>* = 0) const noexcept {
        return arg;
    }

    template<class T>
    decltype(auto) pass_arg(const T& arg, std::enable_if_t<has_configure<T>{}>* = 0) const noexcept {
        return arg.configure();
    }

    template<class... TArgs, class... Ts>
    auto create_from_injector(const injector<TArgs...>& injector
                            , const aux::type_list<Ts...>&) const noexcept {
        return pool<TDeps>(create_dep<Ts>(injector)...);
    }

    template<class TDependency, class TInjector>
    auto create_dep(const TInjector& injector) const noexcept {
        return TDependency{injector};
    }

    mutable TConfig config_;
};

}}} // boost::di::core

#define BOOST_DI_INJECTOR_HPP

namespace boost { namespace di { namespace detail {

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
  , class = typename core::is_injector<T>::type
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
    using type = aux::type_list<core::dependency<scopes::exposed<>, T>>;
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

