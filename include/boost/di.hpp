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

#define BOOST_DI_REQUIRES(...) \
    typename std::enable_if<(__VA_ARGS__), int>::type = 0

namespace boost { namespace di { namespace aux {

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
struct unique {
    template<class I>
    inline operator I() const noexcept {
        return object;
    }

    template<class I>
    inline operator I*() const noexcept { // only for compilation clean
        return {};
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
        using sp = sp_holder<boost::shared_ptr<I>>;
        if (auto* deleter = std::get_deleter<sp, I>(object)) {
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
        auto create(const TProvider&) const noexcept {
            return wrappers::unique<TExpected>{object_};
        }

        TExpected object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, std::reference_wrapper<TGiven>> {
        using is_ref = void;

        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            return object_;
        }

        std::reference_wrapper<TGiven> object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, std::shared_ptr<TGiven>> {
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
    public:
        template<class, class TProvider>
        auto create(const TProvider&) const noexcept {
            using wrapper = wrapper_traits_t<decltype(std::declval<TGiven>()())>;
            return wrapper{object_()};
        }

        TGiven object_;
    };

    template<class TExpected, class TGiven>
    struct scope<TExpected, TGiven, std::enable_if_t<is_lambda_expr<TGiven, const injector&>{}>> {
        template<class, class TProvider>
        auto create(const TProvider& provider) const noexcept {
            using wrapper = wrapper_traits_t<decltype((object_)(provider.injector_))>;
            return wrapper{(object_)(provider.injector_)};
        }

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


namespace boost { namespace di { namespace scopes {

template<class TScope = scopes::deduce>
class exposed {
public:
    static constexpr auto priority = false;

    template<class TExpected, class TGiven>
    class scope {
        template<class T, class = void>
        struct iprovider {
            virtual ~iprovider() = default;
            virtual T* get(const type_traits::heap& = {}) const noexcept = 0;
            virtual T  get(const type_traits::stack&) const noexcept = 0;
        };

        template<class T>
        struct iprovider<T, std::enable_if_t<!std::is_copy_constructible<T>{}>> {
            virtual ~iprovider() = default;
            virtual T* get(const type_traits::heap& = {}) const noexcept = 0;
        };

        template<class TInjector>
        class provider : public iprovider<TExpected> {
        public:
            explicit provider(const TInjector& injector)
                : injector_(injector)
            { }

            TExpected* get(const type_traits::heap&) const noexcept { // non override
                return injector_.template create<TExpected*>();
            }

            TExpected get(const type_traits::stack&) const noexcept { // non override
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
        std::shared_ptr<iprovider<TExpected>> provider_;
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
    const char* operator()() const noexcept { return nullptr; }
};

class config;
template<class...> class injector;

}} // boost::di


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
  , bool  TPriority = TScope::priority
> class dependency
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
        : scope_t{std::forward<T>(object)}
    { }

    template<class T>
    auto named(const T&) const noexcept {
        return dependency<TScope, TExpected, TGiven, T>{};
    }

    template<class T>
    auto in(const T&) const noexcept {
        return dependency<T, TExpected, TGiven, TName>{};
    }

    template<class T,
        BOOST_DI_REQUIRES(!is_injector<T>{} && std::is_same<TExpected, TGiven>{} && std::is_same<TScope, scopes::deduce>{})
    > auto to(T&& object) const noexcept {
        using dependency = dependency<
            scopes::external, TExpected, std::remove_reference_t<T>, TName
        >;
        return dependency{std::forward<T>(object)};
    }

    template<class T, BOOST_DI_REQUIRES(has_configure<T>{})>
    auto to(const T& object) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>, TExpected, decltype(std::declval<T>().configure()), TName
        >;
        return dependency{object.configure()};
    }

    template<class T, BOOST_DI_REQUIRES(has_deps<T>{})>
    auto to(const T& object) const noexcept {
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

#define BOOST_DI_CORE_BINDER_HPP

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

#define BOOST_DI_CORE_ANY_TYPE_HPP

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

template<class TParent>
using any_type_ = any_type<TParent>;

template<class>
struct is_any_type : std::false_type { };

template<class... TArgs>
struct is_any_type<any_type<TArgs...>> : std::true_type { };

}}} // boost::di::core

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
            using type = ::boost::di::aux::type_list< \
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
            using type = ::boost::di::aux::function_traits<decltype(inject)>::args; \
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

#define BOOST_DI_TYPE_TRAITS_CTOR_TRAITS_HPP

namespace boost { namespace di { namespace type_traits {

template<class, class T>
struct named {
    operator T();
    operator T&() const;
};
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

template<template<class...> class, template<class> class, class, class>
struct ctor_impl;

template<
    template<class...> class TIsConstructible
  , template<class> class TAny
  , class T
  , std::size_t... TArgs
> struct ctor_impl<TIsConstructible, TAny, T, std::index_sequence<TArgs...>>
    : aux::at_key_t<
          aux::type_list<>
        , std::true_type
        , ctor_args<
              TIsConstructible
            , T
            , TAny<T>
            , std::make_index_sequence<TArgs>
          >...
      >
{ };

template<
    template<class...> class TIsConstructible
  , template<class> class TAny
  , class T
> using ctor_impl_t =
    typename ctor_impl<
        TIsConstructible
      , TAny
      , T
      , std::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE + 1>
    >::type;

template<class, template<class> class, class>
struct ctor;

template<class T, template<class> class TAny>
struct ctor<T, TAny, aux::type_list<>>
    : aux::pair<uniform, ctor_impl_t<aux::is_braces_constructible, TAny, T>>
{ };

template<class T, template<class> class TAny, class... TArgs>
struct ctor<T, TAny, aux::type_list<TArgs...>>
    : aux::pair<direct, aux::type_list<TArgs...>>
{ };

} // type_traits

template<class T, template<class> class TAny>
struct ctor_traits_
    : type_traits::ctor<T, TAny, type_traits::ctor_impl_t<std::is_constructible, TAny, T>>
{ };

template<class T>
struct ctor_traits
    : ctor_traits_<T, core::any_type_>
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
  , template<class> class
  , class = typename BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<T>::type
> struct ctor_traits;

template<
    class T
  , template<class> class
  , class = typename BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T>>::type
> struct ctor_traits_impl;

template<class T, template<class> class TAny>
struct ctor_traits<T, TAny, std::true_type>
    : aux::pair<direct, parse_args_t<typename T::BOOST_DI_INJECTOR::type>>
{ };

template<class T, template<class> class TAny>
struct ctor_traits<T, TAny, std::false_type>
    : ctor_traits_impl<T, TAny>
{ };

template<class T, template<class> class TAny>
struct ctor_traits_impl<T, TAny, std::true_type>
    : aux::pair<
          direct
        , parse_args_t<typename di::ctor_traits_<T, TAny>::BOOST_DI_INJECTOR::type>
      >
{ };

template<class T, template<class> class TAny>
struct ctor_traits_impl<T, TAny, std::false_type>
    : di::ctor_traits_<T, TAny>
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
                object_ = std::shared_ptr<T>{provider.get()};
            }
            return wrappers::shared<T>{object_};
        }

    private:
        std::shared_ptr<T> object_;
    };
};

}}} // boost::di::scopes

#define BOOST_DI_CONCEPTS__HPP

namespace boost { namespace di { namespace concepts {

template <typename ignore>
struct lookup;

template <std::size_t ...ignore>
struct lookup<std::index_sequence<ignore...>> {
    template <class... ts>
    static aux::type_list<ts...>
    apply(decltype(ignore, (void*)nullptr)..., aux::no_decay<ts>*...);
};

template <std::size_t index, typename ...xs>
using eat = decltype(
    lookup<std::make_index_sequence<index>>::apply(
        (aux::no_decay<xs>*)nullptr...
    )
);

template <bool ...> struct bool_seq;

template<class X>
using never = std::false_type;

template <class x, class>
struct is_in;

template <class x, typename ...xs>
using is_in_impl = std::integral_constant<bool, !std::is_same<
    bool_seq<std::is_same<xs, x>{}...>,
    bool_seq<never<xs>{}...>
>{}>;

template <class x, class... ts>
struct is_in<x, aux::type_list<ts...>> : is_in_impl<x, ts...> {};

template<class... ts>
struct unique_impl;

template<class... ts, std::size_t... s>
struct unique_impl<std::index_sequence<s...>, ts...>
    : std::is_same<bool_seq<never<ts>{}...>, bool_seq<typename is_in<ts, typename eat<s + 1, ts...>::type>::type{}...>>
{ };

template<class... ts>
using unique = typename unique_impl<std::make_index_sequence<sizeof...(ts)>, ts...>::type;

template<class I, class T = void>
struct boundable : std::integral_constant<bool,
    std::is_convertible<T, I>{} || std::is_base_of<I, T>{}
> { };

template<class T>
struct expected {
    using type = aux::pair<
        aux::pair<typename T::expected, typename T::name>
      , std::integral_constant<bool, T::scope::priority>
    >;
};

template<class... Ts, class T>
struct boundable<aux::type_list<Ts...>, T>
    : std::integral_constant<bool, !std::is_same<bool_seq<never<Ts>{}...>, bool_seq<std::is_base_of<Ts, T>{}...>>{}>
{ };

template<class... Ts>
struct boundable<aux::type_list<Ts...>, void>
    : unique<typename expected<Ts>::type...>
{ };

}}} // boost::di::concepts

#define BOOST_DI_BINDINGS_HPP

namespace boost { namespace di {

template<class... Ts>
using any_of = aux::type_list<Ts...>;

template<class TExpected, class TGiven = TExpected, BOOST_DI_REQUIRES(concepts::boundable<TExpected, TGiven>{})>
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
      , class TIsRoot
      , class TPolicies
      , class TDependency
      , class... TArgs
    > static void call_impl(const TPolicies& policies, TDependency&& dependency) noexcept {
        struct arg {
            using type = T;
            using name = TName;
            using is_root = TIsRoot;
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
      , class TIsRoot
      , class TInitialization
      , class TDependency
      , class... TArgs
      , class... TPolicies
    > static void call(const pool<aux::type_list<TPolicies...>>& policies
                     , TDependency&& dependency
                     , aux::pair<TInitialization, aux::type_list<TArgs...>>) noexcept {
        int _[]{0, (call_impl<TPolicies, T, TName, TIsRoot, TPolicies, TDependency, TArgs...>(
            policies, dependency), 0)...}; (void)_;
    }
};

}}} // boost::di::core


namespace boost { namespace di { namespace core {

template<class...>
struct provider;

template<
    class TExpected
  , class TGiven
  , class TParent
  , class TInjector
  , class TInitialization
  , class... TArgs
> struct provider<
    TExpected
  , TGiven
  , TParent
  , aux::pair<TInitialization, aux::type_list<TArgs...>>
  , TInjector
> {
    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        auto&& config = injector_.config_;
        return config.provider().template get<TExpected, TGiven>(
            TInitialization{}
          , memory
          , injector_.template create_t<TParent>(aux::type<TArgs>{})...
        );
    }

    const TInjector& injector_;
};

}}} // boost::di::core

#define BOOST_DI_CONCEPTS_CREATABLE_HPP

namespace boost { namespace di { namespace concepts {

template<class, class, class>
struct creatable_impl;

template<class TScope, class T, class... TArgs>
struct creatable_impl<TScope, T, aux::pair<type_traits::uniform, aux::type_list<TArgs...>>> {
    using type = aux::is_braces_constructible<T, TArgs...>;
};

template<class TScope, class T, class... TArgs>
struct creatable_impl<TScope, T, aux::pair<type_traits::direct, aux::type_list<TArgs...>>> {
    using type = std::is_constructible<T, TArgs...>;
};

template<class TScope, class T, class... TArgs>
struct creatable_impl<scopes::exposed<TScope>, T, aux::pair<type_traits::direct, aux::type_list<TArgs...>>> {
    using type = std::true_type;
};

template<class TScope, class T, class... TArgs>
struct creatable_impl<scopes::exposed<TScope>, T, aux::pair<type_traits::uniform, aux::type_list<TArgs...>>> {
    using type = std::true_type;
};

template<class T, class... TArgs>
struct creatable_impl<scopes::external, T, aux::pair<type_traits::direct, aux::type_list<TArgs...>>> {
    using type = std::true_type;
};

template<class T, class... TArgs>
struct creatable_impl<scopes::external, T, aux::pair<type_traits::uniform, aux::type_list<TArgs...>>> {
    using type = std::true_type;
};

template<class TParent, class TDeps>
struct any {
    template<class T>
    using any_ = any<T, TDeps>;

    template<
        class T
      , class U = aux::decay_t<T>
      , class D = std::remove_reference_t<decltype(core::binder::resolve<U>((TDeps*)nullptr))>
      , class = std::enable_if_t<!(std::is_same<U, TParent>{} || std::is_base_of<TParent, U>{})>
      , class = std::enable_if_t<
            typename creatable_impl<
                typename D::scope
              , typename D::given
              , typename type_traits::ctor_traits<typename D::given, any_>::type
            >::type{}
        >
    > struct is_valid_expr { };

    template<class T, class = is_valid_expr<T>> operator T();
    template<class T, class = is_valid_expr<T>> operator T&() const;
};

template<class, class, class = void>
struct creatable : std::false_type { };

template<class T, class TDeps>
struct creatable<T, TDeps, aux::void_t<decltype(any<void, core::pool<TDeps>>{}.operator T())>>
    : std::true_type
{ };

}}} // boost::di::concepts

#define BOOST_DI_CORE_INJECTOR_HPP

namespace boost { namespace di { namespace core {

BOOST_DI_HAS_METHOD(call, call);

template<class T, class TWrapper>
struct wrapper {
    inline operator T() noexcept {
        return wrapper_;
    }

    TWrapper wrapper_;
};

template<class TDeps, class TConfig, BOOST_DI_REQUIRES(concepts::boundable<TDeps>{})>
class injector : public pool<TDeps> {
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

    template<class TDeps_, class TConfig_>
    injector(const injector<TDeps_, TConfig_>& injector) noexcept // non explicit
        : pool_t{init{}, create_from_injector(injector, TDeps{})}
    { }

    template<class T, BOOST_DI_REQUIRES(concepts::creatable<T, TDeps>{})>
    T create() const {
        using IsRoot = std::true_type;
        return create_impl<T, no_name, IsRoot>();
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

    template<class T, class TName = no_name, class TIsRoot = std::false_type>
    auto create_impl() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = std::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = typename dependency_t::given;
        using ctor_t = typename type_traits::ctor_traits<given_t, core::any_type_>::type;
        using provider_t = provider<expected_t, given_t, T, ctor_t, injector>;
        policy<pool_t>::template call<T, TName, TIsRoot>(
            ((TConfig&)config_).policies(), dependency, ctor_t{}
        );
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

    template<class TDeps_, class TConfig_, class... Ts>
    auto create_from_injector(const injector<TDeps_, TConfig_>& injector
                            , const aux::type_list<Ts...>&) const noexcept {
        return pool<TDeps>(create_dep<Ts>(injector)...);
    }

    template<class TDependency, class TInjector>
    auto create_dep(const TInjector& injector) const noexcept {
        return TDependency{injector};
    }

    TConfig config_;
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

#define BOOST_DI_CONCEPTS_CONFIGURABLE_HPP

namespace boost { namespace di { namespace concepts {

template<class, class = void>
struct configurable : std::false_type { };

template<class T>
struct configurable<T, aux::void_t<decltype(std::declval<T>().policies()), decltype(std::declval<T>().provider())>>
    : std::true_type
{ };

}}} // boost::di::concepts

#define BOOST_DI_MAKE_INJECTOR_HPP

namespace boost { namespace di {

template<class TConfig = ::BOOST_DI_CFG
       , BOOST_DI_REQUIRES(concepts::configurable<TConfig>())
       , class... TArgs
> inline auto make_injector(const TArgs&... args) noexcept {
    return detail::injector<TConfig, TArgs...>(args...);
}

}} // boost::di

#endif

#endif

