//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Preprocessed version of "boost::di"
// DO NOT modify by hand!
//
#ifndef BOOST_DI_PREPROCESSED_DI_CPP_11_C_HPP
#define BOOST_DI_PREPROCESSED_DI_CPP_11_C_HPP

#include <vector>
#include <utility>
#include <typeinfo>
#include <string>
#include <map>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_copy_constructible.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type.hpp>
#include <boost/ref.hpp>
#include <boost/non_type.hpp>
#include <boost/none_t.hpp>
#include "boost/di/inject.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/config.hpp"
#include <boost/config.hpp>

namespace boost {
namespace di {
namespace type_traits {

template<typename T>
struct remove_accessors
    : remove_cv<
         typename remove_pointer<
             typename remove_reference<T>::type
         >::type
      >
{ };

} // namespace type_traits
} // namespace di
} // namespace boost


namespace boost {
namespace di {
namespace type_traits {

template<typename>
struct make_plain;

BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)
BOOST_MPL_HAS_XXX_TRAIT_DEF(named_type)

template<typename T, typename = void>
struct deref_type
{
    typedef T type;
};

template<typename T>
struct deref_type<T, typename enable_if<has_element_type<T> >::type>
{
    typedef typename T::element_type type;
};

template<typename T>
struct deref_type<T, typename enable_if<has_named_type<T> >::type>
{
    typedef typename make_plain<typename T::named_type>::type type;
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
class named
{
    named& operator=(const named&);

public:
    typedef T named_type;
    typedef TName name;

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
class named<const T&, TName>
{
    named& operator=(const named&);

public:
    typedef const T& named_type;
    typedef TName name;

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
class named<T&, TName>
{
    named& operator=(const named&);

public:
    typedef T& named_type;
    typedef TName name;

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

BOOST_DI_FEATURE(RVALUE_REFERENCES)(
    template<
        typename T
      , typename TName
    >
    class named<T&&, TName>
    {
        named& operator=(const named&);

    public:
        typedef T&& named_type;
        typedef TName name;

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
)

template<
    typename T
  , typename TName
>
class named<aux::unique_ptr<T>, TName>
{
    named& operator=(const named&);

public:
    typedef aux::unique_ptr<T> named_type;
    typedef TName name;

    BOOST_DI_FEATURE(RVALUE_REFERENCES)(
        named(aux::unique_ptr<T> object) // non explicit
            : object_(std::move(object))
        { }

        operator aux::unique_ptr<T>() {
            return std::move(object_);
        }
    )

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
  , typename enable_if<
        is_polymorphic<typename type_traits::remove_accessors<T>::type>
    >::type
>
{
public:
    typedef T named_type;
    typedef TName name;
};

} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

template<typename T>
class copy
{
    typedef function<T*()> value_t;

    template<typename I>
    class scoped_ptr
    {
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
    I operator()(const type<I>&, typename disable_if<is_polymorphic<I> >::type* = 0) const {
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
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I> >&) const {
        return aux::shared_ptr<I>(value_());
    }

    template<typename I>
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I> >&) const {
        return aux_::shared_ptr<I>(value_());
    }

    template<typename I>
    aux::auto_ptr<I> operator()(const type<aux::auto_ptr<I> >&) const {
        return aux::auto_ptr<I>(value_());
    }

    template<typename I>
    aux::unique_ptr<I> operator()(const type<aux::unique_ptr<I> >&) const {
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
class unique
{
public:
    typedef aux::mpl::int_<0> priority;

    template<typename TExpected>
    class scope
    {
    public:
        typedef TWrapper<TExpected> result_type;

        result_type create(const function<TExpected*()>& f) {
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
class shared
{
    template<typename U, typename TShared = aux::shared_ptr<U> >
    class sp_holder
    {
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
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I> >&) const {
        return value_;
    }

    template<typename I>
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I> >&) const {
        aux_::shared_ptr<sp_holder<T> > sp(new sp_holder<T>(value_));
        return aux_::shared_ptr<T>(sp, value_.get());
    }

    template<typename I>
    aux::weak_ptr<I> operator()(const type<aux::weak_ptr<I> >&) const {
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
class shared
{
public:
    typedef aux::mpl::int_<0> priority;

    template<typename TExpected>
    class scope
    {
    public:
        typedef TWrapper<TExpected> result_type;

        result_type create(const function<TExpected*()>& f) {
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
class value
{
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

    BOOST_DI_FEATURE(RVALUE_REFERENCES)(
        T&& operator()(const type<T&&>&) {
            return std::move(value_);
        }
    )

    template<typename I>
    aux::shared_ptr<I> operator()(const type<aux::shared_ptr<I> >&) const {
        return aux::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    aux_::shared_ptr<I> operator()(const type<aux_::shared_ptr<I> >&) const {
        return aux_::shared_ptr<I>(new I(value_));
    }

    template<typename I>
    aux::auto_ptr<I> operator()(const type<aux::auto_ptr<I> >&) const {
        return aux::auto_ptr<I>(new I(value_));
    }

    template<typename I>
    aux::unique_ptr<I> operator()(const type<aux::unique_ptr<I> >&) const {
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
namespace type_traits {

template<typename T>
class has_call_operator
{
    struct base_impl { void operator()(...) { } };
    struct base
        : base_impl
        , aux::mpl::if_<is_class<T>, T, aux::mpl::void_>::type
    { base() { } };

    template<typename U>
    static aux::mpl::aux::no_tag test(
        U*
      , non_type<void (base_impl::*)(...), &U::operator()>* = 0
    );

    static aux::mpl::aux::yes_tag test(...);

public:
    typedef has_call_operator type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test((base*)0)) == sizeof(aux::mpl::aux::yes_tag)
    );
};

} // namespace type_traits
} // namespace di
} // namespace boost

BOOST_DI_FEATURE(CPP_11_FUNCTIONAL)(
    namespace boost {

    template<typename T>
    class is_reference_wrapper< ::std::reference_wrapper<T> >
        : public ::boost::di::aux::mpl::true_
    { };

    template<typename T>
    class unwrap_reference< ::std::reference_wrapper<T> >
    {
    public:
        typedef T type;
    };

    } // namespace
)

namespace boost {
namespace di {
namespace scopes {

template<template<typename> class TWrapper = wrappers::value>
class external
{
public:
    typedef aux::mpl::int_<1> priority;

    template<typename TExpected>
    class scope
    {
    public:
        typedef TWrapper<TExpected> result_type;

    private:
        class result_type_holder
        {
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
        typename disable_if<type_traits::has_call_operator<TValueType>, function<result_type()> >::type
        convert_when_function(const T& object) {
            return object;
        }

        template<typename TValueType, typename T>
        typename enable_if<type_traits::has_call_operator<TValueType>, result_type_holder>::type
        convert_when_function(const T& object) {
            return result_type_holder(object);
        }

    public:
        template<typename T>
        explicit scope(const T& object
                     , typename enable_if_c<
                           type_traits::has_call_operator<T>::value &&
                           !is_reference_wrapper<T>::value
                       >::type* = 0)
            : object_(convert_when_function<TExpected>(object))
        { }

        template<typename T>
        explicit scope(const T& object
                     , typename disable_if_c<
                           type_traits::has_call_operator<T>::value &&
                           !is_reference_wrapper<T>::value
                       >::type* = 0)
            : object_(result_type_holder(object))
        { }

        result_type create() {
            return object_();
        }

    private:
        function<result_type()> object_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost


namespace boost {
namespace di {
namespace type_traits {

template<typename T>
struct scope_traits
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<T&>
{
    typedef scopes::external<> type;
};

template<typename T>
struct scope_traits<const T&>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<T*>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<const T*>
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<aux::auto_ptr<T> >
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<aux::shared_ptr<T> >
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<const aux::shared_ptr<T>&>
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<aux_::shared_ptr<T> >
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<const aux_::shared_ptr<T>&>
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<aux::weak_ptr<T> >
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<const aux::weak_ptr<T>&>
{
    typedef scopes::shared<> type;
};

template<typename T>
struct scope_traits<aux::unique_ptr<T> >
{
    typedef scopes::unique<> type;
};

template<typename T>
struct scope_traits<const aux::unique_ptr<T>&>
{
    typedef scopes::unique<> type;
};

BOOST_DI_FEATURE(RVALUE_REFERENCES)(
    template<typename T>
    struct scope_traits<T&&>
    {
        typedef scopes::unique<> type;
    };

    template<typename T>
    struct scope_traits<const T&&>
    {
        typedef scopes::unique<> type;
    };
)

template<typename T, typename TName>
struct scope_traits<named<T, TName> >
{
    typedef typename scope_traits<T>::type type;
};

template<typename T, typename TName>
struct scope_traits<const named<T, TName>&>
{
    typedef typename scope_traits<T>::type type;
};

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace scopes {

class deduce
{
public:
    typedef aux::mpl::int_<0> priority;

    template<typename>
    struct scope
    {
        typedef void result_type;
    };

    template<typename T>
    struct rebind
    {
        typedef typename type_traits::scope_traits<T>::type other;
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
class session
{
public:
    typedef aux::mpl::int_<0> priority;

    template<typename TExpected>
    class scope
    {
    public:
        typedef TWrapper<TExpected> result_type;

        scope()
            : in_scope_(false)
        { }

        void call(const session_entry&) {
            in_scope_ = true;
        }

        void call(const session_exit&) {
            in_scope_ = false;
            object_.reset();
        }

        result_type create(const function<TExpected*()>& f) {
            if (in_scope_ && !object_) {
                object_.reset(f());
            }
            return object_;
        }

    private:
        result_type object_;
        bool in_scope_;
    };
};

} // namespace scopes
} // namespace di
} // namespace boost


namespace boost {
namespace di {
namespace bindings {
namespace detail {

template<typename... T_MPL>
class requires_
{
    template<
        typename TBind
      , typename T
    >
    struct apply_bind
        : TBind::template apply<T>::type
    { };

public:
    typedef requires_ type;

    template<
        typename T
      , typename TMultiplicationFactor = aux::mpl::integral_c<long, 10>
    >
    struct apply
        : aux::mpl::second<
              typename aux::mpl::fold<
                  ::boost::di::aux::mpl::vector<T_MPL...>
                , aux::mpl::pair<aux::mpl::integral_c<long, 1>, aux::mpl::integral_c<long, 1> >
                , aux::mpl::pair<
                      aux::mpl::times<
                          aux::mpl::first<aux::mpl::_1>
                        , TMultiplicationFactor
                      >
                    , aux::mpl::times<
                          aux::mpl::first<aux::mpl::_1>
                        , aux::mpl::second<aux::mpl::_1>
                        , apply_bind<aux::mpl::_2, T>
                      >
                  >
              >::type
          >
    { };
};

} // namespace detail
} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {
namespace detail {

template<typename TContext>
class when_
{
    template<
        typename TBind
      , typename T
    >
    struct apply_bind
        : TBind::template apply<T>::type
    { };

public:
    template<typename T>
    struct apply
        : aux::mpl::if_<
              aux::mpl::empty<TContext>
            , aux::mpl::int_<1>
            , typename aux::mpl::deref<
                  aux::mpl::max_element<
                      aux::mpl::transform_view<
                          TContext
                        , apply_bind<aux::mpl::_1, T>
                      >
                  >
              >::type
          >::type
    { };
};

} // namespace detail
} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

BOOST_MPL_HAS_XXX_TRAIT_DEF(name)

template<typename TName>
class is_required_name
{
    template<typename T, typename = void>
    struct get_name
    {
        struct no_name { };
        typedef no_name type;
    };

    template<typename T>
    struct get_name<T, typename enable_if<
        has_name<typename di::type_traits::remove_accessors<T>::type> >::type
    >
    {
        typedef typename di::type_traits::remove_accessors<T>::type::name type;
    };

public:
    template<typename T>
    struct apply
        : aux::mpl::integral_c<
              long
            , is_same<typename get_name<typename T::type>::type, TName>::value
          >
    { };
};

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

struct is_required_priority
{
    template<typename T>
    struct apply
        : aux::mpl::plus<
              aux::mpl::int_<1>
            , typename T::dependency::scope::priority // lowest = 0, highest = N
          >
    { };
};

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace type_traits {

template<typename T, typename U = aux::mpl::_1>
struct is_same_base_of
    : aux::mpl::or_<
          is_base_of<U, T>
        , is_same<U, T>
      >
{ };

} // namespace type_traits
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace bindings {
namespace type_traits {

template<typename TValueType, typename = void>
struct is_required_type
{
    template<typename T>
    struct apply
        : di::type_traits::is_same_base_of<
              TValueType
            , typename di::type_traits::make_plain<typename T::type>::type
          >
    { };
};

template<typename TValueType>
struct is_required_type<TValueType, typename enable_if<aux::mpl::is_sequence<TValueType> >::type>
{
    template<typename T>
    struct apply
        : aux::mpl::count_if<
              TValueType
            , di::type_traits::is_same_base_of<
                  typename di::type_traits::make_plain<typename T::type>::type
                , aux::mpl::_
              >
          >
    { };
};

} // namespace type_traits
} // namespace bindings
} // namespace di
} // namespace boost


namespace boost {
namespace di {
namespace bindings {

namespace detail {

template<typename TExpected, typename TGiven>
struct get_expected
    : aux::mpl::if_<
          aux::mpl::is_sequence<TExpected>
        , TGiven
        , TExpected
      >
{ };

} // namespace detail

template<
    typename TExpected
  , typename TGiven
  , template<
        typename
      , typename
      , typename
      , typename
    > class TDependency
>
struct bind
    : TDependency<
          scopes::deduce
        , typename detail::get_expected<TExpected, TGiven>::type
        , TGiven
        , detail::requires_<
              type_traits::is_required_priority
            , type_traits::is_required_type<TExpected>
          >
      >
{
    template<typename... T_MPL>
    struct when
        : TDependency<
              scopes::deduce
            , typename detail::get_expected<TExpected, TGiven>::type
            , TGiven
            , detail::requires_<
                  type_traits::is_required_priority
                , type_traits::is_required_type<TExpected>
                , detail::when_< ::boost::di::aux::mpl::vector<T_MPL...> >
              >
          >
    {
        template<typename TName>
        struct named
            : TDependency<
                  scopes::deduce
                , typename detail::get_expected<TExpected, TGiven>::type
                , TGiven
                , detail::requires_<
                      type_traits::is_required_priority
                    , type_traits::is_required_type<TExpected>
                    , type_traits::is_required_name<TName>
                    , detail::when_< ::boost::di::aux::mpl::vector<T_MPL...> >
                  >
              >
        { };
    };

    template<typename TName>
    struct named
        : TDependency<
              scopes::deduce
            , typename detail::get_expected<TExpected, TGiven>::type
            , TGiven
            , detail::requires_<
                  type_traits::is_required_priority
                , type_traits::is_required_type<TExpected>
                , type_traits::is_required_name<TName>
              >
          >
    {
        template<typename... T_MPL>
        struct when
            : TDependency<
                  scopes::deduce
                , typename detail::get_expected<TExpected, TGiven>::type
                , TGiven
                , detail::requires_<
                      type_traits::is_required_priority
                    , type_traits::is_required_type<TExpected>
                    , type_traits::is_required_name<TName>
                    , detail::when_< ::boost::di::aux::mpl::vector<T_MPL...> >
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

template<typename... T_MPL>
class call_stack
{
    typedef ::boost::di::aux::mpl::vector<T_MPL...> context_type;

    template<typename TContext, typename TCallStack>
    struct equal
        : aux::mpl::equal<
              aux::mpl::iterator_range<
                  typename aux::mpl::advance<
                      typename aux::mpl::begin<TCallStack>::type
                    , typename aux::mpl::max<
                          aux::mpl::int_<0>
                        , aux::mpl::minus<
                              aux::mpl::size<TCallStack>
                            , aux::mpl::size<TContext>
                          >
                      >::type
                  >::type
                , typename aux::mpl::end<TCallStack>::type
              >
            , TContext
          >
    { };

    template<typename TContext, typename TCallStack>
    struct apply_impl
        : aux::mpl::if_<
              aux::mpl::empty<TCallStack>
            , aux::mpl::int_<0>
            , aux::mpl::if_<
                  equal<TContext, TCallStack>
                , aux::mpl::size<TContext>
                , aux::mpl::int_<0>
              >
          >
    { };

public:
    template<typename T>
    struct apply
        : apply_impl<
              context_type
            , typename aux::mpl::transform<
                  typename T::call_stack
                , di::type_traits::make_plain<aux::mpl::_>
              >::type
          >::type
    { };
};

} // namespace bindings
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace wrappers {

template<typename T>
class reference
{
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

    template<typename R>
    struct function_traits<R(*)()>
    {
        typedef R result_type;
        typedef aux::mpl::vector<> type;
    };

    template<typename R, typename T>
    struct function_traits<R(T::*)()>
    {
        typedef R result_type;
        typedef T base_type;
        typedef aux::mpl::vector<> type;
    };

    template<typename R, typename T>
    struct function_traits<R(T::*)() const>
    {
        typedef R result_type;
        typedef T base_type;
        typedef aux::mpl::vector<> type;
    };

    template<typename R, typename... Args>
    struct function_traits<R(*)(Args...)>
    {
        typedef R result_type;
        typedef aux::mpl::vector<Args...> type;
    };

    template<typename R, typename T, typename... Args>
    struct function_traits<R(T::*)(Args...)>
    {
        typedef R result_type;
        typedef T base_type;
        typedef aux::mpl::vector<Args...> type;
    };

    template<typename R, typename T, typename... Args>
    struct function_traits<R(T::*)(Args...) const>
    {
        typedef R result_type;
        typedef T base_type;
        typedef aux::mpl::vector<Args...> type;
    };

    } // namespace type_traits
    } // namespace di
    } // namespace boost

namespace boost {
namespace di {
namespace bindings {

BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)

template<
    typename TScope
  , typename TExpected
  , typename TGiven = TExpected
  , typename TBind =
        detail::requires_<
            bindings::type_traits::is_required_priority
          , bindings::type_traits::is_required_type<TExpected>
        >
>
class dependency : public TScope::template scope<TExpected>
{
    typedef typename TScope::template scope<TExpected> scope_type;
    typedef scopes::external<wrappers::reference> ref_type;
    typedef scopes::external<wrappers::shared> shared_type;
    typedef scopes::external<wrappers::value> value_type;

    template<typename, typename = void>
    struct get_wrapper_impl
    {
        typedef value_type type;
    };

    template<typename T>
    struct get_wrapper_impl<aux::shared_ptr<T> >
    {
        typedef shared_type type;
    };

    template<typename T>
    struct get_wrapper_impl<T, typename enable_if<is_reference_wrapper<T> >::type>
    {
        typedef ref_type type;
    };

    template<
        typename T
      , typename = void
      , typename = void
      , typename = void
    >
    struct get_wrapper
    {
        typedef T type;
    };

    template<typename T>
    struct get_wrapper<T, typename enable_if<has_result_type<T> >::type>
        : get_wrapper_impl<typename T::result_type>
    { };

    template<typename T>
    struct get_wrapper<T, typename enable_if<di::type_traits::has_call_operator<T> >::type
                        , typename disable_if<has_result_type<T> >::type
                        , typename disable_if<is_reference_wrapper<T> >::type>
        : get_wrapper_impl<
              typename di::type_traits::function_traits<
                  BOOST_DI_FEATURE_DECLTYPE(&T::operator())
              >::result_type
          >
    { };

public:
    typedef dependency type;
    typedef TScope scope;
    typedef TExpected expected;
    typedef TGiven given;
    typedef TBind bind;

    template<typename T>
    struct rebind
    {
        typedef dependency<
            typename aux::mpl::if_<
                is_same<scope, scopes::deduce>
              , T
              , TScope
            >::type
          , TExpected
          , TGiven
          , TBind
        > other;
    };

    dependency() { }

    template<typename T>
    explicit dependency(const T& object)
        : scope_type(object)
    { }

    template<typename T>
    static dependency<value_type, expected, T, TBind>
    to(const T& object, typename disable_if<is_reference_wrapper<T> >::type* = 0
                      , typename disable_if<di::type_traits::has_call_operator<T> >::type* = 0) {
        return dependency<value_type, expected, T, TBind>(object);
    }

    template<typename T>
    static dependency<ref_type, typename unwrap_reference<T>::type, T, TBind>
    to(const T& object, typename enable_if<is_reference_wrapper<T> >::type* = 0) {
        return dependency<ref_type, typename unwrap_reference<T>::type, T, TBind>(object);
    }

    template<typename T>
    static dependency<typename get_wrapper<T>::type, expected, T, TBind>
    to(const T& object, typename disable_if<is_reference_wrapper<T> >::type* = 0
                      , typename enable_if<di::type_traits::has_call_operator<T> >::type* = 0) {
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

template<
    typename TScope
  , template<
        typename
      , typename
      , typename
      , typename
    > class TDependency
>
class scope
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(given)

    template<typename T>
    struct is_dependency
        : has_given<T>
    { };

    template<typename T>
    struct dependency
        : TDependency<
              TScope
            , T
            , T
            , detail::requires_<
                  type_traits::is_required_priority
                , type_traits::is_required_type<T>
              >
          >
    { };

    template<typename T, typename U>
    struct rebind
        : T::template rebind<U>::other
    { };

public:
    template<typename... T_MPL>
    struct bind
        : aux::mpl::fold<
              ::boost::di::aux::mpl::vector<T_MPL...>
            , aux::mpl::vector<>
            , aux::mpl::push_back<
                  aux::mpl::_1
                , aux::mpl::if_<
                      is_dependency<aux::mpl::_2>
                    , rebind<aux::mpl::_2, TScope>
                    , dependency<aux::mpl::_2>
                  >
              >
          >::type
    { };
};

} // namespace bindings
} // namespace di
} // namespace boost


namespace boost {
namespace di {

template<typename TExpected, typename TGiven = TExpected>
struct bind
    : bindings::bind<TExpected, TGiven, bindings::dependency>
{ };

template<bool V>
struct bind_bool
    : bind<bool, aux::mpl::bool_<V> >
{ };

template<int N>
struct bind_int
    : bind<int, aux::mpl::int_<N> >
{ };

template<typename T>
struct bind_string
    : bind<std::string, T>
{ };

template<typename TScope>
struct scope
    : bindings::scope<TScope, bindings::dependency>
{ };

template<typename... T_MPL>
struct deduce
    : scope<scopes::deduce>::bind<T_MPL...>
{ };

template<typename... T_MPL>
struct unique
    : scope<scopes::unique<> >::bind<T_MPL...>
{ };

template<typename... T_MPL>
struct shared
    : scope<scopes::shared<> >::bind<T_MPL...>
{ };

template<typename... T_MPL>
struct session
    : scope<scopes::session<> >::bind<T_MPL...>
{ };

template<typename... T_MPL>
struct call_stack
    : bindings::call_stack<T_MPL...>
{ };

template<typename... T_MPL>
struct any_of
    : ::boost::di::aux::mpl::vector<T_MPL...>
{ };

} // namespace di
} // namespace boost


    namespace boost {
    namespace di {
    namespace core {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(types)

    class init { };

    template<typename>
    struct never
        : aux::mpl::false_
    { };

    template<
        typename = aux::mpl::vector<>
      , typename = never<aux::mpl::_1>
      , typename = void
    >
    class pool;

    template<typename TIgnore, typename... TArgs>
    class pool<aux::mpl::vector<TArgs...>, TIgnore>
        : public TArgs...
    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef aux::mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename aux::mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type; // ignore type
        };

    public:
        typedef pool type;

        struct types
            : aux::mpl::fold<
                  aux::mpl::vector<TArgs...>
                , aux::mpl::vector<>
                , aux::mpl::copy<
                      pool_type<aux::mpl::_2>
                    , aux::mpl::back_inserter<aux::mpl::_1>
                  >
              >::type
        { };

        template<typename... T>
        explicit pool(const T&... args)
            : T(args)...
        { }

        template<typename TPool>
        pool(const TPool& p, const init&)
            : pool(p, typename aux::mpl::normalize_vector<typename TPool::types>::type(), init())
        { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }

    private:
        template<typename TPool, typename... T>
        pool(const TPool& p, const aux::mpl::vector<T...>&, const init&)
            : T(p.template get<T>())...
        { }
    };

    } // namespace core
    } // namespace di
    } // namespace boost


namespace boost {
namespace di {
namespace type_traits {

namespace detail {

template<
    typename T
  , typename TSignature
>
class is_convertible
{
    template<typename U>
    static aux::mpl::aux::yes_tag test(non_type<TSignature, &U::operator()>*);

    template<typename>
    static aux::mpl::aux::no_tag test(...);

public:
    typedef is_convertible type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test<T>(0)) == sizeof(aux::mpl::aux::yes_tag)
    );
};

} // namespace detail

template<
    typename TValueType
  , typename T
>
struct is_convertible_to_ref
    : detail::is_convertible<TValueType, const T&(TValueType::*)(const boost::type<const T&>&) const>
{ };

} // namespace type_traits
} // namespace di
} // namespace boost


namespace boost {
namespace di {
namespace wrappers {

namespace detail {

template<typename TResult, typename T, typename TValueType>
inline typename disable_if<is_copy_constructible<T>, const TResult&>::type
copy(std::vector<aux::shared_ptr<void> >& refs, const TValueType& value) {
    aux::shared_ptr<TResult> object(value(boost::type<T*>()));
    refs.push_back(object);
    return *object;
}

template<typename T>
struct holder
{
    explicit holder(const T& value)
        : held(value)
    { }

    T held;
};

template<typename TResult, typename T, typename TValueType>
inline typename enable_if<is_copy_constructible<T>, const TResult&>::type
copy(std::vector<aux::shared_ptr<void> >& refs, const TValueType& value) {
    aux::shared_ptr<holder<TResult> > object(new holder<TResult>(value(boost::type<T>())));
    refs.push_back(object);
    return object->held;
}

template<typename T>
class universal_impl
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >&, const TValueType& value)
        : value_(boost::bind<T>(value, boost::type<T>()))
    { }

    operator T() const {
        return value_();
    }

private:
    function<T()> value_;
};

template<typename T>
class universal_impl<aux::auto_ptr<T> >
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs, const TValueType& value)
        : value_(new aux::auto_ptr<T>(value(boost::type<aux::auto_ptr<T> >()).release()))
    {
        refs.push_back(aux::shared_ptr<aux::auto_ptr<T> >(value_));
    }

    operator aux::auto_ptr<T>&() {
        return *value_;
    }

private:
    aux::auto_ptr<T>* value_; // weak
};

template<typename T>
class universal_impl<const T&>
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >&
                 , const TValueType& value
                 , typename enable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : value_(boost::bind<const T&>(value, boost::type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TValueType& value
                 , typename disable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : value_(boost::bind(&copy<T, T, TValueType>, boost::ref(refs), value))
    { }

    operator const T&() const {
        return value_();
    }

private:
    function<const T&()> value_;
};

template<typename T, typename TName>
class universal_impl<named<T, TName> >
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >&, const TValueType& value)
        : value_(boost::bind<T>(value, boost::type<T>()))
    { }

    operator T() const {
        return value_();
    }

    operator named<T, TName>() const {
        return value_();
    }

private:
    function<T()> value_;
};

template<typename T, typename TName>
class universal_impl<named<const T&, TName> >
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >&
                 , const TValueType& value
                 , typename enable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : value_(boost::bind<const T&>(value, boost::type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TValueType& value
                 , typename disable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : value_(boost::bind(&copy<T, T, TValueType>, boost::ref(refs), value))
    { }

    operator named<const T&, TName>() const {
        return value_();
    }

private:
    function<named<const T&, TName>()> value_;
};

template<typename T, typename TName>
class universal_impl<const named<T, TName>&>
{
public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TValueType& value)
        : value_(boost::bind(&copy<named<T, TName>, T, TValueType>, boost::ref(refs), value))
    { }

    operator const named<T, TName>&() const {
        return value_();
    }

private:
    function<const named<T, TName>&()> value_;
};

template<typename T, typename TName>
class universal_impl<const named<const T&, TName>&>
{
    universal_impl& operator=(const universal_impl&);

public:
    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TValueType& value
                 , typename enable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : refs_(refs)
        , value_(boost::bind<const T&>(value, boost::type<const T&>()))
    { }

    template<typename TValueType>
    universal_impl(std::vector<aux::shared_ptr<void> >& refs
                 , const TValueType& value
                 , typename disable_if<type_traits::is_convertible_to_ref<TValueType, T> >::type* = 0)
        : refs_(refs)
        , value_(boost::bind(&copy<T, T, TValueType>, boost::ref(refs), value))
    { }

    operator const named<const T&, TName>&() const {
        aux::shared_ptr<holder<named<const T&, TName> > > object(
            new holder<named<const T&, TName> >(value_())
        );
        refs_.push_back(object);
        return object->held;
    }

private:
    std::vector<aux::shared_ptr<void> >& refs_;
    function<named<const T&, TName>()> value_;
};

} // namespace detail

template<typename T>
class universal : public detail::universal_impl<T>
{
public:
    typedef universal type;
    typedef T element_type;

    template<typename TValueType>
    universal(std::vector<aux::shared_ptr<void> >& refs, const TValueType& value)
        : detail::universal_impl<T>(refs, value)
    { }
};

} // namespace wrappers
} // namespace di
} // namespace boost

namespace boost {
namespace di {
namespace core {

template<typename TDependecies>
class binder
{
    template<
        typename T
      , typename TCallStack
      , typename TDependency
    >
    struct data
    {
        typedef T type;
        typedef TCallStack call_stack;
        typedef TDependency dependency;
    };

    template<
        typename TDependency
      , typename T
      , typename TCallStack
    >
    struct apply
        : TDependency::bind::template apply<
              data<T, TCallStack, TDependency>
          >::type
    { };

public:
    template<
        typename T
      , typename TCallStack
      , typename TDefault =
            bindings::dependency<
                scopes::deduce
              , typename type_traits::make_plain<T>::type
            >
    >
    struct resolve
        : aux::mpl::deref<
              aux::mpl::second<
                  typename aux::mpl::fold<
                      TDependecies
                    , aux::mpl::pair<aux::mpl::int_<0>, TDefault>
                    , aux::mpl::if_<
                          aux::mpl::greater<
                              apply<
                                  aux::mpl::_2
                                , T
                                , TCallStack
                              >
                            , aux::mpl::first<aux::mpl::_1>
                          >
                        , aux::mpl::pair<apply<aux::mpl::_2, T, TCallStack>, aux::mpl::_2>
                        , aux::mpl::_1
                      >
                  >::type
              >
          >::type::template
              rebind<typename scopes::deduce::rebind<T>::other>::other
    { };
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
  , typename TAllocator = none_t
  , typename TDeps = none_t
  , typename TRefs = none_t
  , typename TVisitor = none_t
  , typename TPolicies = none_t
>
class any_type
{
    any_type& operator=(const any_type&);

    template<typename TValueType, typename TRefType>
    struct ref_type
        : aux::mpl::if_<
              is_same<TValueType, none_t>
            , TValueType
            , TRefType
          >
    { };

public:
    typedef void any;
    any_type() { }

    any_type(TCreator& creator
           , const TAllocator& allocator
           , TDeps& deps
           , TRefs& refs
           , const TVisitor& visitor
           , const TPolicies& policies)
        : creator_(creator)
        , allocator_(allocator)
        , deps_(deps)
        , refs_(refs)
        , visitor_(visitor)
        , policies_(policies)
    { }

    any_type(const any_type& other)
        : creator_(other.creator_)
        , allocator_(other.allocator_)
        , deps_(other.deps_)
        , refs_(other.refs_)
        , visitor_(other.visitor_)
        , policies_(other.policies_)
    { }

    template<
        typename U
        BOOST_DI_FEATURE(FUNCTION_TEMPLATE_DEFAULT_ARGS)(
            , typename = typename disable_if<
                type_traits::is_same_base_of<
                    typename type_traits::make_plain<U>::type
                  , typename type_traits::make_plain<T>::type
                >
            >::type
        )
    >
    operator const U&() const {
        return creator_.template create<const U&, none_t, TCallStack>(
            allocator_, deps_, refs_, visitor_, policies_
        );
    }

    template<
        typename U
        BOOST_DI_FEATURE(FUNCTION_TEMPLATE_DEFAULT_ARGS)(
            , typename = typename disable_if<
                type_traits::is_same_base_of<
                    typename type_traits::make_plain<U>::type
                  , typename type_traits::make_plain<T>::type
                >
            >::type
        )
    >
    operator U&() const {
        return creator_.template create<U&, none_t, TCallStack>(
            allocator_, deps_, refs_, visitor_, policies_
        );
    }

    template<typename U>
    operator aux::auto_ptr<U>&() {
        return creator_.template create<aux::auto_ptr<U>, none_t, TCallStack>(
            allocator_, deps_, refs_, visitor_, policies_
        );
    }

    BOOST_DI_WKND(MSVC)(
        template<typename U>
        operator aux::unique_ptr<U>() {
            return creator_.template create<aux::unique_ptr<U>, none_t, TCallStack>(
                allocator_, deps_, refs_, visitor_, policies_
            );
        }
    )

    BOOST_DI_WKND(NO_MSVC)(
        template<
            typename U
            BOOST_DI_FEATURE(FUNCTION_TEMPLATE_DEFAULT_ARGS)(
                , typename = typename disable_if<
                    type_traits::is_same_base_of<
                        typename type_traits::make_plain<U>::type
                      , typename type_traits::make_plain<T>::type
                    >
                >::type
            )
        >
        operator U() {
            return creator_.template create<U, none_t, TCallStack>(
                allocator_, deps_, refs_, visitor_, policies_
            );
        }
    )

private:
    typename ref_type<TCreator, TCreator&>::type creator_;
    typename ref_type<TAllocator, const TAllocator&>::type allocator_;
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
  , typename TAllocator
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
      , TAllocator
      , TDeps
      , TRefs
      , TVisitor
      , TPolicies
   >
> : ::boost::di::aux::mpl::true_ { };

} // namespace boost

BOOST_DI_FEATURE(CPP_11_TYPE_TRAITS)(
    namespace std {

    template<
        typename T
      , typename TCallStack
      , typename TCreator
      , typename TAllocator
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
          , TAllocator
          , TDeps
          , TRefs
          , TVisitor
          , TPolicies
        >
    > : ::boost::di::aux::mpl::true_ { };

    } // namespace std
)


    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename, typename>
    class has_ctor;

    template<typename T, typename... TArgs>
    class has_ctor<T, aux::mpl::vector<TArgs...> >
    {
    public:
        template<typename U>
        static aux::mpl::aux::yes_tag test(BOOST_DI_FEATURE_DECLTYPE(U(TArgs()...))*);

        template<typename>
        static aux::mpl::aux::no_tag test(...);

    public:
        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test<T>(0)) == sizeof(aux::mpl::aux::yes_tag)
        );
    };

    } // namespace type_traits
    } // namespace di
    } // namespace boost


namespace boost {
namespace di {
namespace type_traits {

template<typename T>
class has_boost_di_injector__
{
    struct base_impl { void boost_di_injector__(...) { } };
    struct base
        : base_impl
        , aux::mpl::if_<is_class<T>, T, aux::mpl::void_>::type
    { base() { } };

    template<typename U>
    static aux::mpl::aux::no_tag test(
        U*
      , non_type<void (base_impl::*)(...), &U::boost_di_injector__>* = 0
    );

    static aux::mpl::aux::yes_tag test(...);

public:
    typedef has_boost_di_injector__ type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test((base*)0)) == sizeof(aux::mpl::aux::yes_tag)
    );
};

} // namespace type_traits
} // namespace di
} // namespace boost


namespace boost {
namespace di {

template<typename T>
struct ctor_traits
    : aux::mpl::second<
          typename aux::mpl::fold<
              aux::mpl::range_c<int, 0, BOOST_DI_CFG_CTOR_LIMIT_SIZE>
            , aux::mpl::pair<
                  aux::mpl::vector<core::any_type<T> >
                , aux::mpl::vector<>
              >
            , aux::mpl::if_<
                  type_traits::has_ctor<
                      T
                    , aux::mpl::normalize_vector<aux::mpl::first<aux::mpl::_1> >
                  >
                , aux::mpl::pair<
                      aux::mpl::push_back<aux::mpl::first<aux::mpl::_1>, core::any_type<T> >
                    , aux::mpl::first<aux::mpl::_1>
                  >
                , aux::mpl::pair<
                      aux::mpl::push_back<aux::mpl::first<aux::mpl::_1>, core::any_type<T> >
                    , aux::mpl::second<aux::mpl::_1>
                  >
              >
          >::type
      >
{ };

template<typename T>
struct ctor_traits<std::basic_string<T> > // basic_string ctors are ambiguous
{
    static void boost_di_injector__();
};

namespace type_traits {

template<typename T, typename = void>
struct ctor_traits
    : di::ctor_traits<T>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<has_boost_di_injector__<di::ctor_traits<T> > >::type>
    : function_traits<BOOST_DI_FEATURE_DECLTYPE(&di::ctor_traits<T>::boost_di_injector__)>::type
{ };

template<typename T>
struct ctor_traits<T, typename enable_if<has_boost_di_injector__<T> >::type>
    : function_traits<BOOST_DI_FEATURE_DECLTYPE(&T::boost_di_injector__)>::type
{ };

} // namespace type_traits
} // namespace di
} // namespace boost


    namespace boost {
    namespace di {
    namespace core {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(any)

    template<
        typename TDependecies = aux::mpl::vector<>
      , template<typename> class TBinder = binder
      , template<
            typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
          , typename = ::boost::none_t
        >
        class TAnyType = ::boost::di::core::any_type
    >
    class creator
    {
        template<typename TDependency>
        struct scope_create
            : type_traits::function_traits<
                  BOOST_DI_FEATURE_DECLTYPE(&TDependency::create)
              >::type
        { };

        class type_comparator
        {
        public:
            bool operator()(const std::type_info* lhs, const std::type_info* rhs) const {
                return lhs->before(*rhs);
            }
        };

        typedef std::map<
            const std::type_info*
          , aux::shared_ptr<void>
          , type_comparator
        > scopes_type;

        template<
            typename T
          , typename TCallStack
          , typename TDependency
        >
        struct data
        {
            typedef T type;
            typedef TCallStack call_stack;
            typedef TDependency dependency;
            typedef TBinder<TDependecies> binder;
        };

    public:
        explicit creator(const TBinder<TDependecies>& binder = TBinder<TDependecies>())
            : binder_(binder)
        { }

        template<
            typename T
          , typename TParent // ignore copy/move ctor
          , typename TCallStack
          , typename TAllocator
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        typename enable_if<
            has_any<T>
          , TAnyType<
                TParent
              , TCallStack
              , creator
              , TAllocator
              , TDeps
              , TRefs
              , TVisitor
              , TPolicies
            >
        >::type
        create(const TAllocator& allocator
             , TDeps& deps
             , TRefs& refs
             , const TVisitor& visitor
             , const TPolicies& policies) {
            return TAnyType<
                TParent
              , TCallStack
              , creator
              , TAllocator
              , TDeps
              , TRefs
              , TVisitor
              , TPolicies
            >(*this, allocator, deps, refs, visitor, policies);
        }

        template<
            typename T
          , typename // TParent - not used
          , typename TCallStack
          , typename TAllocator
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        typename disable_if<has_any<T>, wrappers::universal<T> >::type
        create(const TAllocator& allocator
             , TDeps& deps
             , TRefs& refs
             , const TVisitor& visitor
             , const TPolicies& policies) {
            typedef typename TBinder<TDependecies>::template
                resolve<T, TCallStack>::type dependency_type;

            BOOST_DI_FEATURE_EXAMINE_CALL_STACK(
                typedef typename aux::mpl::push_back<TCallStack, T>::type call_stack_type;
            )

            BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(
                typedef TCallStack call_stack_type;
            )

            typedef data<T, call_stack_type, dependency_type> data_type;
            assert_policies<typename TPolicies::types, data_type>(policies);
            (visitor)(data_type());

            return create_impl<T, dependency_type, call_stack_type>(
                allocator, deps, refs, visitor, policies
            );
        }

    private:

    template<
        typename T
      , typename TDependency
      , typename TCallStack
      , typename TAllocator
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TPolicies
      , typename... TArgs
    >
    typename TDependency::expected*
    create_impl(const TAllocator& allocator
              , TDeps& deps
              , TRefs& refs
              , const TVisitor& visitor
              , const TPolicies& policies
              , const aux::mpl::vector<TArgs...>&) {
        return allocator.template
            allocate<typename TDependency::expected, typename TDependency::given>(
                create<TArgs, T, TCallStack>(allocator, deps, refs, visitor, policies)...
        );
    }

        template<
            typename T
          , typename TDependency
          , typename TCallStack
          , typename TAllocator
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        typename enable_if_c<
            !aux::mpl::size<scope_create<TDependency> >::value
          , wrappers::universal<T>
        >::type
        create_impl(const TAllocator&
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
          , typename TAllocator
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TPolicies
        >
        typename disable_if_c<
            !aux::mpl::size<scope_create<TDependency> >::value
          , wrappers::universal<T> >::type
        create_impl(const TAllocator& allocator
                  , TDeps& deps
                  , TRefs& refs
                  , const TVisitor& visitor
                  , const TPolicies& policies) {
            typedef typename aux::mpl::normalize_vector<
                type_traits::ctor_traits<typename TDependency::given>
            >::type ctor_type;

            return wrappers::universal<T>(
                refs
              , acquire<TDependency>(deps).create(
                    boost::bind(
                        static_cast<
                            typename TDependency::expected*(creator::*)(
                                const TAllocator&
                              , TDeps&
                              , TRefs&
                              , const TVisitor&
                              , const TPolicies&
                              , const ctor_type&
                            )
                        >(&creator::create_impl<T, TDependency, TCallStack>)
                      , this
                      , boost::cref(allocator)
                      , boost::ref(deps)
                      , boost::ref(refs)
                      , boost::cref(visitor)
                      , boost::cref(policies)
                      , ctor_type()
                    )
                )
            );
        }

        template<typename TSeq, typename, typename TPolicies>
        static typename enable_if<aux::mpl::empty<TSeq> >::type
        assert_policies(const TPolicies&) { }

        template<typename TSeq, typename T, typename TPolicies>
        static typename disable_if<aux::mpl::empty<TSeq> >::type
        assert_policies(const TPolicies& policies) {
            typedef typename aux::mpl::front<TSeq>::type policy;
            static_cast<const policy&>(policies).template assert_policy<T>();
            assert_policies<typename aux::mpl::pop_front<TSeq>::type, T>(policies);
        }

        template<typename TDependency, typename TDeps>
        typename enable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
        acquire(TDeps& deps) {
            return static_cast<TDependency&>(deps);
        }

        template<typename TDependency, typename TDeps>
        typename disable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
        acquire(TDeps&) {
            typename scopes_type::const_iterator it = scopes_.find(&typeid(TDependency));
            if (it != scopes_.end()) {
                return *static_cast<TDependency*>(it->second.get());
            }

            aux::shared_ptr<TDependency> dependency(new TDependency());
            scopes_[&typeid(TDependency)] = dependency;
            return *dependency;
        }

        TBinder<TDependecies> binder_;
        scopes_type scopes_;
    };

    } // namespace core
    } // namespace di
    } // namespace boost


namespace boost {
namespace di {
namespace type_traits {

template<typename T>
class has_value
{
    struct base_impl { int value; };
    struct base
        : base_impl
        , aux::mpl::if_<is_class<T>, T, aux::mpl::void_>::type
    { base() { } };

    template<typename U>
    static aux::mpl::aux::no_tag test(
        U*
      , non_type<int base_impl::*, &U::value>* = 0
    );

    static aux::mpl::aux::yes_tag test(...);

public:
    typedef has_value type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test((base*)0)) == sizeof(aux::mpl::aux::yes_tag)
    );
};

} // namespace type_traits
} // namespace di
} // namespace boost


    namespace boost {
    namespace di {
    namespace core {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(tag)

    template<typename, typename = void>
    struct is_mpl_string
        : aux::mpl::false_
    { };

    template<typename T>
    struct is_mpl_string<T, typename enable_if<has_tag<T> >::type>
        : is_same<aux::mpl::string_tag, typename T::tag>
    { };

    template<typename T>
    struct is_explicit
        : aux::mpl::or_<
              type_traits::has_value<T>
            , is_mpl_string<T>
          >
    { };

    class allocator
    {
    public:
        template<typename TExpected, typename TGiven>
        typename disable_if<is_explicit<TGiven>, TExpected*>::type
        allocate() const {
            return new TGiven();
        }

        template<typename TExpected, typename TGiven>
        typename enable_if<type_traits::has_value<TGiven>, TExpected*>::type
        allocate() const {
            return new TExpected(TGiven::value);
        }

        template<typename TExpected, typename TGiven>
        typename enable_if<is_mpl_string<TGiven>, TExpected*>::type
        allocate() const {
            return new TExpected(aux::mpl::c_str<TGiven>::value);
        }

    template<typename TExpected, typename TGiven, typename... TArgs>
    TExpected* allocate(TArgs&&... args) const {
        return new TGiven(::std::forward<decltype(args)>(args)...);
    }
    };

    } // namespace core
    } // namespace di
    } // namespace boost


namespace boost {
namespace di {
namespace type_traits {

template<typename, typename = void>
class has_call;

template<typename T>
class has_call<T, void>
{
    struct base_impl { void call() { } };
    struct base : T, base_impl { base() { } };

    template<typename U>
    static aux::mpl::aux::no_tag test(
        U*
      , non_type<void (base_impl::*)(), &U::call>* = 0
    );

    template<typename>
    static aux::mpl::aux::yes_tag test(...);

public:
    typedef has_call type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test<base>(0)) == sizeof(aux::mpl::aux::yes_tag)
    );
};

template<typename T, typename TAction>
class has_call
{
    template<typename>
    struct void_ { };

    template<typename S, typename U>
    friend U& operator,(const U&, void_<S>);

    struct base : T
    {
        using T::call;
        aux::mpl::aux::no_tag call(...) const;
    };

    template<typename, typename = void>
    struct base_call
        : aux::mpl::false_
    { };

    template<typename TDummy>
    struct base_call<aux::mpl::true_, TDummy>
        : is_same<
              BOOST_DI_FEATURE_DECLTYPE(
                 ((((base*)0)->call(*(TAction*)0)), void_<T>())
              )
            , void_<T>
          >
    { };

public :
    typedef has_call type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = base_call<aux::mpl::bool_<has_call<T>::value> >::value
    );
};

} // namespace type_traits
} // namespace di
} // namespace boost


    namespace boost {
    namespace di {
    namespace core {

    template<
        typename TDependecies = aux::mpl::vector<>
      , template<
            typename
          , template<typename> class = ::boost::di::core::binder
          , template<
                typename
              , typename
              , typename
              , typename
              , typename
              , typename
              , typename
              , typename
            >
            class = ::boost::di::core::any_type
        > class TCreator = creator
      , template<
            typename = ::boost::di::aux::mpl::vector<>
          , typename = ::boost::di::core::never< ::boost::di::aux::mpl::_1 >
          , typename = void
        > class TPool = pool
      , typename TDefaultAllocator = allocator
    >
    class module
        : public TPool<typename aux::mpl::normalize_vector<TDependecies>::type>
    {
        template<
            typename
          , template<
                typename
              , template<typename> class
              , template<
                    typename
                  , typename
                  , typename
                  , typename
                  , typename
                  , typename
                  , typename
                  , typename
                >
                class
            > class
          , template<typename, typename, typename> class
          , typename
        > friend class module;

        class empty_visitor
        {
        public:
            template<typename T>
            void operator()(const T&) const { }
        };

    public:
        typedef typename aux::mpl::normalize_vector<TDependecies>::type deps;

        explicit module(const TCreator<TDependecies>& creator = TCreator<TDependecies>())
            : creator_(creator)
        { }

    // bind<...>, etc.   -> ignore
    // module<....>      -> get all dependencies from the module
    // dependency<....>  -> pass

    template<typename... TArgs>
    explicit module(const TArgs&... args)
        : TPool<deps>(
              TPool<
                  typename aux::mpl::normalize_vector< ::boost::di::aux::mpl::vector<TArgs...> >::type
                , aux::mpl::not_<
                      aux::mpl::or_<
                          aux::mpl::contains<deps, aux::mpl::_>
                        , has_types<aux::mpl::_>
                      >
                  >
              >(args...)
            , init()
          )
    { }

    template<typename T, typename... TPolicies>
    T create(const TPolicies&... policies) {
        typedef aux::mpl::vector<> call_stack;
        TPool< ::boost::di::aux::mpl::vector<TPolicies...> > policies_(policies...);
        std::vector<aux::shared_ptr<void> > refs_;

        return creator_.template create<T, T, call_stack>(
            TDefaultAllocator()
          , static_cast<TPool<deps>&>(*this)
          , refs_
          , empty_visitor()
          , policies_
        );
    }

    template<typename T, typename TAllocator, typename... TPolicies>
    T allocate(const TAllocator& allocator, const TPolicies&... policies) {
        typedef aux::mpl::vector<> call_stack;
        TPool< ::boost::di::aux::mpl::vector<TPolicies...> > policies_(policies...);
        std::vector<aux::shared_ptr<void> > refs_;

        return creator_.template create<T, T, call_stack>(
            allocator
          , static_cast<TPool<deps>&>(*this)
          , refs_
          , empty_visitor()
          , policies_
        );
    }

        template<typename T>
        T create() {
            typedef aux::mpl::vector<> call_stack;
            std::vector<aux::shared_ptr<void> > refs_;

            return creator_.template create<T, T, call_stack>(
                TDefaultAllocator()
              , static_cast<TPool<deps>&>(*this)
              , refs_
              , empty_visitor()
              , TPool<>()
            );
        }

        template<typename T, typename TAllocator>
        T allocate(const TAllocator& allocator) {
            typedef aux::mpl::vector<> call_stack;
            std::vector<aux::shared_ptr<void> > refs_;

            return creator_.template create<T, T, call_stack>(
                allocator
              , static_cast<TPool<deps>&>(*this)
              , refs_
              , empty_visitor()
              , TPool<>()
            );
        }

        template<typename T, typename TVisitor>
        T visit(const TVisitor& visitor) {
            typedef aux::mpl::vector<> call_stack;
            std::vector<aux::shared_ptr<void> > refs_;

            return creator_.template create<T, T, call_stack>(
                TDefaultAllocator()
              , static_cast<TPool<deps>&>(*this)
              , refs_
              , visitor
              , TPool<>()
            );
        }

        template<typename TAction>
        void call(const TAction& action) {
            call_impl<deps>(static_cast<TPool<deps>&>(*this), action);
        }

    private:
        template<typename TSeq, typename T, typename TAction>
        typename enable_if<aux::mpl::empty<TSeq> >::type call_impl(T&, const TAction&) { }

        template<typename TSeq, typename T, typename TAction>
        typename enable_if<
            aux::mpl::and_<
                aux::mpl::not_<aux::mpl::empty<TSeq> >
              , type_traits::has_call<typename aux::mpl::front<TSeq>::type, TAction>
            >
        >::type
        call_impl(T& deps, const TAction& action) {
            static_cast<typename aux::mpl::front<TSeq>::type&>(deps).call(action);
            call_impl<typename aux::mpl::pop_front<TSeq>::type>(deps, action);
        }

        template<typename TSeq, typename T, typename TAction>
        typename disable_if<
            aux::mpl::or_<
                aux::mpl::empty<TSeq>
              , type_traits::has_call<typename aux::mpl::front<TSeq>::type, TAction>
            >
        >::type
        call_impl(T& deps, const TAction& action) {
            call_impl<typename aux::mpl::pop_front<TSeq>::type>(deps, action);
        }

        TCreator<TDependecies> creator_;
    };

    } // namespace core
    } // namespace di
    } // namespace boost


namespace boost {
namespace di {
namespace type_traits {

template<typename T>
class has_configure
{
    struct base_impl { void configure() { } };
    struct base
        : base_impl
        , aux::mpl::if_<is_class<T>, T, aux::mpl::void_>::type
    { base() { } };

    template<typename U>
    static aux::mpl::aux::no_tag test(
        U*
      , non_type<void (base_impl::*)(), &U::configure>* = 0
    );

    template<typename>
    static aux::mpl::aux::yes_tag test(...);

public:
    typedef has_configure type;

    BOOST_STATIC_CONSTANT(
        bool
      , value = sizeof(test<base>(0)) == sizeof(aux::mpl::aux::yes_tag)
    );
};

} // namespace type_traits
} // namespace di
} // namespace boost


    namespace boost {
    namespace di {

    namespace detail {

    template<typename T>
    struct default_scope
        : scope<scopes::deduce>::bind<T>
    { };

    BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)

    template<typename T>
    struct is_module
        : aux::mpl::or_<
              has_deps<T>
            , type_traits::has_configure<T>
          >
    { };

    template<typename T, typename = void>
    struct get_module
    {
        typedef T type;
    };

    template<typename T>
    struct get_module<T, typename enable_if<type_traits::has_configure<T> >::type>
    {
        typedef typename type_traits::function_traits<
            BOOST_DI_FEATURE_DECLTYPE(&T::configure)
        >::result_type type;
    };

    template<typename T, typename = void>
    struct get_deps
    {
        typedef typename T::deps type;
    };

    template<typename T>
    struct get_deps<T, typename enable_if<type_traits::has_configure<T> >::type>
    {
        typedef typename get_module<T>::type::deps type;
    };

    template<
        typename TSeq
      , typename TDefaultScope = default_scope<aux::mpl::_2>
    >
    struct concepts
        : aux::mpl::fold<
              TSeq
            , aux::mpl::vector<>
            , aux::mpl::copy<
                  aux::mpl::if_<
                      aux::mpl::is_sequence<aux::mpl::_2>
                    , aux::mpl::_2
                    , aux::mpl::if_<
                          is_module<aux::mpl::_2>
                        , get_deps<aux::mpl::_2>
                        , TDefaultScope
                      >
                  >
                , aux::mpl::back_inserter<aux::mpl::_1>
              >
          >::type
    { };

    } // namespace detail

    template<typename... T_MPL>
    class injector
        : public core::module<
              typename detail::concepts<
                  ::boost::di::aux::mpl::vector<T_MPL...>
              >::type
          >
    {
        template<typename TSeq = aux::mpl::vector<> >
        struct joint_concepts
            : detail::concepts<
                  aux::mpl::joint_view<
                      ::boost::di::aux::mpl::vector<T_MPL...>
                    , TSeq
                  >
              >::type
        { };

    public:
        injector() { }

    template<typename... TArgs>
    explicit injector(const TArgs&... args)
        : core::module<typename joint_concepts<>::type>(
              pass_arg(args)...
          )
    { }

    private:
        template<typename T>
        const T& pass_arg(const T& arg
                        , typename disable_if<type_traits::has_configure<T> >::type* = 0) const {
            return arg;
        }

        template<typename T>
        typename detail::get_module<T>::type
        pass_arg(const T& arg
               , typename enable_if<type_traits::has_configure<T> >::type* = 0) const {
            return arg.configure();
        }
    };

    } // namespace di
    } // namespace boost


    namespace boost {
    namespace di {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(scope)

    inline injector<> make_injector() {
        return injector<>();
    }

    template<typename... TArgs>
    injector<typename detail::concepts< ::boost::di::aux::mpl::vector<TArgs...> >::type>
    inline make_injector(const TArgs&... args) {
        return injector<
            typename detail::concepts<
                ::boost::di::aux::mpl::vector<TArgs...>
              , aux::mpl::if_<
                    has_scope<aux::mpl::_2>
                  , detail::default_scope<aux::mpl::_2>
                  , aux::mpl::_2 // argument not supported
                >
            >::type
        >(args...);
    }

    } // namespace di
    } // namespace boost

namespace boost {
namespace di {

template<typename T>
class provider
{
public:
    virtual ~provider() { }
    virtual T get() const = 0;
};

} // namespace di
} // namespace boost

#endif

