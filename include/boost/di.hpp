//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_HPP
#define BOOST_DI_HPP

#include "boost/di/aux_/config.hpp"
#include "boost/di/inject.hpp"

#if defined(BOOST_DI_CFG_NO_PREPROCESSED_HEADERS)

// annotations
#include "boost/di/named.hpp"

// scopes
#include "boost/di/scopes/deduce.hpp"
#include "boost/di/scopes/external.hpp"
#include "boost/di/scopes/session.hpp"
#include "boost/di/scopes/shared.hpp"
#include "boost/di/scopes/unique.hpp"

// bindings
#include "boost/di/bindings.hpp"

// injectors
#include "boost/di/injector.hpp"
#include "boost/di/make_injector.hpp"

#else

#include <vector>
#include <unordered_map>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <functional>
#include <boost/type_traits/is_integral.hpp>
#include "boost/di/aux_/ref.hpp"
#include "boost/di/aux_/mpl.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/config.hpp"

namespace boost {
namespace di {
namespace type_traits {

template<typename T>
using remove_accessors = std::remove_cv<
   typename std::remove_pointer<
       typename std::remove_reference<T>::type
   >::type
>;

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace type_traits {

template<typename>
struct make_plain;

BOOST_DI_HAS_MEMBER_TYPE(element_type);
BOOST_DI_HAS_MEMBER_TYPE(named_type);

template<typename T, typename = void>
struct deref_type {
    using type = T;
};

template<typename T>
struct deref_type<T, typename std::enable_if<has_element_type<T>::value>::type> {
    using type = typename T::element_type;
};

template<typename T>
struct deref_type<T, typename std::enable_if<has_named_type<T>::value>::type> {
    using type = typename make_plain<typename T::named_type>::type;
};

template<typename T>
struct make_plain
    : deref_type<
          typename remove_accessors<
              typename deref_type<
                  typename remove_accessors<T>::type
              >::type
          >::type
      >
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {

struct no_name { };

template<
    typename T
  , typename TName = no_name
  , typename = void
>
class named {
    named& operator=(const named&) = delete;

public:
    using named_type = T;
    using name = TName;

    named(T object) // non explicit
        : object_(object)
    { }

    named(const named& other)
        : object_(other.object_)
    { }

    operator T() const {
        return object_;
    }

private:
    T object_;
};

template<
    typename T
  , typename TName
>
class named<const T&, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = const T&;
    using name = TName;

    named(const T& object) // non explicit
        : object_(object)
    { }

    named(const named& other)
        : object_(other.object_)
    { }

    operator const T&() const {
        return object_;
    }

private:
    const T& object_;
};

template<
    typename T
  , typename TName
>
class named<T&, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = T&;
    using name = TName;

    named(T& object) // non explicit
        : object_(object)
    { }

    named(const named& other)
        : object_(other.object_)
    { }

    operator T&() {
        return object_;
    }

private:
    T& object_;
};

template<
    typename T
  , typename TName
>
class named<T&&, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = T&&;
    using name = TName;

    named(T&& object) // non explicit
        : object_(std::move(object))
    { }

    named(const named& other)
        : object_(other.object_)
    { }

    operator T&&() {
        return std::move(object_);
    }

private:
    T object_;
};

template<
    typename T
  , typename TName
>
class named<aux::unique_ptr<T>, TName> {
    named& operator=(const named&) = delete;

public:
    using named_type = aux::unique_ptr<T>;
    using name = TName;

    named(aux::unique_ptr<T> object) // non explicit
        : object_(std::move(object))
    { }

    operator aux::unique_ptr<T>() {
        return std::move(object_);
    }

    named(const named& other)
        : object_(new T(*other.object_))
    { }

private:
    aux::unique_ptr<T> object_;
};

template<
    typename T
  , typename TName
>
class named<
    T
  , TName
  , typename std::enable_if<
        std::is_polymorphic<
            typename type_traits::remove_accessors<T>::type
        >::value
    >::type
> {
public:
    using named_type = T;
    using name = TName;
};

} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

template<typename T>
class copy {
    using value_t = std::function<T*()>;

    template<typename I>
    class scoped_ptr {
    public:
        explicit scoped_ptr(I* ptr)
            : ptr_(ptr)
        { }

        ~scoped_ptr() { delete ptr_; }
        I& operator*() const { return *ptr_; }

    private:
        I* ptr_;
    };

public:
    template<typename TValueType>
    copy(const TValueType& value) // non explicit
        : value_(value)
    { }

    template<typename I>
    I operator()(const type<I>&, typename std::enable_if<!std::is_polymorphic<I>::value>::type* = 0) const {
        scoped_ptr<I> ptr(value_());
        return *ptr;
    }

    template<typename I>
    I* operator()(const type<I*>&) const {
        return value_(); // ownership transfer
    }

    template<typename I>
    const I* operator()(const type<const I*>&) const {
        return value_(); // ownership transfer
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I>>&) const {
        return aux::shared_ptr<I>(value_());
    }

    template<typename I>
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I>>&) const {
        return aux_::shared_ptr<I>(value_());
    }

    template<typename I>
    aux::auto_ptr<I> operator()(const type<aux::auto_ptr<I>>&) const {
        return aux::auto_ptr<I>(value_());
    }

    template<typename I>
    aux::unique_ptr<I> operator()(const type<aux::unique_ptr<I>>&) const {
        return aux::unique_ptr<I>(value_());
    }

private:
    value_t value_;
};

} // namespace wrappers
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

template<template<typename> class TWrapper = wrappers::copy>
class unique {
public:
    static const bool priority = false;

    template<typename TExpected>
    class scope {
    public:
        using result_type = TWrapper<TExpected>;

        result_type create(const std::function<TExpected*()>& f) {
            return f;
        }
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

template<typename T>
class shared {
    template<typename U, typename TShared = aux::shared_ptr<U>>
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

    shared(const aux::shared_ptr<T>& value) // non explicit
        : value_(value)
    { }

    bool operator!() const {
        return !value_;
    }

    void reset(T* ptr = 0) {
        return value_.reset(ptr);
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I>>&) const {
        return value_;
    }

    template<typename I>
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I>>&) const {
        aux_::shared_ptr<sp_holder<T>> sp(new sp_holder<T>(value_));
        return aux_::shared_ptr<T>(sp, value_.get());
    }

    template<typename I>
    aux::weak_ptr<I> operator()(const type<aux::weak_ptr<I>>&) const {
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

template<template<typename> class TWrapper = wrappers::shared>
class shared {
public:
    static const bool priority = false;

    template<typename TExpected>
    class scope {
    public:
        using result_type = TWrapper<TExpected>;

        result_type create(const std::function<TExpected*()>& f) {
            if (!object_) {
                object_.reset(f());
            }
            return object_;
        }

    private:
        result_type object_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

template<typename T>
class value {
public:
    value(const T& value) // non explicit
        : value_(value)
    { }

    T operator()(const type<T>&) const {
        return value_;
    }

    T* operator()(const type<T*>&) const {
        return new T(value_);
    }

    const T* operator()(const type<const T*>&) const {
        return new T(value_);
    }

    T&& operator()(const type<T&&>&) {
        return std::move(value_);
    }

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I>>&) const {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I>>&) const {
        return aux_::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    aux::auto_ptr<I> operator()(const type<aux::auto_ptr<I>>&) const {
        return aux::auto_ptr<I>(new I(value_));
    }

    template<typename I>
    aux::unique_ptr<I> operator()(const type<aux::unique_ptr<I>>&) const {
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

BOOST_DI_HAS_MEMBER_FUNCTION(call_operator, operator());

namespace scopes {

template<template<typename> class TWrapper = wrappers::value>
class external {
public:
    static const bool priority = true;

    template<typename TExpected>
    class scope {
    public:
        using result_type = TWrapper<TExpected>;

    private:
        class result_type_holder {
        public:
            template<typename T>
            explicit result_type_holder(const T& object)
                : object_(object)
            { }

            result_type operator()() const {
                return object_;
            }

        private:
            result_type object_;
        };

        template<typename TValueType, typename T>
        typename std::enable_if<!has_call_operator<TValueType>::value, std::function<result_type()>>::type
        convert_when_function(const T& object) {
            return object;
        }

        template<typename TValueType, typename T>
        typename std::enable_if<has_call_operator<TValueType>::value, result_type_holder>::type
        convert_when_function(const T& object) {
            return result_type_holder(object);
        }

    public:
        template<typename T>
        explicit scope(const T& object
                     , typename std::enable_if<has_call_operator<T>::value && !aux::is_reference_wrapper<T>::value>::type* = 0)
            : object_(convert_when_function<TExpected>(object))
        { }

        template<typename T>
        explicit scope(const T& object
                     , typename std::enable_if<!(has_call_operator<T>::value && !aux::is_reference_wrapper<T>::value)>::type* = 0)
            : object_(result_type_holder(object))
        { }

        result_type create() {
            return object_();
        }

    private:
        std::function<result_type()> object_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace type_traits {

template<typename T>
struct scope_traits {
    using type = scopes::unique<>;
};

template<typename T>
struct scope_traits<T&> {
    using type = scopes::external<>;
};

template<typename T>
struct scope_traits<const T&> {
    using type = scopes::unique<>;
};

template<typename T>
struct scope_traits<T*> {
    using type = scopes::unique<>;
};

template<typename T>
struct scope_traits<const T*> {
    using type = scopes::unique<>;
};

template<typename T>
struct scope_traits<aux::auto_ptr<T>> {
    using type = scopes::unique<>;
};

template<typename T>
struct scope_traits<aux::shared_ptr<T>> {
    using type = scopes::shared<>;
};

template<typename T>
struct scope_traits<const aux::shared_ptr<T>&> {
    using type = scopes::shared<>;
};

template<typename T>
struct scope_traits<aux_::shared_ptr<T>> {
    using type = scopes::shared<>;
};

template<typename T>
struct scope_traits<const aux_::shared_ptr<T>&> {
    using type = scopes::shared<>;
};

template<typename T>
struct scope_traits<aux::weak_ptr<T>> {
    using type = scopes::shared<>;
};

template<typename T>
struct scope_traits<const aux::weak_ptr<T>&> {
    using type = scopes::shared<>;
};

template<typename T>
struct scope_traits<aux::unique_ptr<T>> {
    using type = scopes::unique<>;
};

template<typename T>
struct scope_traits<const aux::unique_ptr<T>&> {
    using type = scopes::unique<>;
};

template<typename T>
struct scope_traits<T&&> {
    using type = scopes::unique<>;
};

template<typename T>
struct scope_traits<const T&&> {
    using type = scopes::unique<>;
};

template<typename T, typename TName>
struct scope_traits<named<T, TName>> {
    using type = typename scope_traits<T>::type;
};

template<typename T, typename TName>
struct scope_traits<const named<T, TName>&> {
    using type = typename scope_traits<T>::type;
};

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

class deduce {
public:
    static const bool priority = false;

    template<typename>
    struct scope {
        using result_type = none_t;
    };

    template<typename T>
    struct rebind {
        using other = typename type_traits::scope_traits<T>::type;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

class session_entry { };
class session_exit { };

template<template<typename> class TWrapper = wrappers::shared>
class session {
public:
    static const bool priority = false;

    template<typename TExpected>
    class scope {
    public:
        using result_type = TWrapper<TExpected>;

        void call(const session_entry&) {
            in_scope_ = true;
        }

        void call(const session_exit&) {
            in_scope_ = false;
            object_.reset();
        }

        result_type create(const std::function<TExpected*()>& f) {
            if (in_scope_ && !object_) {
                object_.reset(f());
            }
            return object_;
        }

    private:
        result_type object_;
        bool in_scope_ = false;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

template<typename T>
class reference {
    reference& operator=(const reference&);

public:
    reference(T& value) // non explicit
        : value_(value)
    { }

    reference(const reference& other)
        : value_(other.value_)
    { }

    T& operator()(const type<T&>&) const {
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
namespace type_traits {

template<typename>
struct function_traits;

template<typename R, typename... TArgs>
struct function_traits<R(*)(TArgs...)> {
    using result_type = R;
    using type = type_list<TArgs...>;
};

template<typename R, typename T, typename... TArgs>
struct function_traits<R(T::*)(TArgs...)> {
    using result_type = R;
    using base_type = T;
    using type = type_list<TArgs...>;
};

template<typename R, typename T, typename... TArgs>
struct function_traits<R(T::*)(TArgs...) const> {
    using result_type = R;
    using base_type = T;
    using type = type_list<TArgs...>;
};

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {
namespace detail {

template<typename T, typename TBind>
struct apply_bind : TBind::template apply<T>::type { };

template<typename T, typename TBind>
struct eval_bind : TBind::template eval<T>::type { };

template<typename... Ts>
struct requires_ {
    using type = requires_;

    template<typename T, typename TMultiplicationFactor = int_<10>>
    using apply = times<TMultiplicationFactor, typename apply_bind<T, Ts>::type...>;

    template<typename T>
    using eval = times<int_<1>, typename eval_bind<T, Ts>::type...>;
};

} // namespace detail
} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {
namespace detail {

template<typename T, typename TBind>
struct apply_for_all : TBind::template apply<T>::type { };

template<typename T, typename TBind>
struct eval_for_all : TBind::template eval<T>::type { };

template<typename... Ts>
struct when_ {
    using type = when_;

    template<typename T>
    using apply = typename max<int_<0>, typename apply_for_all<T, Ts>::type...>::type;

    template<typename T>
    using eval = typename max<int_<0>, typename eval_for_all<T, Ts>::type...>::type;
};

template<>
struct when_<> {
    using type = when_;

    template<typename T>
    using apply = int_<1>;

    template<typename>
    using eval = int_<0>;
};

} // namespace detail
} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

struct is_required_priority {
    template<typename T>
    using apply = int_<1 + T::dependency::scope::priority>;

    template<typename T>
    using eval = int_<1>;
};

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace type_traits {

template<typename T, typename U>
using is_same_base_of = bool_<
    std::is_base_of<U, T>::value || std::is_same<U, T>::value
>;

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

template<typename TValueType>
struct is_required_type {
    template<typename T>
    using apply = int_<
        di::type_traits::is_same_base_of<
            TValueType
          , typename di::type_traits::make_plain<typename T::type>::type
        >::value
    >;

    template<typename>
    using eval = int_<1>;
};

template<typename... Ts>
struct is_required_type<type_list<Ts...>> {
    template<typename T>
    using apply = sum<
        di::type_traits::is_same_base_of<
            typename di::type_traits::make_plain<typename T::type>::type
          , Ts
        >::value...
    >;

    template<typename>
    using eval = int_<1>;
};

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {

BOOST_DI_HAS_MEMBER_TYPE(result_type);

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TBind =
        detail::requires_<
            type_traits::is_required_priority
          , type_traits::is_required_type<TExpected>
          , detail::when_<>
        >
>
class dependency : public TScope::template scope<TExpected>
{
    using scope_type = typename TScope::template scope<TExpected>;
    using ref_type = scopes::external<wrappers::reference>;
    using shared_type = scopes::external<wrappers::shared>;
    using value_type = scopes::external<wrappers::value>;

    template<typename, typename = void>
    struct get_wrapper_impl {
        using type = value_type;
    };

    template<typename T>
    struct get_wrapper_impl<aux::shared_ptr<T>> {
        using type = shared_type;
    };

    template<typename T>
    struct get_wrapper_impl<T, typename std::enable_if<aux::is_reference_wrapper<T>::value>::type> {
        using type = ref_type;
    };

    template<
        typename T
      , typename = void
      , typename = void
      , typename = void
    >
    struct get_wrapper {
        using type = T;
    };

    template<typename T>
    struct get_wrapper<T, typename std::enable_if<has_result_type<T>::value>::type>
        : get_wrapper_impl<typename T::result_type>
    { };

    template<typename T>
    struct get_wrapper<T, typename std::enable_if<has_call_operator<T>::value>::type
                        , typename std::enable_if<!has_result_type<T>::value>::type
                        , typename std::enable_if<!aux::is_reference_wrapper<T>::value>::type>
        : get_wrapper_impl<
              typename di::type_traits::function_traits<
                  decltype(&T::operator())
              >::result_type
          >
    { };

public:
    using type = dependency;
    using scope = TScope;
    using expected = TExpected;
    using given = TGiven;
    using bind = TBind;

    template<typename T>
    struct rebind {
        using other = dependency<
            typename std::conditional<
                std::is_same<scope, scopes::deduce>::value
              , T
              , TScope
            >::type
          , TExpected
          , TGiven
          , TBind
        >;
    };

    dependency() { }

    template<typename T>
    explicit dependency(const T& object)
        : scope_type(object)
    { }

    template<typename T>
    static dependency<value_type, expected, T, TBind>
    to(const T& object, typename std::enable_if<!aux::is_reference_wrapper<T>::value>::type* = 0
                      , typename std::enable_if<!has_call_operator<T>::value>::type* = 0) {
        return dependency<value_type, expected, T, TBind>(object);
    }

    template<typename T>
    static dependency<ref_type, typename aux::unwrap_reference<T>::type, T, TBind>
    to(const T& object, typename std::enable_if<aux::is_reference_wrapper<T>::value>::type* = 0) {
        return dependency<ref_type, typename aux::unwrap_reference<T>::type, T, TBind>(object);
    }

    template<typename T>
    static dependency<typename get_wrapper<T>::type, expected, T, TBind>
    to(const T& object, typename std::enable_if<!aux::is_reference_wrapper<T>::value>::type* = 0
                      , typename std::enable_if<has_call_operator<T>::value>::type* = 0) {
        return dependency<typename get_wrapper<T>::type, expected, T, TBind>(object);
    }

    template<typename T>
    static dependency<shared_type, expected, T, TBind>
    to(const aux::shared_ptr<T>& object) {
        return dependency<shared_type, expected, T, TBind>(object);
    }
};

} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

BOOST_DI_HAS_MEMBER_TYPE(name);

template<typename T, typename = void>
struct get_name {
    struct no_name { };
    using type = no_name;
};

template<typename T>
struct get_name<T, typename std::enable_if<has_name<T>::value>::type> {
    using type = typename T::name;
};

template<typename TName>
struct is_required_name {
    template<typename T>
    using apply = int_<
        std::is_same<
            typename get_name<
                typename di::type_traits::remove_accessors<
                    typename T::type
                 >::type
            >::type
          , TName
        >::value
    >;

    template<typename>
    using eval = int_<1>;
};

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {

namespace detail {

template<typename TExpected, typename>
struct get_expected {
    using type = TExpected;
};

template<typename... Ts, typename TGiven>
struct get_expected<type_list<Ts...>, TGiven> {
    using type = TGiven;
};

} // namespace detail

template<
    typename TExpected
  , typename TGiven
>
struct bind
    : dependency<
          scopes::deduce
        , typename detail::get_expected<TExpected, TGiven>::type
        , TGiven
        , detail::requires_<
              type_traits::is_required_priority
            , type_traits::is_required_type<TExpected>
            , detail::when_<>
          >
      >
{
    template<typename... Ts>
    struct when
        : dependency<
              scopes::deduce
            , typename detail::get_expected<TExpected, TGiven>::type
            , TGiven
            , detail::requires_<
                  type_traits::is_required_priority
                , type_traits::is_required_type<TExpected>
                , detail::when_<Ts...>
              >
          >
    {
        template<typename TName>
        struct named
            : dependency<
                  scopes::deduce
                , typename detail::get_expected<TExpected, TGiven>::type
                , TGiven
                , detail::requires_<
                      type_traits::is_required_priority
                    , type_traits::is_required_type<TExpected>
                    , type_traits::is_required_name<TName>
                    , detail::when_<Ts...>
                  >
              >
        { };
    };

    template<typename TName>
    struct named
        : dependency<
              scopes::deduce
            , typename detail::get_expected<TExpected, TGiven>::type
            , TGiven
            , detail::requires_<
                  type_traits::is_required_priority
                , type_traits::is_required_type<TExpected>
                , type_traits::is_required_name<TName>
                , detail::when_<>
              >
          >
    {
        template<typename... Ts>
        struct when
            : dependency<
                  scopes::deduce
                , typename detail::get_expected<TExpected, TGiven>::type
                , TGiven
                , detail::requires_<
                      type_traits::is_required_priority
                    , type_traits::is_required_type<TExpected>
                    , type_traits::is_required_name<TName>
                    , detail::when_<Ts...>
                  >
              >
        { };
    };
};

} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {

template<typename, typename>
struct match;

template<typename T1, typename... Ts1, typename T2, typename... Ts2>
struct match<type_list<T1, Ts1...>, type_list<T2, Ts2...>>
    : bool_<std::is_same<T1, T2>::value && match<type_list<Ts1...>, type_list<Ts2...>>::value>
{ };

template<typename... Ts>
struct match<type_list<>, type_list<Ts...>>
    : std::true_type
{ };

template<typename... Ts>
struct match<type_list<Ts...>, type_list<>>
    : std::false_type
{ };

template<>
struct match<type_list<>, type_list<>>
    : std::false_type
{ };

template<typename T>
struct make_plain;

template<typename... Ts>
struct make_plain<type_list<Ts...>>
    : type_list<typename di::type_traits::make_plain<Ts>::type...>
{ };

template<typename, std::size_t>
struct take_last;

template<std::size_t N, typename T, typename... Ts>
struct take_last<type_list<T, Ts...>, N>
    : std::conditional<!N, type_list<T, Ts...>, typename take_last<type_list<Ts...>, N-1>::type>::type
{ };

template<std::size_t N>
struct take_last<type_list<>, N>
    : type_list<>
{ };

template<typename, typename>
struct is_same_call_stack;

template<typename... Ts, typename T>
struct is_same_call_stack<type_list<Ts...>, T>
    : std::conditional<std::is_same<type_list<Ts...>, T>::value, int_<sizeof...(Ts) + 1>, int_<0>>
{ };

template<typename... Ts>
struct call_stack {
    template<typename T>
    using apply = is_same_call_stack<
        typename make_plain<
            typename take_last<
                typename T::call_stack
              , size<typename T::call_stack>::value - sizeof...(Ts)
            >::type
        >::type
      , type_list<Ts...>
    >;

    template<typename T>
    using eval = int_<
        match<typename make_plain<typename T::call_stack>::type, type_list<Ts...>>::value
    >;
};

} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {

BOOST_DI_HAS_MEMBER_TYPE(given);

template<typename TScope>
class scope {
    template<typename T>
    using is_dependency = has_given<T>;

    template<typename T, typename = void>
    struct get_binding
        : dependency<TScope, T, T>
    { };

    template<typename T>
    struct get_binding<T, typename std::enable_if<is_dependency<T>::value>::type>
        : T::template rebind<TScope>::other
    { };

    template<typename... Ts>
    struct bind_impl
        : type_list<typename get_binding<Ts>::type...>
    { };

public:
    template<typename... Ts>
    using bind = typename bind_impl<Ts...>::type;
};

} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {

template<typename TExpected, typename TGiven = TExpected>
using bind = bindings::bind<TExpected, TGiven>;

template<bool V>
using bind_bool = bind<bool, bool_<V>>;

template<int N>
using bind_int = bind<int, int_<N>>;

template<typename T>
using bind_string = bind<std::string, T>;

template<typename TScope>
using scope = bindings::scope<TScope>;

template<typename... Ts>
using deduce = scope<scopes::deduce>::bind<Ts...>;

template<typename... Ts>
using unique = scope<scopes::unique<>>::bind<Ts...>;

template<typename... Ts>
using shared = scope<scopes::shared<>>::bind<Ts...>;

template<typename... Ts>
using session = scope<scopes::session<>>::bind<Ts...>;

template<typename... Ts>
using call_stack = bindings::call_stack<Ts...>;

template<typename... Ts>
using any_of = type_list<Ts...>;

} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace core {

struct init { };

template<typename = type_list<>>
class pool;

template<typename... TArgs>
class pool<type_list<TArgs...>> : public TArgs... {
public:
    using type = pool;

    template<typename... Ts>
    explicit pool(const Ts&... args)
        : Ts(args)...
    { }

    template<typename TPool>
    pool(const init&, const TPool& p)
        : pool(get<TArgs>(p)...)
    { (void)p; }

    template<typename T>
    inline const T& get() const {
        return static_cast<const T&>(*this);
    }

private:
    template<typename T, typename TPool>
    typename std::enable_if<std::is_base_of<T, pool>::value && std::is_base_of<T, TPool>::value, T>::type
    inline get(const TPool& p) const {
        return p.template get<T>();
    }

    template<typename T, typename TPool>
    typename std::enable_if<std::is_base_of<T, pool>::value && !std::is_base_of<T, TPool>::value, T>::type
    inline get(const TPool&) const {
        return T();
    }

    template<typename T, typename TPool>
    typename std::enable_if<!std::is_base_of<T, pool>::value, const T&>::type
    inline get(const TPool&) const {
        return T();
    }
};

} // namespace core
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace core {

template<
    typename T
  , typename TCallStack
  , typename TDependency
>
struct data {
    using type = T;
    using call_stack = TCallStack;
    using dependency = TDependency;
};

template<typename T, typename TCallStack, typename _>
using resolve_impl = pair<_, typename _::bind::template apply<data<T, TCallStack, _>>::type>;

template<typename T, typename TCallStack, typename _>
using eval_impl = typename _::bind::template eval<data<T, TCallStack, _>>::type;

template<typename>
struct binder;

template<typename... Ts>
struct binder<type_list<Ts...>> {
    template<
        typename T
      , typename TCallStack
      , typename TDefault =
            bindings::dependency<
                scopes::deduce
              , typename type_traits::make_plain<T>::type
            >
    >
    using resolve = typename greatest<
        pair<TDefault, int_<0>>
      , resolve_impl<T, TCallStack, Ts>...
    >::type::template rebind<typename scopes::deduce::rebind<T>::other>::other;

    template<
        typename T
      , typename TCallStack
    >
    using eval = typename or_<eval_impl<T, TCallStack, Ts>...>::type;
};

} // namespace core
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace core {

template<
    typename T = none_t
  , typename TCallStack = none_t
  , typename TCreator = none_t
  , typename TProvider = none_t
  , typename TDeps = none_t
  , typename TRefs = none_t
  , typename TVisitor = none_t
  , typename TPolicies = none_t
>
class any_type {
    any_type& operator=(const any_type&);

    template<typename TValueType, typename TRefType>
    using ref_type = std::conditional<
          std::is_same<TValueType, none_t>::value
        , TValueType
        , TRefType
      >;

public:
    typedef void any;
    any_type() { }

    any_type(TCreator& creator
           , const TProvider& provider
           , TDeps& deps
           , TRefs& refs
           , const TVisitor& visitor
           , const TPolicies& policies)
        : creator_(creator)
        , provider_(provider)
        , deps_(deps)
        , refs_(refs)
        , visitor_(visitor)
        , policies_(policies)
    { }

    any_type(const any_type& other)
        : creator_(other.creator_)
        , provider_(other.provider_)
        , deps_(other.deps_)
        , refs_(other.refs_)
        , visitor_(other.visitor_)
        , policies_(other.policies_)
    { }

    template<
        typename U
      , typename = typename std::enable_if<
            !type_traits::is_same_base_of<
                typename type_traits::make_plain<U>::type
              , typename type_traits::make_plain<T>::type
            >::value
        >::type
    >
    operator const U&() const {
        return creator_.template create<const U&, T, TCallStack>(
            provider_, deps_, refs_, visitor_, policies_
        );
    }

    template<
        typename U
      , typename = typename std::enable_if<
            !type_traits::is_same_base_of<
                typename type_traits::make_plain<U>::type
              , typename type_traits::make_plain<T>::type
            >::value
        >::type
    >
    operator U&() const {
        return creator_.template create<U&, T, TCallStack>(
            provider_, deps_, refs_, visitor_, policies_
        );
    }

    template<typename U>
    operator aux::auto_ptr<U>&() {
        return creator_.template create<aux::auto_ptr<U>, T, TCallStack>(
            provider_, deps_, refs_, visitor_, policies_
        );
    }

    BOOST_DI_WKND(MSVC)(
        template<typename U>
        operator aux::unique_ptr<U>() {
            return creator_.template create<aux::unique_ptr<U>, T, TCallStack>(
                provider_, deps_, refs_, visitor_, policies_
            );
        }
    )

    BOOST_DI_WKND(NO_MSVC)(
        template<
            typename U
          , typename = typename std::enable_if<
                !type_traits::is_same_base_of<
                    typename type_traits::make_plain<U>::type
                  , typename type_traits::make_plain<T>::type
                >::value
            >::type
        >
        operator U() {
            return creator_.template create<U, T, TCallStack>(
                provider_, deps_, refs_, visitor_, policies_
            );
        }
    )

private:
    typename ref_type<TCreator, TCreator&>::type creator_;
    typename ref_type<TProvider, const TProvider&>::type provider_;
    typename ref_type<TDeps, TDeps&>::type deps_;
    typename ref_type<TRefs, TRefs&>::type refs_;
    typename ref_type<TVisitor, const TVisitor&>::type visitor_;
    typename ref_type<TPolicies, const TPolicies&>::type policies_;
};

} // namespace core
} // namespace di

template<
    typename T
  , typename TCallStack
  , typename TCreator
  , typename TProvider
  , typename TDeps
  , typename TRefs
  , typename TVisitor
  , typename TPolicies
>
struct is_integral<
    di::core::any_type<
        T
      , TCallStack
      , TCreator
      , TProvider
      , TDeps
      , TRefs
      , TVisitor
      , TPolicies
   >
> : ::std::true_type { };

} // namespace boost

namespace std {

template<
    typename T
  , typename TCallStack
  , typename TCreator
  , typename TProvider
  , typename TDeps
  , typename TRefs
  , typename TVisitor
  , typename TPolicies
>
struct is_integral<
    boost::di::core::any_type<
        T
      , TCallStack
      , TCreator
      , TProvider
      , TDeps
      , TRefs
      , TVisitor
      , TPolicies
    >
> : ::std::true_type { };

} // namespace std

namespace boost {
namespace di {

BOOST_DI_HAS_MEMBER_FUNCTION(BOOST_DI_INJECTOR, BOOST_DI_INJECTOR);

template<typename T, typename Q>
struct ctor_traits_impl;

template<typename T, std::size_t... Args>
struct ctor_traits_impl<T, index_sequence<Args...>>
    : longest<typename genn<T, core::any_type<T>, Args>::type...>
{ };

template<typename T>
struct ctor_traits
    : ctor_traits_impl<T, typename make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE + 1>::type>
{ };

template<typename T>
struct ctor_traits<std::basic_string<T>> {
    static void BOOST_DI_INJECTOR();
};

namespace type_traits {

template<typename T, typename = bool_<BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<T>::value>>
struct ctor_traits;

template<typename T, typename = bool_<BOOST_DI_CAT(has_, BOOST_DI_INJECTOR)<di::ctor_traits<T>>::value>>
struct ctor_traits_impl;

template<typename T>
struct ctor_traits<T, std::true_type>
    : function_traits<decltype(&T::BOOST_DI_INJECTOR)>::type
{ };

template<typename T>
struct ctor_traits<T, std::false_type>
    : ctor_traits_impl<T>::type
{ };

template<typename T>
struct ctor_traits_impl<T, std::true_type>
    : function_traits<decltype(&di::ctor_traits<T>::BOOST_DI_INJECTOR)>::type
{ };

template<typename T>
struct ctor_traits_impl<T, std::false_type>
    : di::ctor_traits<T>::type
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace type_traits {

namespace detail {

template<typename T, typename TSignature>
class is_convertible {
    template<typename U>
    static yes_tag test(non_type<TSignature, &U::operator()>*);

    template<typename>
    static no_tag test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes_tag);
};

} // namespace detail

template<typename TValueType, typename T>
using is_convertible_to_ref = detail::is_convertible<
    TValueType, const T&(TValueType::*)(const type<const T&>&) const
>;

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

namespace detail {

template<typename TResult, typename T, typename TValueType>
inline typename std::enable_if<!std::is_copy_constructible<T>::value, const TResult&>::type
copy(std::vector<aux::shared_ptr<void>>& refs, const TValueType& value) {
    aux::shared_ptr<TResult> object(value(type<T*>()));
    refs.push_back(object);
    return *object;
}

template<typename T>
struct holder {
    explicit holder(const T& value)
        : held(value)
    { }

    T held;
};

template<typename TResult, typename T, typename TValueType>
inline typename std::enable_if<std::is_copy_constructible<T>::value, const TResult&>::type
copy(std::vector<aux::shared_ptr<void>>& refs, const TValueType& value) {
    aux::shared_ptr<holder<TResult>> object(new holder<TResult>(value(type<T>())));
    refs.push_back(object);
    return object->held;
}

template<typename T>
class universal_impl {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>&, const TValueType& value)
        : value_(std::bind<T>(value, type<T>()))
    { }

    operator T() const {
        return value_();
    }

private:
    std::function<T()> value_;
};

template<typename T>
class universal_impl<aux::auto_ptr<T>> {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs, const TValueType& value)
        : value_(new aux::auto_ptr<T>(value(type<aux::auto_ptr<T>>()).release()))
    {
        refs.push_back(aux::shared_ptr<aux::auto_ptr<T>>(value_));
    }

    operator aux::auto_ptr<T>&() {
        return *value_;
    }

private:
    aux::auto_ptr<T>* value_; // weak
};

template<typename T>
class universal_impl<const T&> {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>&
                 , const TValueType& value
                 , typename std::enable_if<type_traits::is_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : value_(std::bind<const T&>(value, type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TValueType& value
                 , typename std::enable_if<!type_traits::is_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : value_(std::bind(&copy<T, T, TValueType>, std::ref(refs), value))
    { }

    operator const T&() const {
        return value_();
    }

private:
    std::function<const T&()> value_;
};

template<typename T, typename TName>
class universal_impl<named<T, TName>> {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>&, const TValueType& value)
        : value_(std::bind<T>(value, type<T>()))
    { }

    operator T() const {
        return value_();
    }

    operator named<T, TName>() const {
        return value_();
    }

private:
    std::function<T()> value_;
};

template<typename T, typename TName>
class universal_impl<named<const T&, TName>> {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>&
                 , const TValueType& value
                 , typename std::enable_if<type_traits::is_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : value_(std::bind<const T&>(value, type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TValueType& value
                 , typename std::enable_if<!type_traits::is_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : value_(std::bind(&copy<T, T, TValueType>, std::ref(refs), value))
    { }

    operator named<const T&, TName>() const {
        return value_();
    }

private:
    std::function<named<const T&, TName>()> value_;
};

template<typename T, typename TName>
class universal_impl<const named<T, TName>&> {
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TValueType& value)
        : value_(std::bind(&copy<named<T, TName>, T, TValueType>, std::ref(refs), value))
    { }

    operator const named<T, TName>&() const {
        return value_();
    }

private:
    std::function<const named<T, TName>&()> value_;
};

template<typename T, typename TName>
class universal_impl<const named<const T&, TName>&> {
    universal_impl& operator=(const universal_impl&);

public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TValueType& value
                 , typename std::enable_if<type_traits::is_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : refs_(refs)
        , value_(std::bind<const T&>(value, type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void>>& refs
                 , const TValueType& value
                 , typename std::enable_if<!type_traits::is_convertible_to_ref<TValueType, T>::value>::type* = 0)
        : refs_(refs)
        , value_(std::bind(&copy<T, T, TValueType>, std::ref(refs), value))
    { }

    operator const named<const T&, TName>&() const {
        aux::shared_ptr<holder<named<const T&, TName>>> object(
            new holder<named<const T&, TName>>(value_())
        );
        refs_.push_back(object);
        return object->held;
    }

private:
    std::vector<aux::shared_ptr<void>>& refs_;
    std::function<named<const T&, TName>()> value_;
};

} // namespace detail

template<typename T>
class universal : public detail::universal_impl<T> {
public:
    using type = universal;
    using element_type = T;

    template<typename TValueType>
    universal(std::vector<aux::shared_ptr<void>>& refs, const TValueType& value)
        : detail::universal_impl<T>(refs, value)
    { }
};

} // namespace wrappers
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace core {

BOOST_DI_HAS_MEMBER_TYPE(any);

template<typename TDeps_>
class creator {
    using binder_t = binder<TDeps_>;

    template<typename TDependency>
    struct scope_create
        : type_traits::function_traits<
              decltype(&TDependency::create)
          >::type
    { };

    using scopes_type = std::unordered_map<
        std::type_index
      , aux::shared_ptr<void>
    >;

    template<typename T, typename TDependency>
    struct data_visitor {
        using type = T;
        using dependency = TDependency;
        using binder = binder_t;
    };

public:
    template<
        typename T
      , typename TParent
      , typename TCallStack
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    typename std::enable_if<
        has_any<T>::value
      , any_type<
            TParent
          , TCallStack
          , creator
          , TProvider
          , TDeps
          , TRefs
          , TVisitor
          , TPolicies
        >
    >::type
    create(const TProvider& provider
         , TDeps& deps
         , TRefs& refs
         , const TVisitor& visitor
         , const TPolicies& policies) {
        return any_type<
            TParent
          , TCallStack
          , creator
          , TProvider
          , TDeps
          , TRefs
          , TVisitor
          , TPolicies
        >(*this, provider, deps, refs, visitor, policies);
    }

    template<
        typename T
      , typename TParent
      , typename TCallStack
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    typename std::enable_if<!has_any<T>::value, wrappers::universal<T>>::type
    create(const TProvider& provider
         , TDeps& deps
         , TRefs& refs
         , const TVisitor& visitor
         , const TPolicies& policies) {
        using call_stack = typename std::conditional<
            std::is_same<TParent, none_t>::value
          , TCallStack
          , typename add<TCallStack, TParent>::type
        >::type;
        using eval_type = typename binder_t::template eval<T, call_stack>::type;
        using dependency_type = typename binder_t::template resolve<T, call_stack>::type;
        using propagate_call_stack = typename std::conditional<eval_type::value, call_stack, TCallStack>::type;

        //assert_policies<typename TPolicies::types, data_type>(policies);
        (visitor)(data_visitor<T, dependency_type>());

        return create_impl<T, dependency_type, propagate_call_stack>(
            provider, deps, refs, visitor, policies
        );
    }

private:
    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    typename std::enable_if<
        !size<typename scope_create<TDependency>::type>::value
      , wrappers::universal<T>
    >::type
    create_impl(const TProvider&
              , TDeps& deps
              , TRefs& refs
              , const TVisitor&
              , const TPolicies&) {
        return wrappers::universal<T>(
            refs, acquire<TDependency>(deps).create()
        );
    }

    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
    >
    typename std::enable_if<
        size<typename scope_create<TDependency>::type>::value
      , wrappers::universal<T>>::type
    create_impl(const TProvider& provider
              , TDeps& deps
              , TRefs& refs
              , const TVisitor& visitor
              , const TPolicies& policies) {
        return wrappers::universal<T>(
            refs
          , acquire<TDependency>(deps).create([&]{
                using ctor_type =
                    typename type_traits::ctor_traits<typename TDependency::given>::type;

                return create_impl<T, TDependency, TCallStack>(
                    provider
                  , deps
                  , refs
                  , visitor
                  , policies
                  , ctor_type()
                );
            })
        );
    }

    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TProvider
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
      , typename... TArgs
    >
    typename TDependency::expected*
    create_impl(const TProvider& provider
              , TDeps& deps
              , TRefs& refs
              , const TVisitor& visitor
              , const TPolicies& policies
              , const type_list<TArgs...>&) {
        (void)provider; (void)deps; (void)refs; (void)visitor; (void)policies;
        return provider.template
            get<typename TDependency::expected, typename TDependency::given>(
                create<TArgs, T, TCallStack>(provider, deps, refs, visitor, policies)...
        );
    }

    //static typename enable_if<aux::mpl::empty<TSeq>>::type
    //assert_policies(const TPolicies&) { }

    //template<typename TSeq, typename T, typename TPolicies>
    //static typename disable_if<aux::mpl::empty<TSeq>>::type
    //assert_policies(const TPolicies& policies) {
        //typedef typename aux::mpl::front<TSeq>::type policy;
        //static_cast<const policy&>(policies).template assert_policy<T>();
        //assert_policies<typename aux::mpl::pop_front<TSeq>::type, T>(policies);

    template<typename TDependency, typename TDeps>
    typename std::enable_if<std::is_base_of<TDependency, TDeps>::value, TDependency&>::type
    acquire(TDeps& deps) {
        return static_cast<TDependency&>(deps);
    }

    template<typename TDependency, typename TDeps>
    typename std::enable_if<!std::is_base_of<TDependency, TDeps>::value, TDependency&>::type
    acquire(TDeps&) {
        auto it = scopes_.find(std::type_index(typeid(TDependency)));
        if (it != scopes_.end()) {
            return *static_cast<TDependency*>(it->second.get());
        }

        aux::shared_ptr<TDependency> dependency(new TDependency());
        scopes_[std::type_index(typeid(TDependency))] = dependency;
        return *dependency;
    }

    scopes_type scopes_;
};

} // namespace core
} // namespace di
} // namespace boost

namespace boost {

namespace mpl {
    struct string_tag;
    template<typename> struct c_str;
} // namespace mpl

namespace di {
namespace core {

BOOST_DI_HAS_MEMBER_TYPE(tag);
BOOST_DI_HAS_MEMBER(value);

template<typename, typename = void>
struct is_mpl_string
    : std::false_type
{ };

template<typename T>
struct is_mpl_string<T, typename std::enable_if<has_tag<T>::value>::type>
    : std::is_same<mpl::string_tag, typename T::tag>
{ };

template<typename T>
using is_explicit = bool_<has_value<T>::value || is_mpl_string<T>::value>;

class provider {
public:
    template<typename TExpected, typename TGiven>
    typename std::enable_if<!is_explicit<TGiven>::value, TExpected*>::type
    get() const {
        return new TGiven();
    }

    template<typename TExpected, typename TGiven>
    typename std::enable_if<has_value<TGiven>::value, TExpected*>::type
    get() const {
        return new TExpected(TGiven::value);
    }

    template<typename TExpected, typename TGiven>
    typename std::enable_if<is_mpl_string<TGiven>::value, TExpected*>::type
    get() const {
        return new TExpected(mpl::c_str<TGiven>::value);
    }

    template<typename TExpected, typename TGiven, typename... TArgs>
    TExpected* get(TArgs&&... args) const {
        return new TGiven(std::forward<TArgs>(args)...);
    }
};

} // namespace core
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace core {

BOOST_DI_HAS_MEMBER_FUNCTION(call, call);

template<typename TDeps = type_list<>>
class module : public pool<TDeps> {
    template<typename> friend class module;
    using pool_t = pool<TDeps>;

    class empty_visitor {
    public:
        template<typename T>
        void operator()(const T&) const { }
    };

public:
    using deps = TDeps;

    // bind<...>, etc.   -> ignore
    // module<....>      -> get all dependencies from the module
    // dependency<....>  -> pass

    template<typename... TArgs>
    explicit module(const TArgs&... args)
        : pool_t(init(), pool<type_list<TArgs...>>(args...))
    { }

    template<typename T, typename... TPolicies>
    T create(const TPolicies&... policies) {
        using call_stack = type_list<>;
        pool<type_list<TPolicies...>> policies_(policies...);
        std::vector<aux::shared_ptr<void>> refs_;

        return creator_.template create<T, none_t, call_stack>(
            provider()
          , static_cast<pool_t&>(*this)
          , refs_
          , empty_visitor()
          , policies_
        );
    }

    template<typename T, typename Tprovider, typename... TPolicies>
    T allocate(const Tprovider& provider, const TPolicies&... policies) {
        using call_stack = type_list<>;
        pool<type_list<TPolicies...>> policies_(policies...);
        std::vector<aux::shared_ptr<void>> refs_;

        return creator_.template create<T, none_t, call_stack>(
            provider
          , static_cast<pool_t&>(*this)
          , refs_
          , empty_visitor()
          , policies_
        );
    }

    template<typename T, typename TVisitor>
    T visit(const TVisitor& visitor) {
        using call_stack = type_list<>;
        std::vector<aux::shared_ptr<void>> refs_;

        return creator_.template create<T, none_t, call_stack>(
            provider()
          , static_cast<pool_t&>(*this)
          , refs_
          , visitor
          , pool<>()
        );
    }

    template<typename TAction>
    void call(const TAction& action) {
        call_impl(static_cast<pool<deps>&>(*this), action, deps());
    }

private:
    template<typename TDeps_, typename TAction, typename... Ts>
    void call_impl(TDeps_& deps, const TAction& action, const type_list<Ts...>&) {
        int dummy[]{0, (call_impl<Ts>(deps, action), 0)...};
        (void)dummy;
    }

    template<typename T, typename TDeps_, typename TAction>
    typename std::enable_if<has_call<T>::value>::type
    call_impl(TDeps_& deps, const TAction& action) {
        static_cast<T&>(deps).call(action);
    }

    template<typename T, typename TDeps_, typename TAction>
    typename std::enable_if<!has_call<T>::value>::type
    call_impl(TDeps_& deps, const TAction& action) { }

    creator<TDeps> creator_;
};

} // namespace core
} // namespace di
} // namespace boost

namespace boost {
namespace di {

BOOST_DI_HAS_MEMBER_TYPE(deps);
BOOST_DI_HAS_MEMBER_FUNCTION(configure, configure);

template<typename T>
struct is_module
    : bool_<has_deps<T>::value || has_configure<T>::value>
{ };

template<typename T, typename = void>
struct get_module {
    using type = T;
};

template<typename T>
struct get_module<T, typename std::enable_if<has_configure<T>::value>::type> {
    using type = typename type_traits::function_traits<
        decltype(&T::configure)
    >::result_type;
};

template<typename T, typename = void>
struct get_deps {
    using type = typename T::deps;
};

template<typename T>
struct get_deps<T, typename std::enable_if<has_configure<T>::value>::type> {
    using type = typename get_module<T>::type::deps;
};

template<typename T, typename = typename is_type_list<T>::type, typename = typename is_module<T>::type>
struct add_type_list;

template<typename T, typename Any>
struct add_type_list<T, std::true_type, Any> {
    using type = T;
};

template<typename T>
struct add_type_list<T, std::false_type, std::true_type> {
    using type = typename get_deps<T>::type;
};

template<typename T>
struct add_type_list<T, std::false_type, std::false_type> {
    using type = scope<scopes::deduce>::bind<T>;
};

template<typename... Ts>
using get_bindings = typename join<typename add_type_list<Ts>::type...>::type;

template<typename... Ts>
class injector
    : public core::module<typename get_bindings<Ts...>::type>
{
public:
    template<typename... TArgs>
    explicit injector(const TArgs&... args)
        : core::module<typename get_bindings<Ts...>::type>(pass_arg(args)...)
    { }

private:
    template<typename T>
    typename std::enable_if<!has_configure<T>::value, const T&>::type
    pass_arg(const T& arg) const {
        return arg;
    }

    template<typename T>
    typename std::enable_if<has_configure<T>::value, typename get_module<T>::type>::type
    pass_arg(const T& arg) const {
        return arg.configure();
    }
};

} // namespace di
} // namespace boost

namespace boost {
namespace di {

template<typename... TArgs>
injector<typename get_bindings<TArgs...>::type>
inline make_injector(const TArgs&... args) {
    return injector<typename get_bindings<TArgs...>::type>(args...);
}

} // namespace di
} // namespace boost

#endif

#endif

