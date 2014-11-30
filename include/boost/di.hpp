//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_HPP
#define BOOST_DI_HPP

#include "boost/di/inject.hpp"

#if defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS)

// annotations
#include "boost/di/named.hpp"

// scopes
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/scopes/exposed.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/singleton.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/unique.hpp"

// bindings
#include "boost/di/bindings.hpp"

// injectors
#include "boost/di/injector.hpp"
#include "boost/di/make_injector.hpp"

#else

#include <string>
#include <new>
#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/aux_/memory.hpp"

namespace boost {
namespace di {

struct no_name { };

template<class T, class TName = no_name, class = void>
class named {
    named& operator=(const named&) = delete;

public:
    using named_type = T;
    using name = TName;

    named(const T& object = T{}) noexcept // non explicit
        : object_(object)
    { }

    named(const named& other) noexcept
        : object_(other.object_)
    { }

    operator T() const noexcept {
        return object_;
    }

private:
    T object_;
};

template<class T, class TName>
class named<const T&, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = const T&;
    using name = TName;

    named(const T& object = T{}) noexcept // non explicit
        : object_(object)
    { }

    named(const named& other) noexcept
        : object_(other.object_)
    { }

    operator const T&() const noexcept {
        return object_;
    }

private:
    std::reference_wrapper<const T> object_;
};

template<class T, class TName>
class named<T&, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = T&;
    using name = TName;

    named(T& object) noexcept // non explicit
        : object_(object)
    { }

    named(const named& other) noexcept
        : object_(other.object_)
    { }

    operator T&() noexcept {
        return object_;
    }

private:
    std::reference_wrapper<T> object_;
};

template<class T, class TName>
class named<T&&, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = T&&;
    using name = TName;

    named(T&& object) noexcept // non explicit
        : object_(std::move(object))
    { }

    named(const named& other) noexcept
        : object_(other.object_)
    { }

    operator T&&() noexcept {
        return std::move(object_);
    }

private:
    T object_;
};

template<class T, class TName>
class named<aux::unique_ptr<T>, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = aux::unique_ptr<T>;
    using name = TName;

    named(aux::unique_ptr<T> object = aux::unique_ptr<T>(new T{})) noexcept // non explicit
        : object_(std::move(object))
    { }

    operator aux::unique_ptr<T>() noexcept {
        return std::move(object_);
    }

    named(const named& other) noexcept
        : object_(new T(*other.object_))
    { }

private:
    aux::unique_ptr<T> object_;
};

template<class T, class TName>
class named<T, TName, std::enable_if_t<std::is_polymorphic<aux::remove_accessors_t<T>>{}>> {
public:
    using named_type = T;
    using name = TName;
};

} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

template<class T>
class copy {
public:
    copy(const T& value) noexcept // non explicit
        : value_(value)
    { }

    template<class I>
    inline I operator()(const aux::type<I>&, std::enable_if_t<!std::is_polymorphic<I>::value>* = 0) const noexcept {
        return value_;
    }

    template<class I>
    inline I* operator()(const aux::type<I*>&) const noexcept {
        return new I(value_); // ownership transfer
    }

    template<class I>
    inline const I* operator()(const aux::type<const I*>&) const noexcept {
        return I(value_); // ownership transfer
    }

    template<class I>
    inline aux::shared_ptr<I> operator()(const aux::type<aux::shared_ptr<I>>&) const noexcept {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<class I>
    inline aux_::shared_ptr<I> operator()(const aux::type<aux_::shared_ptr<I>>&) const noexcept {
        return aux_::shared_ptr<I>(new I(value_));
    }

    template<class I>
    inline aux::unique_ptr<I> operator()(const aux::type<aux::unique_ptr<I>>&) const noexcept {
        return aux::unique_ptr<I>(new I(value_));
    }

private:
    T value_;
};

template<class T>
class copy<T*> {
public:
    copy(T* value) noexcept // non explicit
        : value_(value)
    { }

    template<class I>
    inline I operator()(const aux::type<I>&, std::enable_if_t<!std::is_polymorphic<I>::value>* = 0) const noexcept {
        aux::unique_ptr<I> ptr(value_);
        return *ptr;
    }

    template<class I>
    inline I* operator()(const aux::type<I*>&) const noexcept {
        return value_; // ownership transfer
    }

    template<class I>
    inline const I* operator()(const aux::type<const I*>&) const noexcept {
        return value_; // ownership transfer
    }

    template<class I>
    inline aux::shared_ptr<I> operator()(const aux::type<aux::shared_ptr<I>>&) const noexcept {
        return aux::shared_ptr<I>(value_);
    }

    template<class I>
    inline aux_::shared_ptr<I> operator()(const aux::type<aux_::shared_ptr<I>>&) const noexcept {
        return aux_::shared_ptr<I>(value_);
    }

    template<class I>
    inline aux::unique_ptr<I> operator()(const aux::type<aux::unique_ptr<I>>&) const noexcept {
        return aux::unique_ptr<I>(value_);
    }

private:
    T* value_ = nullptr;
};

} // namespace wrappers
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

class unique {
public:
    static constexpr auto priority = false;

    template<class, class T>
    class scope {
    public:
        template<class, class TProvider>
        auto create(const TProvider& provider) const noexcept -> wrappers::copy<decltype(provider.get())> {
            return provider.get();
        }
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

template<class T>
class shared {
    template<class U, class TShared = aux::shared_ptr<U>>
    class sp_holder {
    public:
        explicit sp_holder(const TShared& value)
            : value_(value)
        { }

    private:
        TShared value_;
    };

public:
    shared() { }

    shared(const aux::shared_ptr<T>& value) noexcept // non explicit
        : value_(value)
    { }

    inline bool operator!() const noexcept {
        return !value_;
    }

    inline void reset(T* ptr = 0) noexcept {
        return value_.reset(ptr);
    }

    template<class I>
    inline aux::shared_ptr<I> operator()(const aux::type<aux::shared_ptr<I>>&) const noexcept {
        return value_;
    }

    template<class I>
    inline aux_::shared_ptr<I> operator()(const aux::type<aux_::shared_ptr<I>>&) const noexcept {
        aux_::shared_ptr<sp_holder<T>> sp(new sp_holder<T>(value_));
        return aux_::shared_ptr<T>(sp, value_.get());
    }

    template<class I>
    inline aux::weak_ptr<I> operator()(const aux::type<aux::weak_ptr<I>>&) const noexcept {
        return value_;
    }

private:
    aux::shared_ptr<T> value_;
};

} // namespace wrappers
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

class singleton {
public:
    static constexpr auto priority = false;

    template<class, class T>
    class scope {
    public:
        template<class, class TProvider>
        decltype(auto) create(const TProvider& provider) const noexcept {
            if (!get_instance()) {
                get_instance().reset(provider.get_ptr());
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

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

template<class T>
class value {
public:
    value(const T& value) noexcept // non explicit
        : value_(value)
    { }

    inline T operator()(const aux::type<T>&) const noexcept {
        return value_;
    }

    inline T* operator()(const aux::type<T*>&) const noexcept {
        return new T(value_);
    }

    inline const T* operator()(const aux::type<const T*>&) const noexcept {
        return new T(value_);
    }

    inline T&& operator()(const aux::type<T&&>&) noexcept {
        return std::move(value_);
    }

    template<class I>
    inline aux::shared_ptr<I> operator()(const aux::type<aux::shared_ptr<I>>&) const noexcept {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<class I>
    inline aux_::shared_ptr<I> operator()(const aux::type<aux_::shared_ptr<I>>&) const noexcept {
        return aux_::shared_ptr<I>(new I(value_));
    }

    template<class I>
    inline aux::unique_ptr<I> operator()(const aux::type<aux::unique_ptr<I>>&) const noexcept {
        return aux::unique_ptr<I>(new I(value_));
    }

private:
    T value_;
};

} // namespace wrappers
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

template<class T>
class reference {
    reference& operator=(const reference&) = delete;

public:
    reference(T& value) noexcept // non explicit
        : value_(value)
    { }

    inline T& operator()(const aux::type<T&>&) const noexcept {
        return value_;
    }

private:
    T& value_;
};

} // namespace wrappers
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

BOOST_DI_HAS_TYPE(result_type);
BOOST_DI_HAS_METHOD(call_operator, operator());

template<class T, class U>
using has_lambda =
    std::integral_constant<bool, has_call_operator<T, U>::value && !has_result_type<T>::value>;

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

namespace boost {
namespace di {
namespace type_traits {

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
struct scope_traits<aux::shared_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const aux::shared_ptr<T>&> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<aux_::shared_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const aux_::shared_ptr<T>&> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<aux::weak_ptr<T>> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<const aux::weak_ptr<T>&> {
    using type = scopes::singleton;
};

template<class T>
struct scope_traits<aux::unique_ptr<T>> {
    using type = scopes::unique;
};

template<class T>
struct scope_traits<const aux::unique_ptr<T>&> {
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

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

class deduce {
public:
    static constexpr auto priority = false;

    template<class TExpected, class T>
    class scope {
    public:
        template<class TDst, class TProvider>
        decltype(auto) create(const TProvider& provider) const noexcept {
            using scope = typename type_traits::scope_traits_t<TDst>::template scope<TExpected, T>;
            return scope{}.template create<T>(provider);
        }
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

class exposed {
public:
    static constexpr auto priority = false;

    template<class T>
    struct provider {
        provider() noexcept {}

        template<typename TProvider>
        explicit provider(const TProvider& provider) noexcept
            : f([=]{ return provider.get(); })
        { }

        T* get_ptr() const noexcept { return f(); }
        T* get() const noexcept { return f(); }

        std::function<T*()> f;
    };

    template<class TExpected, class T>
    class scope {
    public:
        scope() noexcept { }

        template<class TProvider>
        explicit scope(const TProvider& provider) noexcept
            : provider_{provider}
        { }

        template<class TDst, class TProvider>
        decltype(auto) create(const TProvider&) const noexcept {
            using scope = typename type_traits::scope_traits_t<TDst>::template scope<TExpected, T>;
            return scope{}.template create<TDst>(provider_);
        }

    private:
        provider<T> provider_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

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
        decltype(auto) create(const TProvider& provider) const noexcept {
            if (in_scope_ && !object_) {
                object_.reset(provider.get());
            }
            return object_;
        }

    private:
        mutable wrappers::shared<T> object_;
        bool in_scope_ = false;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

class shared {
public:
    static constexpr auto priority = false;

    template<class, class T>
    class scope {
    public:
        template<class, class TProvider>
        decltype(auto) create(const TProvider& provider) const noexcept {
            if (!object_) {
                object_.reset(provider.get_ptr());
            }
            return object_;
        }

    private:
        mutable wrappers::shared<T> object_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace core {

template<class TExpected, class TName>
struct dependency_concept { };

template<class T, class TDependency>
struct dependency_impl : aux::pair<T, TDependency>
{ };

template<class... Ts, class TName, class TDependency>
struct dependency_impl<dependency_concept<aux::type_list<Ts...>, TName>, TDependency>
    : aux::pair<dependency_concept<Ts, TName>, TDependency>...
{ };

template<
    class TScope
  , class TExpected
  , class TGiven = TExpected
  , class TName = no_name
  , bool TPriority = TScope::priority
>
class dependency
    : public TScope::template scope<TExpected, TGiven>
    , public dependency_impl<
          dependency_concept<TExpected, TName>
        , dependency<TScope, TExpected, TGiven, TName, TPriority>
      > {
public:
    using type = dependency;
    using scope = TScope;
    using expected = TExpected;
    using given = TGiven;
    using name = TName;

    dependency() noexcept { }

    template<class T>
    explicit dependency(T&& object) noexcept
        : TScope::template scope<TExpected, TGiven>(std::forward<T>(object))
    { }

    template<class T, class TInjector>
    dependency(T&& object, const TInjector& injector) noexcept
        : TScope::template scope<TExpected, TGiven>(std::forward<T>(object), injector)
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
        using dep = dependency<scopes::external, TExpected, aux::remove_accessors_t<T>, TName>;
        return dep{std::forward<T>(object)};
    }
};

template<class>
struct is_dependency : std::false_type { };

template<
    class TScope
  , class TExpected
  , class TGiven
  , class TName
  , bool TPriority
>
struct is_dependency<
    dependency<TScope, TExpected, TGiven, TName, TPriority>
> : std::true_type { };

} // namespace core
} // namespace di
} // namespace boost

namespace boost {
namespace di {

template<class... Ts>
using any_of = aux::type_list<Ts...>;

template<class TExpected, class TGiven = TExpected>
core::dependency<scopes::deduce, TExpected, TGiven> bind{};

extern constexpr scopes::deduce deduce{};
extern constexpr scopes::unique unique{};
extern constexpr scopes::shared shared{};
extern constexpr scopes::singleton singleton{};

template<class TName>
constexpr scopes::session<TName>
session(const TName&) noexcept { return {}; }

template<class TName>
constexpr scopes::session_entry<TName>
session_entry(const TName&) noexcept { return {}; }

template<class TName>
constexpr scopes::session_exit<TName>
session_exit(const TName&) noexcept { return {}; }

} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace core {

template<
    class T = aux::none_t
  , class TInjector = aux::none_t
  , class TProvider = aux::none_t
  , class TRefs = aux::none_t
  , class TPolicies = aux::none_t
>
class any_type {
    template<class TValueType, class TRefType>
    using ref_type_t = std::conditional_t<
          std::is_same<TValueType, aux::none_t>{}
        , TValueType
        , TRefType
      >;

    any_type& operator=(const any_type&) = delete;
    //any_type(const any_type&) = delete;

    template<class U>
    using is_not_same_t = std::enable_if_t<
        !std::is_same<aux::make_plain_t<U>
      , aux::make_plain_t<T>>::value
    >;

public:
    any_type() noexcept { }

    any_type(const TInjector& creator
           , const TProvider& provider
           , TRefs& refs
           , const TPolicies& policies) noexcept
        : creator_(creator)
        , provider_(provider)
        , refs_(refs)
        , policies_(policies)
    { }

    template<class U, class = is_not_same_t<U>>
    operator const U&() const noexcept {
        return creator_.template create_impl<const U&, T>(provider_, refs_, policies_);
    }

    template<class U, class = is_not_same_t<U>>
    operator U&() const noexcept {
        return creator_.template create_impl<U&, T>(provider_, refs_, policies_);
    }

    template<class U, class = is_not_same_t<U>>
    operator U() noexcept {
        return creator_.template create_impl<U, T>(provider_, refs_, policies_);
    }

private:
    ref_type_t<TInjector, const TInjector&> creator_;
    ref_type_t<TProvider, const TProvider&> provider_;
    ref_type_t<TRefs, TRefs&> refs_;
    ref_type_t<TPolicies, const TPolicies&> policies_;
};

template<class>
struct is_any_type : std::false_type { };

template<class... TArgs>
struct is_any_type<any_type<TArgs...>> : std::true_type { };

} // namespace core
} // namespace di

    //class T
  //, class TInjector
  //, class TProvider
  //, class TRefs
  //, class TPolicies
    //di::core::any_type<
        //T
      //, TInjector
      //, TProvider
      //, TRefs
      //, TPolicies
   //>

} // namespace boost

namespace std {

template<
    class T
  , class TInjector
  , class TProvider
  , class TRefs
  , class TPolicies
>
struct is_integral<
    boost::di::core::any_type<
        T
      , TInjector
      , TProvider
      , TRefs
      , TPolicies
    >
> : ::std::true_type { };

} // namespace std

namespace boost {
namespace di {
namespace core {

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
    static const TDependency&
    resolve_impl(const aux::pair<TConcept, TDependency>* dep) noexcept {
        return static_cast<const TDependency&>(*dep);
    }

    template<
        class
      , class TConcept
      , class TScope
      , class TExpected
      , class TGiven
      , class TName
    >
    static decltype(auto) // priority scope
    resolve_impl(const aux::pair<TConcept, dependency<TScope, TExpected, TGiven, TName, true>>* dep) noexcept {
        return static_cast<const dependency<TScope, TExpected, TGiven, TName, true>&>(*dep);
    }

public:
    template<
        class T
      , class TDeps
      , class TDefault = dependency<scopes::deduce, aux::make_plain_t<T>>
    >
    static decltype(auto) resolve(const TDeps* deps) noexcept {
        using dependency = dependency_concept<
            aux::make_plain_t<T>
          , get_name_t<aux::remove_accessors_t<T>>
        >;

        return resolve_impl<TDefault, dependency>(deps);
    }
};

} // namespace core
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace core {

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
    std::enable_if_t<std::is_base_of<T, pool>{} && std::is_base_of<T, TPool>{}, T>
    inline get(const TPool& p) const noexcept { return p.template get<T>(); }

    template<class T, class TPool>
    std::enable_if_t<std::is_base_of<T, pool>{} && !std::is_base_of<T, TPool>{}, T>
    inline get(const TPool&) const noexcept { return {}; }

    template<class T, class TPool>
    std::enable_if_t<!std::is_base_of<T, pool>{}, const T&>
    inline get(const TPool&) const noexcept { return {}; }
};

} // namespace core
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace providers {

class min_allocs {
public:
    template<class T, class... TArgs>
    inline T* get_ptr(TArgs&&... args) const noexcept {
        return new (std::nothrow) T{std::forward<TArgs>(args)...};
    }

    template<class T, class... TArgs>
    inline T get_value(TArgs&&... args) const noexcept {
        return T{std::forward<TArgs>(args)...};
    }

    template<class T, class... TArgs>
    inline std::enable_if_t<std::is_polymorphic<T>{}, T*>
    get(TArgs&&... args) const noexcept {
        return get_ptr<T>(std::forward<TArgs>(args)...);
    }

    template<class T, class... TArgs>
    inline std::enable_if_t<!std::is_polymorphic<T>{}, T>
    get(TArgs&&... args) const noexcept {
        return get_value<T>(std::forward<TArgs>(args)...);
    }
};

} // namespace providers
} // namespace di
} // namespace boost

namespace boost {
namespace di {

namespace detail {

BOOST_DI_CALL(BOOST_DI_HAS_METHOD_CALL, BOOST_DI_INJECTOR, BOOST_DI_INJECTOR);

template<class T, std::size_t>
struct get_type {
    using type = T;
};

template<class, class, class>
struct ctor_impl;

template<class R, class T, std::size_t... TArgs>
struct ctor_impl<R, T, std::index_sequence<TArgs...>>
    : aux::pair<
          aux::is_braces_constructible_t<R, typename get_type<T, TArgs>::type...>
        , aux::type_list<typename get_type<T, TArgs>::type...>
      >
{ };

template<class, class>
struct ctor_traits_impl;

template<class T, std::size_t... Args>
struct ctor_traits_impl<T, std::index_sequence<Args...>>
    : aux::at_key<
          aux::type_list<>
        , std::true_type
        , ctor_impl<T, core::any_type<T>, std::make_index_sequence<Args>>...
      >
{ };

} // namespace detail

template<class T>
struct ctor_traits
    : detail::ctor_traits_impl<
          T
        , std::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE + 1>
      >
{ };

template<class T>
struct ctor_traits<std::basic_string<T>> {
    static void BOOST_DI_INJECTOR();
};

namespace type_traits {

template<
    class T
  , class = typename BOOST_DI_CAT(detail::has_, BOOST_DI_INJECTOR)<T>::type
>
struct ctor_traits;

template<
    class T
  , class = typename BOOST_DI_CAT(detail::has_, BOOST_DI_INJECTOR)<di::ctor_traits<T>>::type
>
struct ctor_traits_impl;

template<class T>
struct ctor_traits<T, std::true_type>
    : aux::function_traits<decltype(&T::BOOST_DI_INJECTOR)>::args
{ };

template<class T>
struct ctor_traits<T, std::false_type>
    : ctor_traits_impl<T>::type
{ };

template<class T>
struct ctor_traits_impl<T, std::true_type>
    : aux::function_traits<decltype(&di::ctor_traits<T>::BOOST_DI_INJECTOR)>::args
{ };

template<class T>
struct ctor_traits_impl<T, std::false_type>
    : di::ctor_traits<T>::type
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

namespace detail {

BOOST_DI_HAS_METHOD_SIGN(call_operator, opeartor());

template<class TValueType, class T>
using is_convertible_to_ref =
    has_call_operator<TValueType, const T&(TValueType::*)(const aux::type<const T&>&) const>;

template<class TResult, class T, class TWrapper>
inline std::enable_if_t<!std::is_copy_constructible<T>{}, const TResult&>
copy(aux::shared_ptr<void>& refs, const TWrapper& wrapper) noexcept {
    refs.reset(wrapper(aux::type<T*>()));
    return *refs;
}

template<class T>
struct holder {
    explicit holder(const T& wrapper) noexcept
        : held(wrapper)
    { }

    T held;
};

template<class TResult, class T, class TWrapper>
inline std::enable_if_t<std::is_copy_constructible<T>{}, const TResult&>
copy(aux::shared_ptr<void>& refs, const TWrapper& wrapper) noexcept {
    aux::shared_ptr<holder<TResult>> sp(new holder<TResult>(wrapper(aux::type<T>())));
    refs = sp;
    return sp->held;
}

template<class T>
class universal_impl {
public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>&, const TWrapper& wrapper) noexcept
        : object_(wrapper(aux::type<T>()))
    { }

    inline operator T() const noexcept {
        return object_;
    }

    inline operator T&&() noexcept {
        return std::move(object_);
    }

private:
    T object_;
};

template<class T>
class universal_impl<const T&> {
public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>&
                 , const TWrapper& wrapper
                 , std::enable_if_t<is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : object_(wrapper(aux::type<const T&>()))
    { }

    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>& refs
                 , const TWrapper& wrapper
                 , std::enable_if_t<!is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : object_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    inline operator const T&() const noexcept {
        return object_;
    }

private:
    std::reference_wrapper<const T> object_;
};

template<class T, class TName>
class universal_impl<named<T, TName>> {
public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>&, const TWrapper& wrapper) noexcept
        : object_(wrapper(aux::type<T>()))
    { }

    inline operator T() const noexcept {
        return object_;
    }

    inline operator named<T, TName>() const noexcept {
        return object_;
    }

private:
    T object_;
};

template<class T, class TName>
class universal_impl<named<const T&, TName>> {
public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>&
                 , const TWrapper& wrapper
                 , std::enable_if_t<is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : object_(wrapper(aux::type<const T&>()))
    { }

    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>& refs
                 , const TWrapper& wrapper
                 , std::enable_if_t<!is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : object_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    inline operator named<const T&, TName>() const noexcept {
        return object_;
    }

private:
    named<const T&, TName> object_;
};

template<class T, class TName>
class universal_impl<const named<T, TName>&> {
public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>& refs
                 , const TWrapper& wrapper) noexcept
        : object_(copy<named<T, TName>, T, TWrapper>(refs, wrapper))
    { }

    inline operator const named<T, TName>&() const noexcept {
        return object_;
    }

private:
    std::reference_wrapper<const named<T, TName>> object_;
};

template<class T, class TName>
class universal_impl<const named<const T&, TName>&> {
    universal_impl& operator=(const universal_impl&);

public:
    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>& refs
                 , const TWrapper& wrapper
                 , std::enable_if_t<is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : refs_(refs)
        , object_(wrapper(aux::type<const T&>()))
    { }

    template<class TWrapper>
    universal_impl(aux::shared_ptr<void>& refs
                 , const TWrapper& wrapper
                 , std::enable_if_t<!is_convertible_to_ref<TWrapper, T>{}>* = 0) noexcept
        : refs_(refs)
        , object_(copy<T, T, TWrapper>(refs, wrapper))
    { }

    inline operator const named<const T&, TName>&() const noexcept {
        aux::shared_ptr<holder<named<const T&, TName>>> object(
            new holder<named<const T&, TName>>(object_)
        );
        refs_ = object;
        return object->held;
    }

private:
    aux::shared_ptr<void>& refs_;
    named<const T&, TName> object_;
};

} // namespace detail

template<class T>
using universal = detail::universal_impl<T>;

} // namespace wrappers
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace core {

BOOST_DI_HAS_METHOD(configure, configure);
BOOST_DI_HAS_METHOD(call, call);

template<class TDeps = aux::type_list<>, typename TDefaultProvider = providers::min_allocs>
class injector : public pool<TDeps> {
    template<class, class, class, class, class>
    friend class any_type;

    using pool_t = pool<TDeps>;

    template<class T, class TDependency>
    struct data {
        using type = T;
        using dependency = TDependency;
        using binder = core::binder;
    };

    template<class...>
    struct provider_impl;

    template<
        class T
      , class TDependency
      , class TProvider
      , class TPolicies
      , class... TArgs
    >
    struct provider_impl<T, TDependency, TProvider, TPolicies, aux::type_list<TArgs...>> {
        const injector& injector_;
        const TProvider& provider_;
        const TPolicies& policies_;

        decltype(auto) get() const noexcept {
            aux::shared_ptr<void> v;
            return provider_.template get<TDependency>(
                injector_.create_impl<TArgs, T>(provider_, v, policies_)...
            );
        }

        decltype(auto) get_ptr() const noexcept {
            aux::shared_ptr<void> v;
            return provider_.template get_ptr<TDependency>(
                injector_.create_impl<TArgs, T>(provider_, v, policies_)...
            );
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

    template<class T>
    injector(const injector<T>& injector) noexcept // non explicit
        : pool_t{init{}, create_from_injector(injector, TDeps{})}
    { }

    template<class T, class... TArgs>
    T create(const TArgs&... args) const noexcept {
        pool<aux::type_list<TArgs...>> policies(args...);
        aux::shared_ptr<void> v;
        return create_impl<T, aux::none_t>(TDefaultProvider{}, v, policies);
    }

    template<class T, class TProvider, class... TArgs>
    T provide(const TProvider& provider, const TArgs&... args) const noexcept {
        pool<aux::type_list<TArgs...>> policies(args...);
        aux::shared_ptr<void> v;
        return create_impl<T, aux::none_t>(provider, v, policies);
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

    template<
        class T
      , class TParent
      , class TProvider
      , class TRefs
      , class TPolicies
    >
    decltype(auto)
    create_impl(const TProvider& provider, TRefs& refs, const TPolicies& policies
              , std::enable_if_t<is_any_type<T>{}>* = 0) const noexcept {
        return any_type<TParent, injector, TProvider, TRefs, TPolicies>{
            *this, provider, refs, policies
        };
    }

    template<
        class T
      , class TParent
      , class TProvider
      , class TRefs
      , class TPolicies
    >
    decltype(auto)
    create_impl(const TProvider& provider, TRefs& refs, const TPolicies& policies
              , std::enable_if_t<!is_any_type<T>{}>* = 0) const noexcept {
        return create_from_dep_impl<T>(provider, refs, policies);
    }

    template<
        class T
      , class TProvider
      , class TRefs
      , class TPolicies
    >
    decltype(auto)
    create_from_dep_impl(const TProvider& provider, TRefs& refs, const TPolicies& policies) const noexcept {
        const auto& dependency = binder::resolve<T>(this);
        using type = typename std::remove_reference_t<decltype(dependency)>::given;;
        using ctor = typename type_traits::ctor_traits<type>::type;
        call_policies<data<T, type>>(policies);
        return wrappers::universal<T>{refs, dependency.template create<T>(
            provider_impl<T, type, TProvider, TPolicies, ctor>{*this, provider, policies})
        };
    }

    template<class T, class... TPolicies>
    void call_policies(const pool<aux::type_list<TPolicies...>>& policies) const noexcept {
        void(call_policies_impl<TPolicies, T>(policies)...);
    }

    template<class TPolicy, class T, class TPolicies>
    void call_policies_impl(const TPolicies& policies) const noexcept {
        (static_cast<const TPolicy&>(policies))(T{});
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
    decltype(auto) pass_arg(const T& arg, std::enable_if_t<!has_configure<T>{}>* = 0) const noexcept {
        return arg;
    }

    template<class T>
    decltype(auto) pass_arg(const T& arg, std::enable_if_t<has_configure<T>{}>* = 0) const noexcept {
        return arg.configure();
    }

    template<class T, class... Ts>
    decltype(auto) create_from_injector(const injector<T>& injector, const aux::type_list<Ts...>&) const noexcept {
        return pool<TDeps>(create_dep<Ts>(injector)...);
    }

    template<class TDependency, class TInjector>
    decltype(auto) create_dep(const TInjector& injector) const noexcept {
        struct provider {
            TInjector injector_;

            decltype(auto) get() const noexcept {
                return injector_.template create<typename TDependency::given*>();
            }
        };

        return TDependency{provider{injector}};
    }
};

} // namespace core
} // namespace di
} // namespace boost

namespace boost {
namespace di {

namespace detail {

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
>
struct add_type_list;

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
using bindings_t = typename aux::join<typename add_type_list<Ts>::type...>::type;

} // detail

template<class... TArgs>
using injector = core::injector<detail::bindings_t<TArgs...>>;

} // namespace di
} // namespace boost

namespace boost {
namespace di {

template<class... TArgs>
inline decltype(auto) make_injector(const TArgs&... args) noexcept {
    return injector<TArgs...>(args...);
}

} // namespace di
} // namespace boost

#endif

#endif

