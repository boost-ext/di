//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_HPP
#define BOOST_DI_HPP
#if (__cplusplus < 201305L && _MSC_VER < 1900)
#error "Boost.DI requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)"
#else
#define BOOST_DI_VERSION 100000
#if defined(__clang__)
    #define BOOST_DI_UNUSED __attribute__((unused))
    #define BOOST_DI_DEPRECATED(...) [[deprecated(__VA_ARGS__)]]
    #define BOOST_DI_TYPE_WKND(T)
    #define BOOST_DI_DECLTYPE_WKND(...) decltype(__VA_ARGS__)
#elif defined(__GNUC__)
    #define BOOST_DI_UNUSED __attribute__((unused))
    #define BOOST_DI_DEPRECATED(...) [[deprecated(__VA_ARGS__)]]
    #define BOOST_DI_TYPE_WKND(T)
    #define BOOST_DI_DECLTYPE_WKND(...) decltype(__VA_ARGS__)
#elif defined(_MSC_VER)
    #pragma warning(disable : 4503)
    #pragma warning(disable : 4822)
    #define __has_include(...) 0
    #define BOOST_DI_UNUSED
    #define BOOST_DI_DEPRECATED(...) __declspec(deprecated(__VA_ARGS__))
    #define BOOST_DI_TYPE_WKND(T) (T&&)
    namespace boost { namespace di { inline namespace v1 { namespace detail { template<class T> T get_type__(T); }}}}
    #define BOOST_DI_DECLTYPE_WKND(...) decltype(::boost::di::v1::detail::get_type__(__VA_ARGS__))
#endif
namespace boost { namespace di { inline namespace v1 {
struct _ { _(...) { } };
namespace aux {
template<class...>
struct type_list { using type = type_list; };
template<class...>
struct valid_t { using type = int; };
template<class...>
struct type { };
struct none_type { };
template<class T, T>
struct non_type { };
template<class T>
using owner = T;
template<class...>
struct always {
    static constexpr auto value = true;
};
template<class...>
struct never {
    static constexpr auto value = false;
};
template<class, class>
struct pair { using type = pair; };
template<bool...>
struct bool_list { using type = bool_list; };
template<class... Ts>
struct inherit : Ts... { using type = inherit; };
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
template<int...>
struct index_sequence {
    using type = index_sequence;
};
template<int>
struct make_index_sequence_impl;
template<>
struct make_index_sequence_impl<0> : index_sequence<> { };
template<>
struct make_index_sequence_impl<1> : index_sequence<1> { };
template<>
struct make_index_sequence_impl<2> : index_sequence<1, 2> { };
template<>
struct make_index_sequence_impl<3> : index_sequence<1, 2, 3> { };
template<>
struct make_index_sequence_impl<4> : index_sequence<1, 2, 3, 4> { };
template<>
struct make_index_sequence_impl<5> : index_sequence<1, 2, 3, 4, 5> { };
template<>
struct make_index_sequence_impl<6> : index_sequence<1, 2, 3, 4, 5, 6> { };
template<>
struct make_index_sequence_impl<7> : index_sequence<1, 2, 3, 4, 5, 6, 7> { };
template<>
struct make_index_sequence_impl<8> : index_sequence<1, 2, 3, 4, 5, 6, 7, 8> { };
template<>
struct make_index_sequence_impl<9> : index_sequence<1, 2, 3, 4, 5, 6, 7, 8, 9> { };
template<>
struct make_index_sequence_impl<10> : index_sequence<1, 2, 3, 4, 5, 6, 7, 8, 9, 10> { };
template<int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
}}}}
#if __has_include(<__config>)
    #include <__config>
#endif
#if defined(_LIBCPP_VERSION)
_LIBCPP_BEGIN_NAMESPACE_STD
#else
namespace std {
#endif
    template<class> class shared_ptr;
    template<class> class weak_ptr;
    template<class, class> class unique_ptr;
    template<class> struct char_traits;
    template<class, class> class vector;
    template<class, class, class> class set;
    template<class> class move_iterator;
#if defined(_LIBCPP_VERSION)
_LIBCPP_END_NAMESPACE_STD
#else
}
#endif
namespace std {
    template<class> class initializer_list;
}
namespace boost {
    template<class> class shared_ptr;
}
namespace boost { namespace di { inline namespace v1 {
    struct no_name {
        constexpr auto operator()() const noexcept { return ""; }
    };
    template<class, class = void> struct named { };
    template<class, class = int> struct ctor_traits;
    struct self { };
    namespace core {
        template<class> struct any_type_fwd;
        template<class> struct any_type_ref_fwd;
        template<class> struct any_type_1st_fwd;
        template<class> struct any_type_1st_ref_fwd;
        template<class T>
        struct dependency__ : T {
            using T::try_create;
            using T::is_referable;
            using T::create;
        };
        template<class T>
        struct injector__ : T {
            using T::try_create;
            using T::create_impl;
            using T::create_successful_impl;
            #if defined(_MSC_VER)
                template<class... Ts> using is_creatable =
                    typename T::template is_creatable<Ts...>;
            #else
                using T::is_creatable;
            #endif
        };
        template<class, class...>
        struct array;
    }
    namespace concepts { template<class...> struct boundable__; template<class...> struct any_of; }
}}}
#define BOOST_DI_REQUIRES(...) typename ::boost::di::aux::enable_if<__VA_ARGS__, int>::type
#define BOOST_DI_REQUIRES_MSG(...) typename ::boost::di::aux::concept_check<__VA_ARGS__>::type
namespace boost { namespace di { inline namespace v1 { namespace aux {
template<class T, T V>
struct integral_constant {
    using type = integral_constant;
    static constexpr T value = V;
};
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;
template<bool B, class T, class F> struct conditional { using type = T; };
template<class T, class F> struct conditional<false, T, F> { using type = F; };
template<bool B, class T, class F> using conditional_t = typename conditional<B, T, F>::type;
template<bool B, class T = void> struct enable_if { };
template<class T> struct enable_if<true, T> { using type = T; };
template<bool B, class T = void> using enable_if_t = typename enable_if<B, T>::type;
template<class T> struct concept_check { static_assert(T::value, "constraint not satisfied"); };
template<> struct concept_check<true_type> { using type = int; };
template<class T> T&& declval();
template<class T> T decval(T);
template< class T > struct remove_reference {typedef T type;};
template< class T > struct remove_reference<T&> {typedef T type;};
template< class T > struct remove_reference<T&&> {typedef T type;};
template< class T >
using remove_reference_t = typename remove_reference<T>::type;
template<class T> struct remove_specifiers { using type = T; };
template<class T> struct remove_specifiers<const T> { using type = T; };
template<class T> struct remove_specifiers<T&> { using type = T; };
template<class T> struct remove_specifiers<const T&> { using type = T; };
template<class T> struct remove_specifiers<T*> { using type = T; };
template<class T> struct remove_specifiers<const T*> { using type = T; };
template<class T> struct remove_specifiers<T* const &> { using type = T; };
template<class T> struct remove_specifiers<T* const> { using type = T; };
template<class T> struct remove_specifiers<const T* const> { using type = T; };
template<class T> struct remove_specifiers<T&&> { using type = T; };
template<class T> using remove_specifiers_t = typename remove_specifiers<T>::type;
template<class T>
struct deref_type { using type = T; };
template<class T, class TDeleter>
struct deref_type<std::unique_ptr<T, TDeleter>> {
    using type = remove_specifiers_t<typename deref_type<T>::type>;
};
template<class T>
struct deref_type<std::shared_ptr<T>> {
    using type = remove_specifiers_t<typename deref_type<T>::type>;
};
template<class T>
struct deref_type<boost::shared_ptr<T>> {
    using type = remove_specifiers_t<typename deref_type<T>::type>;
};
template<class T>
struct deref_type<std::weak_ptr<T>> {
    using type = remove_specifiers_t<typename deref_type<T>::type>;
};
template<class T, class TAllocator>
struct deref_type<std::vector<T, TAllocator>> {
    using type = core::array<remove_specifiers_t<typename deref_type<T>::type>*[]>;
};
template<class TKey, class TCompare, class TAllocator>
struct deref_type<std::set<TKey, TCompare, TAllocator>> {
    using type = core::array<remove_specifiers_t<typename deref_type<TKey>::type>*[]>;
};
template<class T>
using decay_t = typename deref_type<remove_specifiers_t<T>>::type;
template<class, class> struct is_same : false_type { };
template<class T> struct is_same<T, T> : true_type { };
template<class T, class U> struct is_base_of : integral_constant<bool, __is_base_of(T, U)> { };
template<class T, class U>
struct is_same_or_base_of {
    static constexpr auto value = is_same<T, U>::value || is_base_of<T, U>::value;
};
template<class T> struct is_class : integral_constant<bool, __is_class(T)> { };
template<class T> struct is_abstract : integral_constant<bool, __is_abstract(T)> { };
template<class T> struct is_polymorphic : integral_constant<bool, __is_polymorphic(T)> { };
template<class...> using is_valid_expr = true_type;
template<class T, class... TArgs>
decltype(void(T(declval<TArgs>()...)), true_type{}) test_is_constructible(int);
template<class, class...>
false_type test_is_constructible(...);
template<class T, class... TArgs>
using is_constructible = decltype(test_is_constructible<T, TArgs...>(0));
template<class T, class... TArgs>
using is_constructible_t = typename is_constructible<T, TArgs...>::type;
#if defined(_MSC_VER)
    template<class T>
    struct is_copy_constructible : integral_constant<bool, __is_constructible(T, const T&)> { };
    template<class T>
    struct is_default_constructible : integral_constant<bool, __is_constructible(T)> { };
#else
    template<class T>
    using is_copy_constructible = is_constructible<T, const T&>;
    template<class T>
    using is_default_constructible = is_constructible<T>;
#endif
template<class T, class... TArgs>
decltype(void(T{declval<TArgs>()...}), true_type{}) test_is_braces_constructible(int);
template<class, class...>
false_type test_is_braces_constructible(...);
template<class T, class... TArgs>
using is_braces_constructible = decltype(test_is_braces_constructible<T, TArgs...>(0));
template<class T, class... TArgs>
using is_braces_constructible_t = typename is_braces_constructible<T, TArgs...>::type;
#if defined(__clang__) || defined(_MSC_VER)
    template<class T, class U>
    struct is_convertible : integral_constant<bool, __is_convertible_to(T, U)> { };
#else
  template<typename _From, typename _To>
    class __is_convertible_helper {
       template<typename _To1> static void __test_aux(_To1);
      template<typename _From1, typename _To1,
      typename = decltype(__test_aux<_To1>(declval<_From1>()))>
      static true_type __test(int);
      template<typename, typename> static false_type __test(...);
    public:
      typedef decltype(__test<_From, _To>(0)) type;
    };
  template<typename _From, typename _To>
    struct is_convertible : public __is_convertible_helper<_From, _To>::type
    { };
#endif
template<class TSrc, class TDst, class U = remove_specifiers_t<TDst>>
using is_narrowed = integral_constant<bool,
    !is_class<TSrc>::value && !is_class<U>::value && !is_same<TSrc, U>::value
>;
template<class, class...> struct is_array : false_type { };
template<class T, class... Ts> struct is_array<T[], Ts...> : true_type { };
template<class T, class = decltype(sizeof(T))>
true_type is_complete_impl(int);
template<class T>
false_type is_complete_impl(...);
template<class T>
struct is_complete : decltype(is_complete_impl<T>(0)) { };
template<class, class...>
struct is_unique_impl;
template<class...>
struct not_unique : false_type {
    using type = not_unique;
};
template<>
struct not_unique<> : true_type {
    using type = not_unique;
};
template<class T>
struct is_unique_impl<T> : not_unique<> { };
template<class T1, class T2, class... Ts>
struct is_unique_impl<T1, T2, Ts...>
    : conditional_t<
          is_base_of<type<T2>, T1>::value
        , not_unique<T2>
        , is_unique_impl<inherit<T1, type<T2>>, Ts...>
      >
{ };
template<class... Ts>
using is_unique = is_unique_impl<none_type, Ts...>;
template<class T, class... TArgs> decltype(::boost::di::aux::declval<T>().operator()( ::boost::di::aux::declval<TArgs>()...) , ::boost::di::aux::true_type()) is_callable_with_impl(int); template<class, class...> ::boost::di::aux::false_type is_callable_with_impl(...); template<class T, class... TArgs> struct is_callable_with : decltype(is_callable_with_impl<T, TArgs...>(0)) { };
struct callable_base_impl { void operator()(...) { } };
template<class T>
struct callable_base
    : callable_base_impl, aux::conditional_t<aux::is_class<T>::value, T, aux::none_type>
{ };
template<typename T>
aux::false_type is_callable_impl(T*, aux::non_type<void(callable_base_impl::*)(...), &T::operator()>* = 0);
aux::true_type is_callable_impl(...);
template<class T>
struct is_callable : decltype(is_callable_impl((callable_base<T>*)0)) { };
template<class>
struct function_traits;
template<class R, class... TArgs>
struct function_traits<R(*)(TArgs...)> {
    using result_type = R;
    using args = type_list<TArgs...>;
};
template<class R, class... TArgs>
struct function_traits<R(TArgs...)> {
    using result_type = R;
    using args = type_list<TArgs...>;
};
template<class R, class T, class... TArgs>
struct function_traits<R(T::*)(TArgs...)> {
    using result_type = R;
    using args = type_list<TArgs...>;
};
template<class R, class T, class... TArgs>
struct function_traits<R(T::*)(TArgs...) const> {
    using result_type = R;
    using args = type_list<TArgs...>;
};
template<class T>
using function_traits_t = typename function_traits<T>::args;
}}}}
namespace boost { namespace di { inline namespace v1 { namespace core {
template<class = aux::type_list<>>
struct pool;
template<class... TArgs>
using pool_t = pool<aux::type_list<TArgs...>>;
template<class... TArgs>
struct pool<aux::type_list<TArgs...>> : TArgs... {
    template<class... Ts>
    explicit pool(const Ts&... args) noexcept
        : Ts(args)...
    { }
    template<class... Ts, class TPool>
    pool(const aux::type_list<Ts...>&, const TPool& p) noexcept
        : pool(static_cast<const Ts&>(p)...)
    { }
};
}}}}
namespace boost { namespace di { inline namespace v1 { namespace type_traits {
template<class T, class U>
struct rebind_traits {
    using type = U;
};
template<class T, class U>
struct rebind_traits<T, named<U>> {
    using type = named<U, T>;
};
template<class T, class U>
struct rebind_traits<std::shared_ptr<T>, U> {
    using type = std::shared_ptr<U>;
};
template<class T, class U>
struct rebind_traits<std::shared_ptr<T>, named<U>> {
    using type = named<U, std::shared_ptr<T>>;
};
template<class T, class D, class U>
struct rebind_traits<std::unique_ptr<T, D>, U> {
    using type = std::unique_ptr<U, D>;
};
template<class T, class D, class U>
struct rebind_traits<std::unique_ptr<T, D>, named<U>> {
    using type = named<U, std::unique_ptr<T, D>>;
};
template<class T, class U>
using rebind_traits_t = typename rebind_traits<T, U>::type;
}}}}
namespace boost { namespace di { inline namespace v1 { namespace core {
template<class T, int N>
struct array__ { T array_[N]; };
template<class T>
struct array__<T, 0> { T array_[1]; };
template<class T, class... Ts>
struct array : array__<typename T::value_type, sizeof...(Ts)>, T {
    using value_type = typename T::value_type;
    using array_t = array__<value_type, sizeof...(Ts)>;
    using array_t::array_;
    using boost_di_inject__ = aux::type_list<self>;
    template<class TInjector>
    explicit array(const TInjector& injector)
        : array(injector, aux::true_type{})
    { }
    template<class TInjector>
    array(const TInjector& injector, const aux::true_type&)
        : array_t{{
            *static_cast<const core::injector__<TInjector>&>(injector).
                create_successful_impl(aux::type<type_traits::rebind_traits_t<value_type, Ts>>{})...
         }}
        , T(std::move_iterator<value_type*>(array_)
          , std::move_iterator<value_type*>(array_ + sizeof...(Ts)))
    { }
    template<class TInjector>
    explicit array(const TInjector& injector, const aux::false_type&) {
        int _[]{0, (
            static_cast<const core::injector__<TInjector>&>(injector).
                create_impl(aux::type<type_traits::rebind_traits_t<value_type, Ts>>{})
        , 0)...}; (void)_;
    }
};
template<class T, class... Ts>
struct array<T*[], Ts...> { };
}}}}
namespace boost { namespace di { inline namespace v1 { namespace wrappers {
template<class T>
struct unique {
    template<class I, BOOST_DI_REQUIRES(aux::is_convertible<T, I>::value) = 0>
    inline operator I() const noexcept {
        return object;
    }
    inline operator T&&() noexcept {
        return static_cast<T&&>(object);
    }
    T object;
};
template<class T>
struct unique<T*> {
    #if defined(_MSC_VER)
        explicit unique(T* object)
            : object(object)
        { }
    #endif
    template<class I, BOOST_DI_REQUIRES(aux::is_convertible<T, I>::value) = 0>
    inline operator I() const noexcept {
        struct scoped_ptr { aux::owner<T*> ptr; ~scoped_ptr() noexcept { delete ptr; } };
        return *scoped_ptr{object}.ptr;
    }
    template<class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
    inline operator aux::owner<I*>() const noexcept {
        return object;
    }
    template<class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, const I*>::value) = 0>
    inline operator aux::owner<const I*>() const noexcept {
        return object;
    }
    template<class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
    inline operator std::shared_ptr<I>() const noexcept {
        return std::shared_ptr<I>{object};
    }
    template<class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
    inline operator boost::shared_ptr<I>() const noexcept {
        return boost::shared_ptr<I>{object};
    }
    template<class I, class D, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
    inline operator std::unique_ptr<I, D>() const noexcept {
        return std::unique_ptr<I, D>{object};
    }
    T* object = nullptr;
};
}}}}
namespace boost { namespace di { inline namespace v1 { namespace type_traits {
struct stack { };
struct heap { };
template<class T, class = int>
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
template<class T>
struct memory_traits<boost::shared_ptr<T>> {
    using type = heap;
};
template<class T>
struct memory_traits<const boost::shared_ptr<T>&> {
    using type = heap;
};
template<class T>
struct memory_traits<std::weak_ptr<T>> {
    using type = heap;
};
template<class T>
struct memory_traits<const std::weak_ptr<T>&> {
    using type = heap;
};
template<class T>
struct memory_traits<T, BOOST_DI_REQUIRES(aux::is_polymorphic<T>::value)> {
    using type = heap;
};
template<class T>
using memory_traits_t = typename memory_traits<T>::type;
}}}}
namespace boost { namespace di { inline namespace v1 { namespace scopes {
class unique {
public:
    template<class, class>
    class scope {
    public:
        template<class>
        using is_referable = aux::false_type;
        template<class T, class TProvider>
        static decltype(wrappers::unique<decltype(
            aux::declval<TProvider>().get(type_traits::memory_traits_t<T>{}))>{
                aux::declval<TProvider>().get(type_traits::memory_traits_t<T>{})})
        try_create(const TProvider&);
        template<class T, class TProvider>
        auto create(const TProvider& provider) const {
            using memory = type_traits::memory_traits_t<T>;
            using wrapper = wrappers::unique<decltype(provider.get(memory{}))>;
            return wrapper{provider.get(memory{})};
        }
    };
};
}}}}
namespace boost { namespace di { inline namespace v1 { namespace wrappers {
template<class T, class TObject = std::shared_ptr<T>>
struct shared {
    template<class>
    struct is_referable_impl
        : aux::true_type
    { };
    template<class I>
    struct is_referable_impl<std::shared_ptr<I>>
        : aux::is_same<I, T>
    { };
    template<class I>
    struct is_referable_impl<boost::shared_ptr<I>>
        : aux::false_type
    { };
    template<class T_>
    using is_referable = is_referable_impl<aux::remove_specifiers_t<T_>>;
    template<class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
    inline operator std::shared_ptr<I>() const noexcept {
        return object;
    }
    inline operator std::shared_ptr<T>&() noexcept {
        return object;
    }
    template<class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
    inline operator boost::shared_ptr<I>() const noexcept {
        struct sp_holder {
            std::shared_ptr<T> object;
            void operator()(...) noexcept { object.reset(); }
        };
        return {object.get(), sp_holder{object}};
    }
    template<class I, BOOST_DI_REQUIRES(aux::is_convertible<T*, I*>::value) = 0>
    inline operator std::weak_ptr<I>() const noexcept {
        return object;
    }
    inline operator T&() noexcept {
        return *object;
    }
    inline operator const T&() const noexcept {
        return *object;
    }
    TObject object;
};
template<class T>
struct shared<T&> {
    template<class>
    struct is_referable
        : aux::true_type
    { };
    explicit shared(T& object)
        : object(&object)
    { }
    explicit shared(T&&);
    template<class I, BOOST_DI_REQUIRES(aux::is_convertible<T, I>::value) = 0>
    inline operator I() const noexcept {
        return *object;
    }
    inline operator T&() const noexcept {
        return *object;
    }
    T* object = nullptr;
};
}}}}
namespace boost { namespace di { inline namespace v1 { namespace scopes {
aux::false_type has_shared_ptr__(...);
template<class T>
auto has_shared_ptr__(T&&) -> aux::is_valid_expr<decltype(std::shared_ptr<T>{})>;
class singleton {
public:
    template<class, class T, class = decltype(has_shared_ptr__(aux::declval<T>()))>
    class scope {
    public:
        template<class T_>
        using is_referable = typename wrappers::shared<T&>::template is_referable<T_>;
        template<class, class TProvider>
        static decltype(wrappers::shared<T&>{aux::declval<TProvider>().get(type_traits::stack{})})
        try_create(const TProvider&);
        template<class, class TProvider>
        auto create(const TProvider& provider) {
            return create_impl(provider);
        }
    private:
        template<class TProvider>
        auto create_impl(const TProvider& provider) {
            static T object(provider.get(type_traits::stack{}));
            return wrappers::shared<T&>(object);
        }
    };
    template<class _, class T>
    class scope<_, T, aux::true_type> {
    public:
        template<class T_>
        using is_referable = typename wrappers::shared<T>::template is_referable<T_>;
        template<class, class TProvider, class T_ = aux::decay_t<decltype(aux::declval<TProvider>().get())>>
        static decltype(wrappers::shared<T_>{std::shared_ptr<T_>{std::shared_ptr<T_>{aux::declval<TProvider>().get()}}})
        try_create(const TProvider&);
        template<class T_, class TProvider>
        auto create(const TProvider& provider) {
            return create_impl<aux::decay_t<decltype(provider.get())>>(provider);
        }
    private:
        template<class T_, class TProvider>
        auto create_impl(const TProvider& provider) {
            static std::shared_ptr<T_> object{provider.get()};
            return wrappers::shared<T_, std::shared_ptr<T_>&>{object};
        }
    };
};
}}}}
namespace boost { namespace di { inline namespace v1 { namespace type_traits {
template<class T>
struct scope_traits {
    using type = scopes::unique;
};
template<class T>
struct scope_traits<T&> {
    using type = scopes::singleton;
};
template<class T>
struct scope_traits<const T&> {
    using type = scopes::singleton;
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
template<class T>
struct scope_traits<boost::shared_ptr<T>> {
    using type = scopes::singleton;
};
template<class T>
struct scope_traits<const boost::shared_ptr<T>&> {
    using type = scopes::singleton;
};
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
}}}}
namespace boost { namespace di { inline namespace v1 { namespace scopes {
class deduce {
public:
    template<class TExpected, class TGiven>
    class scope {
    public:
        template<class T>
        using is_referable = typename type_traits::scope_traits_t<T>::template
            scope<TExpected, TGiven>::template is_referable<T>;
        template<class T, class TProvider>
        static decltype(typename type_traits::scope_traits_t<T>::template
            scope<TExpected, TGiven>{}.template try_create<T>(aux::declval<TProvider>()))
        try_create(const TProvider&);
        template<class T, class TProvider>
        auto create(const TProvider& provider) {
            using scope_traits = type_traits::scope_traits_t<T>;
            using scope = typename scope_traits::template scope<TExpected, TGiven>;
            return scope{}.template create<T>(provider);
        }
    };
};
}}}}
namespace boost { namespace di { inline namespace v1 { namespace scopes {
template<class, class = int> struct has_deps : ::boost::di::aux::false_type { }; template<class T> struct has_deps<T, typename ::boost::di::aux::valid_t<typename T::deps>::type> : ::boost::di::aux::true_type { };
template<class TScope = scopes::deduce>
class exposed {
public:
    template<class TExpected, class TGiven>
    class scope {
        #if defined(__GNUC__) || defined(_MSC_VER)
            using type = aux::conditional_t<
                aux::is_copy_constructible<TExpected>::value
              , TExpected
              , TExpected*
            >;
        #else
            using type = TExpected;
        #endif
        struct iprovider {
            TExpected* (*heap)(const iprovider*) = nullptr;
            type (*stack)(const iprovider*) = nullptr;
            auto get(const type_traits::heap& = {}) const noexcept {
                return ((iprovider*)(this))->heap(this);
            }
            auto get(const type_traits::stack&) const noexcept {
                return ((iprovider*)(this))->stack(this);
            }
        };
        template<class TInjector>
        struct provider_impl {
            TExpected* (*heap)(const provider_impl*) = nullptr;
            type (*stack)(const provider_impl*) = nullptr;
            template<class T>
            static T create(const provider_impl* object) noexcept {
                return static_cast<const core::injector__<TInjector>&>(object->injector).create_impl(aux::type<T>{});
            }
            template<class T>
            static T create_successful(const provider_impl* object) noexcept {
                return static_cast<const core::injector__<TInjector>&>(object->injector).create_successful_impl(aux::type<T>{});
            }
            explicit provider_impl(const TInjector& injector) noexcept
                : provider_impl(injector
                              , aux::integral_constant<bool, core::injector__<TInjector>::template is_creatable<TExpected*>::value>{}
                              , aux::integral_constant<bool, core::injector__<TInjector>::template is_creatable<TExpected>::value>{}
                  )
            { }
            provider_impl(const TInjector& injector, const aux::true_type&, const aux::true_type&) noexcept
                : heap(&provider_impl::template create_successful<TExpected*>)
                , stack(&provider_impl::template create_successful<type>)
                , injector(injector)
            { }
            provider_impl(const TInjector& injector, const aux::false_type&, const aux::true_type&) noexcept
                : stack(&provider_impl::template create_successful<type>)
                , injector(injector)
            { }
            provider_impl(const TInjector& injector, const aux::true_type&, const aux::false_type&) noexcept
                : heap(&provider_impl::template create_successful<TExpected*>)
                , injector(injector)
            { }
            provider_impl(const TInjector& injector, const aux::false_type&, const aux::false_type&)
                : heap(&provider_impl::template create<TExpected*>)
                , injector(injector)
            { }
            TInjector injector;
        };
    public:
        template<class>
        using is_referable = aux::false_type;
        template<class TInjector, BOOST_DI_REQUIRES(has_deps<TInjector>::value) = 0>
        explicit scope(const TInjector& injector) noexcept {
            static auto provider = provider_impl<TInjector>{injector};
            provider.injector = injector;
            provider_ = (iprovider*)&provider;
        }
        template<class T, class TProvider>
        static T try_create(const TProvider&);
        template<class T, class TProvider>
        auto create(const TProvider&) {
            return scope_.template create<T>(*provider_);
        }
    private:
        iprovider* provider_ = nullptr;
        typename TScope::template scope<TExpected, TGiven> scope_;
    };
};
}}}}
namespace boost { namespace di { inline namespace v1 { namespace scopes {
namespace detail {
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
template<class, class = int> struct has_result_type : ::boost::di::aux::false_type { }; template<class T> struct has_result_type<T, typename ::boost::di::aux::valid_t<typename T::result_type>::type> : ::boost::di::aux::true_type { };
template<class TGiven, class TProvider, class... Ts>
struct is_expr : aux::integral_constant<bool,
    aux::is_callable_with<TGiven, no_implicit_conversions<
        aux::remove_specifiers_t<decltype(aux::declval<TProvider>().injector_)>
    >, Ts...>::value && !has_result_type<TGiven>::value
> { };
}
class external {
public:
    template<class TExpected, class, class = int>
    struct scope {
        template<class>
        using is_referable = aux::false_type;
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
        using is_referable = aux::false_type;
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
    struct scope<TExpected, TGiven&, BOOST_DI_REQUIRES(!aux::is_callable<TGiven>::value)> {
        template<class>
        using is_referable = aux::true_type;
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
    struct scope<TExpected, TGiven, BOOST_DI_REQUIRES(aux::is_callable<TGiven>::value)> {
        template<class>
        using is_referable = aux::false_type;
        explicit scope(const TGiven& object)
            : object_(object)
        { }
        template<class T, class TProvider>
        T static try_create(const TProvider&);
        template<class, class TProvider,
            BOOST_DI_REQUIRES(!detail::is_expr<TGiven, TProvider>::value &&
                               aux::is_callable<TGiven>::value &&
                               aux::is_callable<TExpected>::value) = 0>
        auto create(const TProvider&) const noexcept {
            return wrappers::unique<TExpected>{object_};
        }
        template<class T, class TProvider,
            BOOST_DI_REQUIRES(!detail::is_expr<TGiven, TProvider>::value &&
                               aux::is_callable_with<TGiven>::value &&
                              !aux::is_callable<TExpected>::value) = 0>
        auto create(const TProvider&) const noexcept {
            using wrapper = detail::wrapper_traits_t<decltype(aux::declval<TGiven>()())>;
            return wrapper{object_()};
        }
        template<class, class TProvider, BOOST_DI_REQUIRES(detail::is_expr<TGiven, TProvider>::value) = 0>
        auto create(const TProvider& provider) noexcept {
            using wrapper = detail::wrapper_traits_t<decltype((object_)(provider.injector_))>;
            return wrapper{(object_)(provider.injector_)};
        }
        template<class T, class TProvider, BOOST_DI_REQUIRES(detail::is_expr<TGiven, TProvider, const detail::arg<T, TExpected, TGiven>&>::value) = 0>
        auto create(const TProvider& provider) noexcept {
            using wrapper = detail::wrapper_traits_t<decltype((object_)(provider.injector_, detail::arg<T, TExpected, TGiven>{}))>;
            return wrapper{(object_)(provider.injector_, detail::arg<T, TExpected, TGiven>{})};
        }
        TGiven object_;
    };
};
}}}}
#if !defined(BOOST_DI_CFG_CTOR_LIMIT_SIZE)
#define BOOST_DI_CFG_CTOR_LIMIT_SIZE 10
#endif
namespace boost { namespace di { inline namespace v1 { namespace type_traits {
template<class, class = int> struct is_injectable : ::boost::di::aux::false_type { }; template<class T> struct is_injectable<T, typename ::boost::di::aux::valid_t<typename T::boost_di_inject__>::type> : ::boost::di::aux::true_type { };
struct direct { };
struct uniform { };
template<class T, int>
using get = T;
template<template<class...> class, class, class, class = int>
struct ctor_impl;
template<template<class...> class TIsConstructible, class T>
struct ctor_impl<TIsConstructible, T, aux::index_sequence<>>
    : aux::type_list<>
{ };
template<template<class...> class TIsConstructible, class T>
struct ctor_impl<TIsConstructible, T, aux::index_sequence<1>
    , BOOST_DI_REQUIRES(TIsConstructible<T, core::any_type_1st_fwd<T>>::value)>
    : aux::type_list<core::any_type_1st_fwd<T>>
{ };
template<template<class...> class TIsConstructible, class T>
struct ctor_impl<TIsConstructible, T, aux::index_sequence<1>
    , BOOST_DI_REQUIRES(!TIsConstructible<T, core::any_type_1st_fwd<T>>::value)>
    : aux::conditional_t<
          TIsConstructible<T, core::any_type_1st_ref_fwd<T>>::value
        , aux::type_list<core::any_type_1st_ref_fwd<T>>
        , aux::type_list<>
      >
{ };
template<template<class...> class TIsConstructible, class T, int... TArgs>
struct ctor_impl<TIsConstructible, T, aux::index_sequence<TArgs...>,
      BOOST_DI_REQUIRES((sizeof...(TArgs) > 1) && TIsConstructible<T, get<core::any_type_fwd<T>, TArgs>...>::value)>
    : aux::type_list<get<core::any_type_fwd<T>, TArgs>...>
{ };
template<template<class...> class TIsConstructible, class T, int... TArgs>
struct ctor_impl<TIsConstructible, T, aux::index_sequence<TArgs...>
    , BOOST_DI_REQUIRES((sizeof...(TArgs) > 1) && !TIsConstructible<T, get<core::any_type_fwd<T>, TArgs>...>::value)>
    : aux::conditional<
           TIsConstructible<T, get<core::any_type_ref_fwd<T>, TArgs>...>::value
         , aux::type_list<get<core::any_type_ref_fwd<T>, TArgs>...>
         , typename ctor_impl<
               TIsConstructible
             , T
             , aux::make_index_sequence<sizeof...(TArgs) - 1>
           >::type
      >
{ };
template<template<class...> class TIsConstructible, class T>
using ctor_impl_t =
    typename ctor_impl<
        TIsConstructible
      , T
      , aux::make_index_sequence<BOOST_DI_CFG_CTOR_LIMIT_SIZE>
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
template<
    class T
  , class = typename is_injectable<T>::type
> struct ctor_traits__;
template<
    class T
  , class = typename is_injectable<di::ctor_traits<T>>::type
> struct ctor_traits_impl;
template<class T>
struct ctor_traits__<T, aux::true_type>
    : aux::pair<direct, typename T::boost_di_inject__::type>
{ };
template<class T>
struct ctor_traits__<T, aux::false_type>
    : ctor_traits_impl<T>
{ };
template<class T>
struct ctor_traits_impl<T, aux::true_type>
    : aux::pair<direct, typename di::ctor_traits<T>::boost_di_inject__::type>
{ };
template<class T>
struct ctor_traits_impl<T, aux::false_type>
    : di::ctor_traits<T>
{ };
}
template<class T, class>
struct ctor_traits
    : type_traits::ctor<T , type_traits::ctor_impl_t<aux::is_constructible, T>>
{ };
template<>
struct ctor_traits<_> {
    using boost_di_inject__ = aux::type_list<>;
};
template<class T>
struct ctor_traits<std::initializer_list<T>> {
    using boost_di_inject__ = aux::type_list<>;
};
template<class T>
struct ctor_traits<T, BOOST_DI_REQUIRES(
    aux::is_same<std::char_traits<char>, typename T::traits_type>::value)> {
    using boost_di_inject__ = aux::type_list<>;
};
template<class T>
struct ctor_traits<T, BOOST_DI_REQUIRES(!aux::is_class<T>::value)> {
    using boost_di_inject__ = aux::type_list<>;
};
}}}
namespace boost { namespace di { inline namespace v1 { namespace concepts {
template<class...>
struct scope {
    struct is_referable { };
    struct try_create { };
    struct create { };
    template<class...>
    struct requires_ : aux::false_type { };
};
template<class T>
struct provider__ {
    template<class TMemory = type_traits::heap>
    aux::conditional_t<aux::is_same<TMemory, type_traits::stack>::value, T, T*>
    try_get(const TMemory& = {}) const;
    template<class TMemory = type_traits::heap>
    T* get(const TMemory& = {}) const {
        return nullptr;
    }
};
template<class T>
typename scope<T>::template requires_<
    typename scope<_, _>::is_referable
  , typename scope<_, _>::try_create
  , typename scope<_, _>::create
> scopable_impl(...);
template<class T>
auto scopable_impl(T&&) -> aux::is_valid_expr<
    typename T::template scope<_, _>::template is_referable<_>
  , decltype(T::template scope<_, _>::template try_create<_>(provider__<_>{}))
  , decltype(aux::declval<typename T::template scope<_, _>>().template create<_>(provider__<_>{}))
>;
template<class T>
struct scopable__ {
    using type = decltype(scopable_impl<T>(aux::declval<T>()));
};
template<class T>
using scopable = typename scopable__<T>::type;
}}}}
namespace boost { namespace di { inline namespace v1 { namespace core {
template<class T, class... TArgs> decltype(::boost::di::aux::declval<T>().configure( ::boost::di::aux::declval<TArgs>()...) , ::boost::di::aux::true_type()) has_configure_impl(int); template<class, class...> ::boost::di::aux::false_type has_configure_impl(...); template<class T, class... TArgs> struct has_configure : decltype(has_configure_impl<T, TArgs...>(0)) { };
template<class, class = int> struct has_deps : ::boost::di::aux::false_type { }; template<class T> struct has_deps<T, typename ::boost::di::aux::valid_t<typename T::deps>::type> : ::boost::di::aux::true_type { };
template<class>
struct array_type;
template<class T>
struct array_type<T*[]> {
    using type = T*[];
};
template<class T>
struct array_type<T[]> {
    using type = T*[];
};
template<class T>
using array_type_t = typename array_type<T>::type;
template<class T, class U = aux::remove_reference_t<T>>
struct is_injector
    : aux::integral_constant<bool, has_deps<U>::value || has_configure<U>::value>
{ };
template<class, class>
struct dependency_concept { };
template<class T, class TDependency>
struct dependency_impl
    : aux::pair<T, TDependency>
{ };
template<class... Ts, class TName, class TDependency>
struct dependency_impl<
    dependency_concept<concepts::any_of<Ts...>, TName>
  , TDependency
> : aux::pair<dependency_concept<Ts, TName>, TDependency>...
{ };
struct override { };
struct dependency_base { };
template<
    class TScope
  , class TExpected
  , class TGiven = TExpected
  , class TName = no_name
  , class TPriority = aux::none_type
  , class TBase = TExpected
> class dependency : dependency_base, TScope::template scope<TExpected, TGiven>
    , public dependency_impl<
          dependency_concept<TBase, TName>
        , dependency<TScope, TExpected, TGiven, TName, TPriority, TBase>
      > {
    template<class, class, class, class, class, class> friend class dependency;
    using scope_t = typename TScope::template scope<TExpected, TGiven>;
    template<class T>
    using externable = aux::integral_constant<bool,
        !is_injector<T>::value &&
        aux::is_same<TScope, scopes::deduce>::value &&
        aux::is_same<TExpected, TGiven>::value
    >;
    template<class T>
    using specific = aux::integral_constant<bool,
       is_injector<T>::value ||
       aux::is_array<TExpected, T>::value
    >;
    template<class T>
    struct ref_traits {
        using type = T;
    };
    template<int N>
    struct ref_traits<const char(&)[N]> {
        using type = TExpected;
    };
    template<class T>
    struct ref_traits<std::shared_ptr<T>&> {
        using type = std::shared_ptr<T>;
    };
public:
    using scope = TScope;
    using expected = TExpected;
    using given = TGiven;
    using name = TName;
    using priority = TPriority;
    using base = TBase;
    dependency() noexcept { }
    template<class T>
    explicit dependency(T&& object) noexcept
        : scope_t(static_cast<T&&>(object))
    { }
    template<
        class TScope_
      , class TExpected_
      , class TGiven_
      , class TName_
      , class TPriority_
      , class TBase_
    > dependency(const dependency<TScope_, TExpected_, TGiven_, TName_, TPriority_, TBase_>& other) noexcept
        : scope_t(other)
    { }
    template<class T, BOOST_DI_REQUIRES(aux::is_same<TName, no_name>::value && !aux::is_same<T, no_name>::value) = 0>
    auto named() const noexcept {
        return dependency<
            TScope
          , TExpected
          , TGiven
          , T
          , TPriority
          , TBase
        >{*this};
    }
    template<class T, BOOST_DI_REQUIRES(aux::is_same<TName, no_name>::value && !aux::is_same<T, no_name>::value) = 0>
    auto named(const T&) const noexcept {
        return dependency<
            TScope
          , TExpected
          , TGiven
          , T
          , TPriority
          , TBase
        >{*this};
    }
    template<class T, BOOST_DI_REQUIRES_MSG(concepts::scopable<T>) = 0>
    auto in(const T&) const noexcept {
        return dependency<
            T
          , TExpected
          , TGiven
          , TName
          , TPriority
          , TBase
        >{};
    }
    template<class T, BOOST_DI_REQUIRES(!specific<T>::value) = 0, BOOST_DI_REQUIRES_MSG(typename concepts::boundable__<TExpected, T>::type) = 0>
    auto to() const noexcept {
        return dependency<
            TScope
          , TExpected
          , T
          , TName
          , TPriority
          , TBase
        >{};
    }
    template<class... Ts, BOOST_DI_REQUIRES(aux::is_array<TExpected, Ts...>::value) = 0>
    auto to() const noexcept {
        return dependency<
            TScope
          , array<array_type_t<TExpected>, Ts...>
          , array<array_type_t<TExpected>, Ts...>
          , TName
          , TPriority
          , array<array_type_t<TExpected>>
        >{};
    }
    template<class T, BOOST_DI_REQUIRES(externable<T>::value && !aux::is_narrowed<TExpected, T>::value || aux::is_same<TExpected, _>::value) = 0>
    auto to(T&& object) const noexcept {
        using dependency = dependency<
            scopes::external
          , aux::conditional_t<aux::is_same<TExpected, _>::value, typename ref_traits<T>::type, TExpected>
          , typename ref_traits<T>::type
          , TName
          , TPriority
          , TBase
        >;
        return dependency{static_cast<T&&>(object)};
    }
    template<class T, BOOST_DI_REQUIRES(has_configure<T>::value) = 0>
    auto to(const T& object = {}) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>
          , TExpected
          , decltype(aux::declval<T>().configure())
          , TName
          , TPriority
          , TBase
        >;
        return dependency{object.configure()};
    }
    template<class T, BOOST_DI_REQUIRES(has_deps<T>::value) = 0>
    auto to(const T& object = {}) const noexcept {
        using dependency = dependency<
            scopes::exposed<TScope>
          , TExpected
          , T
          , TName
          , TPriority
          , TBase
        >;
        return dependency{object};
    }
    template<class T>
    auto to(std::initializer_list<T>&& object) const noexcept {
        using dependency = dependency<
            scopes::external
          , TExpected
          , std::initializer_list<T>
          , TName
          , TPriority
          , array<array_type_t<TExpected>>
        >;
        return dependency{object};
    }
    auto operator[](const override&) const noexcept {
        return dependency<
            TScope
          , TExpected
          , TGiven
          , TName
          , override
        >{*this};
    }
    #if defined(__cpp_variable_templates)
        const dependency& operator()() const noexcept {
            return *this;
        }
    #endif
protected:
    using scope_t::is_referable;
    using scope_t::create;
    using scope_t::try_create;
    template<class> static void try_create(...);
};
template<class T>
struct is_dependency : aux::is_base_of<dependency_base, T> { };
}}}}
namespace boost { namespace di { inline namespace v1 { namespace concepts {
struct call_operator { };
template<class>
struct policy {
    template<class>
    struct requires_ : aux::false_type { };
};
struct arg {
    using type = void;
    using name = no_name;
    using is_root = aux::false_type;
    template<class, class, class>
    struct resolve;
};
struct ctor { };
aux::false_type callable_impl(...);
template<class T, class TArg>
auto callable_impl(T&& t, TArg&& arg) -> aux::is_valid_expr<
    decltype(t(arg))
>;
template<class T, class TArg, class TDependency, class... TCtor>
auto callable_impl(T&& t, TArg&& arg, TDependency&& dep, TCtor&&... ctor) -> aux::is_valid_expr<
    decltype(t(arg, dep, ctor...))
>;
template<class...>
struct is_callable_impl;
template<class T, class... Ts>
struct is_callable_impl<T, Ts...> {
    using callable_with_arg = decltype(callable_impl(aux::declval<T>(), arg{}));
    using callable_with_arg_and_dep =
        decltype(callable_impl(aux::declval<T>(), arg{}, core::dependency<scopes::deduce, T>{}, ctor{}));
    using type = aux::conditional_t<
        callable_with_arg::value || callable_with_arg_and_dep::value
      , typename is_callable_impl<Ts...>::type
      , typename policy<T>::template requires_<call_operator>
    >;
};
template<>
struct is_callable_impl<>
    : aux::true_type
{ };
template<class... Ts>
struct is_callable
    : is_callable_impl<Ts...>
{ };
template<class... Ts>
struct is_callable<core::pool<aux::type_list<Ts...>>>
    : is_callable_impl<Ts...>
{ };
template<>
struct is_callable<void> {
    using type = policy<void>::requires_<call_operator>;
};
template<class... Ts>
using callable = typename is_callable<Ts...>::type;
}}}}
namespace boost { namespace di { inline namespace v1 { namespace type_traits {
template<class T>
struct cast {
    using type = T&&;
};
template<template<class...> class T, class... Ts>
struct cast<T<aux::false_type, Ts...>> {
    using type = typename T<aux::false_type, Ts...>::value_type;
};
template<class T>
using cast_t = typename cast<T>::type;
template<class T, class>
struct typename_traits {
    using type = T;
};
template<class T>
struct typename_traits<_, T> {
    using type = T;
};
template<class T>
struct typename_traits<_&, T> {
    using type = T&;
};
template<class T>
struct typename_traits<const _&, T> {
    using type = const T&;
};
template<class T>
struct typename_traits<_&&, T> {
    using type = T&&;
};
template<class T>
struct typename_traits<_*, T> {
    using type = T*;
};
template<class T>
struct typename_traits<const _*, T> {
    using type = const T*;
};
template<class T, template<class...> class TDeleter>
struct typename_traits<std::unique_ptr<_, TDeleter<_>>, T> {
    using type = std::unique_ptr<T, TDeleter<T>>;
};
template<class T, template<class...> class TDeleter>
struct typename_traits<const std::unique_ptr<_, TDeleter<_>>&, T> {
    using type = const std::unique_ptr<T, TDeleter<T>>&;
};
template<class T>
struct typename_traits<std::shared_ptr<_>, T> {
    using type = std::shared_ptr<T>;
};
template<class T>
struct typename_traits<const std::shared_ptr<_>&, T> {
    using type = std::shared_ptr<T>;
};
template<class T>
struct typename_traits<boost::shared_ptr<_>, T> {
    using type = boost::shared_ptr<T>;
};
template<class T>
struct typename_traits<const boost::shared_ptr<_>&, T> {
    using type = boost::shared_ptr<T>;
};
template<class T>
struct typename_traits<std::weak_ptr<_>, T> {
    using type = std::weak_ptr<T>;
};
template<class T>
struct typename_traits<const std::weak_ptr<_>&, T> {
    using type = std::weak_ptr<T>;
};
template<class T, class U>
using typename_traits_t = typename typename_traits<T, U>::type;
template<class, class T>
struct given_traits {
    using type = T;
};
template<class T, class X, class... Ts>
struct given_traits<T, core::array<X[], Ts...>> {
    using type = core::array<T, Ts...>;
};
template<class T, class X, class... Ts>
struct given_traits<const T&, core::array<X[], Ts...>> {
    using type = core::array<T, Ts...>;
};
template<class T, class X, class... Ts>
struct given_traits<T&, core::array<X[], Ts...>> {
    using type = core::array<T, Ts...>;
};
template<class T, class X, class... Ts>
struct given_traits<std::shared_ptr<T>, core::array<X[], Ts...>> {
    using type = core::array<T, Ts...>;
};
template<class T, class U>
using given_traits_t = typename given_traits<T, U>::type;
}}}}
namespace boost { namespace di { inline namespace v1 { namespace core {
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
      , class TBase
    > static decltype(auto)
    resolve_impl(aux::pair<TConcept
               , dependency<TScope, TExpected, TGiven, TName, override, TBase>>* dep) noexcept {
        return static_cast<dependency<TScope, TExpected, TGiven, TName, override, TBase>&>(*dep);
    }
    template<class TDeps, class T, class TName, class TDefault>
    struct resolve__ {
        using dependency = dependency_concept<aux::decay_t<T>, TName>;
        using type = decltype(aux::decval(resolve_impl<TDefault, dependency>((TDeps*)0)));
    };
public:
    template<
        class T
      , class TName = no_name
      , class TDefault = dependency<scopes::deduce, aux::decay_t<T>>
      , class TDeps
    > static decltype(auto) resolve(TDeps* deps) noexcept {
        using dependency = dependency_concept<aux::decay_t<T>, TName>;
        return resolve_impl<TDefault, dependency>(deps);
    }
    template<
        class TDeps
      , class T
      , class TName = no_name
      , class TDefault = dependency<scopes::deduce, aux::decay_t<T>>
    > using resolve_t = typename resolve__<TDeps, T, TName, TDefault>::type;
};
}}}}
namespace boost { namespace di { inline namespace v1 { namespace core {
template<class T, class TParent>
using is_not_same_t = BOOST_DI_REQUIRES(!aux::is_same_or_base_of<T, TParent>::value);
template<class T, class TInjector>
struct is_referable_impl {
    static constexpr auto value =
        dependency__<binder::resolve_t<TInjector, T>>::template is_referable<T>::value;
};
template<class T, class TInjector>
using is_referable_t = BOOST_DI_REQUIRES(is_referable_impl<T, TInjector>::value);
template<class T, class TInjector, class TError>
struct is_creatable_impl {
    static constexpr auto value = injector__<TInjector>::template is_creatable<T>::value;
};
template<class T, class TInjector>
struct is_creatable_impl<T, TInjector, aux::false_type> {
    static constexpr auto value = true;
};
template<class T, class TInjector, class TError>
using is_creatable_t = BOOST_DI_REQUIRES(is_creatable_impl<T, TInjector, TError>::value);
template<class TParent, class TInjector, class TError = aux::false_type>
struct any_type {
    template<class T, class = is_creatable_t<T, TInjector, TError>>
    operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T>{});
    }
    template<class T
           , class = is_referable_t<const T&, TInjector>
           , class = is_creatable_t<const T&, TInjector, TError>>
    operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<const T&>{});
    }
    const TInjector& injector_;
};
template<class TParent, class TInjector, class TError = aux::false_type>
struct any_type_ref {
    template<class T, class = is_creatable_t<T, TInjector, TError>>
    operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T>{});
    }
    #if defined(__GNUC__)
        template<class T
               , class = is_referable_t<T&&, TInjector>
               , class = is_creatable_t<T&&, TInjector, TError>
        > operator T&&() const {
            return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T&&>{});
        }
    #endif
    template<class T
           , class = is_referable_t<T&, TInjector>
           , class = is_creatable_t<T&, TInjector, TError>>
    operator T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T&>{});
    }
    template<class T
           , class = is_referable_t<const T&, TInjector>
           , class = is_creatable_t<const T&, TInjector, TError>>
    operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<const T&>{});
    }
    const TInjector& injector_;
};
template<class TParent, class TInjector, class TError = aux::false_type>
struct any_type_1st {
    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_creatable_t<T, TInjector, TError>>
    operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T>{});
    }
    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<const T&, TInjector>
           , class = is_creatable_t<const T&, TInjector, TError>>
    operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<const T&>{});
    }
    const TInjector& injector_;
};
template<class TParent, class TInjector, class TError = aux::false_type>
struct any_type_1st_ref {
    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_creatable_t<T, TInjector, TError>>
    operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T>{});
    }
    #if defined(__GNUC__)
        template<class T
               , class = is_not_same_t<T, TParent>
               , class = is_referable_t<T&&, TInjector>
               , class = is_creatable_t<T&&, TInjector, TError>>
        operator T&&() const {
            return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T&&>{});
        }
    #endif
    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<T&, TInjector>
           , class = is_creatable_t<T&, TInjector, TError>>
    operator T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<T&>{});
    }
    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<const T&, TInjector>
           , class = is_creatable_t<const T&, TInjector, TError>>
    operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_impl(aux::type<const T&>{});
    }
    const TInjector& injector_;
};
namespace successful {
template<class TParent, class TInjector>
struct any_type {
    template<class T>
    operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T>{});
    }
    template<class T, class = is_referable_t<const T&, TInjector>>
    operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<const T&>{});
    }
    const TInjector& injector_;
};
template<class TParent, class TInjector>
struct any_type_ref {
    template<class T>
    operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T>{});
    }
    #if defined(__GNUC__)
        template<class T, class = is_referable_t<T&&, TInjector>>
        operator T&&() const {
            return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T&&>{});
        }
    #endif
    template<class T, class = is_referable_t<T&, TInjector>>
    operator T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T&>{});
    }
    template<class T, class = is_referable_t<const T&, TInjector>>
    operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<const T&>{});
    }
    const TInjector& injector_;
};
template<class TParent, class TInjector>
struct any_type_1st {
    template<class T, class = is_not_same_t<T, TParent>>
    operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T>{});
    }
    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<const T&, TInjector>>
    operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<const T&>{});
    }
    const TInjector& injector_;
};
template<class TParent, class TInjector>
struct any_type_1st_ref {
    template<class T, class = is_not_same_t<T, TParent>>
    operator T() {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T>{});
    }
    #if defined(__GNUC__)
        template<class T
               , class = is_not_same_t<T, TParent>
               , class = is_referable_t<T&&, TInjector>>
        operator T&&() const {
            return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T&&>{});
        }
    #endif
    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<T&, TInjector>>
    operator T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<T&>{});
    }
    template<class T
           , class = is_not_same_t<T, TParent>
           , class = is_referable_t<const T&, TInjector>>
    operator const T&() const {
        return static_cast<const core::injector__<TInjector>&>(injector_).create_successful_impl(aux::type<const T&>{});
    }
    const TInjector& injector_;
};
}
template<class TParent>
struct any_type_fwd {
    template<class T>
    operator T();
};
template<class TParent>
struct any_type_ref_fwd {
    template<class T>
    operator T();
    template<class T>
    operator T&() const;
    #if defined(__GNUC__)
        template<class T>
        operator T&&() const;
    #endif
    template<class T>
    operator const T&() const;
};
template<class TParent>
struct any_type_1st_fwd {
    template<class T, class = is_not_same_t<T, TParent>>
    operator T();
};
template<class TParent>
struct any_type_1st_ref_fwd {
    template<class T, class = is_not_same_t<T, TParent>>
    operator T();
    template<class T, class = is_not_same_t<T, TParent>>
    operator T&() const;
    #if defined(__GNUC__)
        template<class T, class = is_not_same_t<T, TParent>>
        operator T&&() const;
    #endif
    template<class T, class = is_not_same_t<T, TParent>>
    operator const T&() const;
};
}}}}
namespace boost { namespace di { inline namespace v1 { namespace concepts {
template<class T>
struct abstract_type {
struct is_not_bound {
    operator T*() const {
        using constraint_not_satisfied = is_not_bound;
        return
            constraint_not_satisfied{}.error();
    }
    static inline T*
    error(_ = "type is not bound, did you forget to add: 'di::bind<interface>.to<implementation>()'?");
};
struct is_not_fully_implemented {
    operator T*() const {
        using constraint_not_satisfied = is_not_fully_implemented;
        return
            constraint_not_satisfied{}.error();
    }
    static inline T*
    error(_ = "type is not implemented, did you forget to implement all interface methods?");
};
template<class TName>
struct named {
struct is_not_bound {
    operator T*() const {
        using constraint_not_satisfied = is_not_bound;
        return
            constraint_not_satisfied{}.error();
    }
    static inline T*
    error(_ = "type is not bound, did you forget to add: 'di::bind<interface>.named(name).to<implementation>()'?");
};
struct is_not_fully_implemented {
    operator T*() const {
        using constraint_not_satisfied = is_not_fully_implemented;
        return
            constraint_not_satisfied{}.error();
    }
    static inline T*
    error(_ = "type is not implemented, did you forget to implement all interface methods?");
};
};};
template<class...>
struct type;
template<class TParent, class = no_name>
struct try_create__ {
    template<class T, class = core::is_not_same_t<T, TParent>>
    operator T() { return {}; }
    template<class T, class = core::is_not_same_t<T, TParent>>
    operator T&() const {
        using constraint_not_satisfied =
            typename type<TParent>::template has_not_bound_reference<T&>;
        return
            constraint_not_satisfied{}.error();
    }
};
template<class TParent, class TName, class _>
struct try_create__<TParent, named<TName, _>> {
    template<class T, class = core::is_not_same_t<T, TParent>>
    operator T() { return {}; }
    template<class T, class = core::is_not_same_t<T, TParent>>
    operator T&() const {
        using constraint_not_satisfied =
            typename type<TParent>::template has_not_bound_reference<T&>::template named<TName>;
        return
            constraint_not_satisfied{}.error();
    }
};
template<class T, class TInitialization, class... TArgs, class... TCtor>
struct type<T, type_traits::direct, aux::pair<TInitialization, aux::type_list<TArgs...>>, TCtor...> {
    operator T*() const {
        return new T(try_create__<T, TArgs>{}...);
    }
};
template<class T, class TInitialization, class... TArgs, class... TCtor>
struct type<T, type_traits::uniform, aux::pair<TInitialization, aux::type_list<TArgs...>>, TCtor...> {
    operator T*() const {
        return new T{try_create__<T, TArgs>{}...};
    }
};
template<class T>
struct type<T> {
template<class To>
struct is_not_convertible_to {
    operator To() const {
        using constraint_not_satisfied = is_not_convertible_to;
        return
            constraint_not_satisfied{}.error();
    }
    static inline To
    error(_ = "wrapper is not convertible to requested type, did you mistake the scope?");
};
struct has_not_bound_generic_types {
    operator T() const {
        using constraint_not_satisfied = has_not_bound_generic_types;
        return
            constraint_not_satisfied{}.error();
    }
    static inline T
    error(_ = "generic type is not bound, did you forget to add di::bind<di::_>.to<implementation>()?");
};
template<class TReference>
struct has_not_bound_reference {
    template<class TName>
    struct named {
        static inline TReference
        error(_ = "reference is not bound, did you forget to add `auto value = ...; di::bind<T>.named(name).to(value)`");
    };
    static inline TReference
    error(_ = "reference is not bound, did you forget to add `auto value = ...; di::bind<T>.to(value)`");
};
struct has_ambiguous_number_of_constructor_parameters {
template<int Given> struct given {
template<int Expected> struct expected {
    operator T*() const {
        using constraint_not_satisfied = expected;
        return
            constraint_not_satisfied{}.error();
    }
    static inline T*
    error(_ = "verify BOOST_DI_INJECT_TRAITS or di::ctor_traits");
};};};
struct has_to_many_constructor_parameters {
template<int TMax> struct max {
    operator T*() const {
        using constraint_not_satisfied = max;
        return
            constraint_not_satisfied{}.error();
    }
    static inline T*
    error(_ = "increase BOOST_DI_CFG_CTOR_LIMIT_SIZE value or reduce number of constructor parameters");
};};
};
template<class>
struct ctor_size;
template<class TInit, class... TCtor>
struct ctor_size<aux::pair<TInit, aux::type_list<TCtor...>>>
    : aux::integral_constant<int, sizeof...(TCtor)>
{ };
template<class...>
struct creatable_error_impl;
template<class T>
using ctor_size_t = ctor_size<
    typename type_traits::ctor<
        aux::decay_t<T>
      , type_traits::ctor_impl_t<aux::is_constructible, aux::decay_t<T>>
    >::type
>;
template<class TInitialization, class TName, class I, class T, class... TCtor>
struct creatable_error_impl<TInitialization, TName, I, T, aux::type_list<TCtor...>>
    : aux::conditional_t<
          aux::is_abstract<aux::decay_t<T>>::value
        , aux::conditional_t<
              aux::is_same<I, T>::value
            , aux::conditional_t< aux::is_same<TName, no_name>::value , typename abstract_type<aux::decay_t<T>>::is_not_bound , typename abstract_type<aux::decay_t<T>>::template named<TName>::is_not_bound >
            , aux::conditional_t< aux::is_same<TName, no_name>::value , typename abstract_type<aux::decay_t<T>>::is_not_fully_implemented , typename abstract_type<aux::decay_t<T>>::template named<TName>::is_not_fully_implemented >
          >
        , aux::conditional_t<
              ctor_size_t<T>::value == sizeof...(TCtor)
            , aux::conditional_t<
                  !sizeof...(TCtor)
                , typename type<aux::decay_t<T>>::has_to_many_constructor_parameters::
                      template max<BOOST_DI_CFG_CTOR_LIMIT_SIZE>
                , type<
                      aux::decay_t<T>
                    , TInitialization
                    , typename type_traits::ctor_traits__<aux::decay_t<T>>::type, TCtor...
                  >
              >
            , typename type<aux::decay_t<T>>::has_ambiguous_number_of_constructor_parameters::
                  template given<sizeof...(TCtor)>::
                  template expected<ctor_size_t<T>::value>
          >
      >
{ };
template<class TInit, class T, class... TArgs>
struct creatable {
    static constexpr auto value = aux::is_constructible<T, TArgs...>::value;
};
template<class T, class... TArgs>
struct creatable<type_traits::uniform, T, TArgs...> {
    static constexpr auto value = aux::is_braces_constructible<T, TArgs...>::value;
};
template<class TInitialization, class TName, class I, class T, class... TArgs>
T creatable_error() {
    return creatable_error_impl<TInitialization, TName, I, T, aux::type_list<TArgs...>>{};
}
}}}}
namespace boost { namespace di { inline namespace v1 { namespace providers {
class stack_over_heap {
public:
    template<class TInitialization, class TMemory, class T, class... TArgs>
    struct is_creatable {
        static constexpr auto value =
            concepts::creatable<TInitialization, T, TArgs...>::value;
    };
    template<class, class T, class... TArgs>
    auto get(const type_traits::direct&
           , const type_traits::heap&
           , TArgs&&... args) {
        return new T(static_cast<type_traits::cast_t<TArgs>>(args)...);
    }
    template<class, class T, class... TArgs>
    auto get(const type_traits::uniform&
           , const type_traits::heap&
           , TArgs&&... args) {
        return new T{static_cast<type_traits::cast_t<TArgs>>(args)...};
    }
    template<class, class T, class... TArgs>
    auto get(const type_traits::direct&
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T(static_cast<type_traits::cast_t<TArgs>>(args)...);
    }
    template<class, class T, class... TArgs>
    auto get(const type_traits::uniform&
           , const type_traits::stack&
           , TArgs&&... args) const noexcept {
        return T{static_cast<type_traits::cast_t<TArgs>>(args)...};
    }
};
}}}}
#if defined(BOOST_DI_CFG)
    class BOOST_DI_CFG;
#else
#define BOOST_DI_CFG boost::di::config
#endif
namespace boost { namespace di { inline namespace v1 {
template<class... TPolicies, BOOST_DI_REQUIRES_MSG(concepts::callable<TPolicies...>) = 0>
inline auto make_policies(const TPolicies&... args) noexcept {
    return core::pool_t<TPolicies...>(args...);
}
class config {
public:
    template<class T>
    static auto provider(const T&) noexcept {
        return providers::stack_over_heap{};
    }
    template<class T>
    static auto policies(const T&) noexcept {
        return make_policies();
    }
};
}}}
namespace boost { namespace di { inline namespace v1 { namespace core {
template<class T, class = int>
struct get_deps {
    using type = typename T::deps;
};
template<class T>
struct get_deps<T, BOOST_DI_REQUIRES(has_configure<T>::value)> {
    using result_type = typename aux::function_traits<
        decltype(&T::configure)
    >::result_type;
    using type = typename result_type::deps;
};
template<
    class T
  , class = typename is_injector<T>::type
  , class = typename is_dependency<T>::type
> struct add_type_list;
template<class T, class TAny>
struct add_type_list<T, aux::true_type, TAny> {
    using type = typename get_deps<T>::type;
};
template<class T>
struct add_type_list<T, aux::false_type, aux::true_type> {
    using type = aux::type_list<T>;
};
template<class T>
struct add_type_list<T, aux::false_type, aux::false_type> {
    using type = aux::type_list<dependency<scopes::exposed<>, T>>;
};
#if defined(_MSC_VER)
    template<class... Ts>
    struct bindings : aux::join_t<typename add_type_list<Ts>::type...> { };
    template<class... Ts>
    using bindings_t = typename bindings<Ts...>::type;
#else
    template<class... Ts>
    using bindings_t = aux::join_t<typename add_type_list<Ts>::type...>;
#endif
}}}}
namespace boost { namespace di { inline namespace v1 { namespace concepts {
template<class...>
struct bind {
    template<class TName>
    struct named { struct is_bound_more_than_once : aux::false_type { }; };
    struct is_bound_more_than_once : aux::false_type { };
    struct is_neither_a_dependency_nor_an_injector : aux::false_type { };
    struct has_disallowed_specifiers : aux::false_type { };
    template<class> struct is_not_related_to : aux::false_type { };
};
template<class...>
struct any_of : aux::false_type { };
template<class... TDeps>
struct is_supported : aux::is_same<
   aux::bool_list<aux::always<TDeps>::value...>
 , aux::bool_list<(core::is_injector<TDeps>::value || core::is_dependency<TDeps>::value)...>
> { };
template<class...>
struct get_not_supported;
template<class T>
struct get_not_supported<T> {
    using type = T;
};
template<class T, class... TDeps>
struct get_not_supported<T, TDeps...>
    : aux::conditional<
          core::is_injector<T>::value || core::is_dependency<T>::value
        , typename get_not_supported<TDeps...>::type
        , T
      >
{ };
template<class>
struct is_unique;
template<class T>
struct unique_dependency : aux::pair<
    aux::pair<typename T::base, typename T::name>
  , typename T::priority
> { };
template<class... TDeps>
struct is_unique<aux::type_list<TDeps...>>
    : aux::is_unique<typename unique_dependency<TDeps>::type...>
{ };
template<class>
struct get_is_unique_error_impl
    : aux::true_type
{ };
template<class T, class TName, class TPriority>
struct get_is_unique_error_impl<aux::not_unique<aux::pair<aux::pair<T, TName>, TPriority>>> {
    using type = typename bind<T>::template named<TName>::is_bound_more_than_once;
};
template<class T, class TPriority>
struct get_is_unique_error_impl<aux::not_unique<aux::pair<aux::pair<T, no_name>, TPriority>>> {
    using type = typename bind<T>::is_bound_more_than_once;
};
template<class T>
struct get_is_unique_error_impl<aux::not_unique<T>> {
    using type = typename bind<T>::is_bound_more_than_once;
};
template<class>
struct get_is_unique_error;
template<class... TDeps>
struct get_is_unique_error<aux::type_list<TDeps...>>
    : get_is_unique_error_impl<typename aux::is_unique<typename unique_dependency<TDeps>::type...>::type>
{ };
template<class... TDeps>
using get_bindings_error =
    aux::conditional_t<
        is_supported<TDeps...>::value
      , typename get_is_unique_error<core::bindings_t<TDeps...>>::type
      , typename bind<typename get_not_supported<TDeps...>::type>::is_neither_a_dependency_nor_an_injector
    >;
template<class... Ts>
using get_any_of_error =
    aux::conditional_t<
        aux::is_same<
            aux::bool_list<aux::always<Ts>::value...>
          , aux::bool_list<aux::is_same<aux::true_type, Ts>::value...>
        >::value
      , aux::true_type
      , any_of<Ts...>
    >;
template<bool, class...>
struct is_related {
    static constexpr auto value = true;
};
template<class I, class T>
struct is_related<true, I, T> {
    static constexpr auto value =
        aux::is_base_of<I, T>::value || (
            aux::is_same<_, I>::value || (
                aux::is_convertible<T, I>::value && !aux::is_narrowed<I, T>::value
            )
        );
};
auto boundable_impl(any_of<>&&) -> aux::true_type;
template<class T, class... Ts>
auto boundable_impl(any_of<T, Ts...>&&) ->
    aux::conditional_t<
        aux::is_same<T, aux::remove_specifiers_t<T>>::value
      , decltype(boundable_impl(aux::declval<any_of<Ts...>>()))
      , typename bind<T>::has_disallowed_specifiers
    >;
template<class I, class T>
auto boundable_impl(I&&, T&&) ->
    aux::conditional_t<
        !aux::is_same<T, aux::remove_specifiers_t<T>>::value
      , typename bind<T>::has_disallowed_specifiers
      , aux::conditional_t<
            is_related<aux::is_complete<I>::value && aux::is_complete<T>::value, I, T>::value
          , aux::true_type
          , typename bind<T>::template is_not_related_to<I>
        >
    >;
template<class I, class T>
auto boundable_impl(I[], T&&) -> aux::true_type;
template<class... TDeps>
auto boundable_impl(aux::type_list<TDeps...>&&) -> get_bindings_error<TDeps...>;
template<class T, class... Ts>
auto boundable_impl(concepts::any_of<Ts...>&&, T&&) ->
    get_any_of_error<decltype(boundable_impl(aux::declval<Ts>(), aux::declval<T>()))...>;
template<class... TDeps>
auto boundable_impl(aux::type<TDeps...>&&) ->
    typename get_is_unique_error_impl<typename aux::is_unique<TDeps...>::type>::type;
aux::true_type boundable_impl(...);
template<class... Ts>
struct boundable__ {
    using type = decltype(boundable_impl(aux::declval<Ts>()...));
};
template<class... Ts>
using boundable = typename boundable__<Ts...>::type;
}}}}
namespace boost { namespace di { inline namespace v1 { namespace detail {
template<class...>
struct bind;
template<class TScope, class... Ts>
struct bind<int, TScope, Ts...> {
    using type = core::dependency<TScope, concepts::any_of<Ts...>>;
};
template<class TScope, class T>
struct bind<int, TScope, T> {
    using type = core::dependency<TScope, T>;
};
}
template<class... Ts>
#if !defined(__cpp_variable_templates)
    struct bind :
#else
    typename
#endif
    detail::bind<
        BOOST_DI_REQUIRES_MSG(concepts::boundable<concepts::any_of<Ts...>>)
      , scopes::deduce
      , Ts...
    >::type
#if defined(__cpp_variable_templates)
    bind
#endif
{ };
static constexpr BOOST_DI_UNUSED core::override override{};
static constexpr BOOST_DI_UNUSED scopes::deduce deduce{};
static constexpr BOOST_DI_UNUSED scopes::unique unique{};
static constexpr BOOST_DI_UNUSED scopes::singleton singleton{};
}}}
namespace boost { namespace di { inline namespace v1 { namespace core {
template<class T, class TName, class TIsRoot, class TDeps>
struct arg_wrapper {
    using type BOOST_DI_UNUSED = T;
    using name BOOST_DI_UNUSED = TName;
    using is_root BOOST_DI_UNUSED = TIsRoot;
    template<class T_, class TName_, class TDefault_>
    using resolve = decltype(core::binder::resolve<T_, TName_, TDefault_>((TDeps*)0));
};
template<class T>
struct allow_void : T { };
template<>
struct allow_void<void> : aux::true_type { };
class policy {
    template<class TArg, class TPolicy, class TPolicies, class TDependency, class TCtor>
    static void call_impl(const TPolicies& policies, TDependency& dependency, const TCtor& ctor) noexcept {
        call_impl__<TArg>(static_cast<const TPolicy&>(policies), dependency, ctor);
    }
    template<class TArg, class TPolicy, class TDependency, class TCtor>
    static void call_impl__(const TPolicy& policy, TDependency& dependency, const TCtor& ctor) noexcept {
        call_impl_args<TArg>(policy, dependency, ctor);
    }
    template<class TArg, class TDependency, class TPolicy, class TInitialization, class... TCtor
           , BOOST_DI_REQUIRES(!aux::is_callable_with<TPolicy, TArg, TDependency&, TCtor...>::value) = 0>
    static void call_impl_args(const TPolicy& policy
                             , TDependency&
                             , const aux::pair<TInitialization, aux::type_list<TCtor...>>&) noexcept {
        (policy)(TArg{});
    }
    template<class TArg, class TDependency, class TPolicy, class TInitialization, class... TCtor
           , BOOST_DI_REQUIRES(aux::is_callable_with<TPolicy, TArg, TDependency&, TCtor...>::value) = 0>
    static void call_impl_args(const TPolicy& policy
                             , TDependency& dependency
                             , const aux::pair<TInitialization, aux::type_list<TCtor...>>&) noexcept {
        (policy)(TArg{}, dependency, aux::type<TCtor>{}...);
    }
    template<class, class, class, class, class = int>
    struct try_call_impl;
    template<class TArg, class TPolicy, class TDependency, class TInitialization, class... TCtor>
    struct try_call_impl<TArg, TPolicy, TDependency, aux::pair<TInitialization, aux::type_list<TCtor...>>
                       , BOOST_DI_REQUIRES(!aux::is_callable_with<TPolicy, TArg, TDependency, TCtor...>::value)>
        : allow_void<decltype((aux::declval<TPolicy>())(aux::declval<TArg>()))>
    { };
    template<class TArg, class TPolicy, class TDependency, class TInitialization, class... TCtor>
    struct try_call_impl<TArg, TPolicy, TDependency, aux::pair<TInitialization, aux::type_list<TCtor...>>
                       , BOOST_DI_REQUIRES(aux::is_callable_with<TPolicy, TArg, TDependency, TCtor...>::value)>
        : allow_void<decltype((aux::declval<TPolicy>())(aux::declval<TArg>(), aux::declval<TDependency>(), aux::type<TCtor>{}...))>
    { };
public:
    template<class, class, class, class>
    struct try_call;
    template<class TArg, class TDependency, class TCtor, class... TPolicies>
    struct try_call<TArg, pool_t<TPolicies...>, TDependency, TCtor>
        : aux::is_same<
            aux::bool_list<aux::always<TPolicies>::value...>
          , aux::bool_list<try_call_impl<TArg, TPolicies, TDependency, TCtor>::value...>
        >
    { };
    template<class TArg, class TDependency, class TCtor, class... TPolicies>
    static void call(BOOST_DI_UNUSED const pool_t<TPolicies...>& policies
                   , BOOST_DI_UNUSED TDependency& dependency
                   , BOOST_DI_UNUSED const TCtor& ctor) noexcept {
        int _[]{0, (call_impl<TArg, TPolicies>(policies, dependency, ctor), 0)...}; (void)_;
    }
};
}}}}
namespace boost { namespace di { inline namespace v1 { namespace core {
template<class, class, class, class>
struct try_provider;
template<
    class TGiven
  , class TInjector
  , class TProvider
  , class TInitialization
  , class... TCtor
> struct try_provider<TGiven, aux::pair<TInitialization, aux::type_list<TCtor...>>, TInjector, TProvider> {
    template<class TMemory>
    struct is_creatable {
        static constexpr auto value =
            TProvider::template is_creatable<
                TInitialization
              , TMemory
              , TGiven
              , typename injector__<TInjector>::template try_create<TCtor>::type...
            >::value;
    };
    template<class TMemory = type_traits::heap>
    auto get(const TMemory& = {}) const -> aux::enable_if_t<
        is_creatable<TMemory>::value
      , aux::conditional_t<aux::is_same<TMemory, type_traits::stack>::value, TGiven, aux::remove_reference_t<TGiven>*>
    >;
};
template<class, class, class, class, class>
struct provider;
template<
    class TExpected
  , class TGiven
  , class TName
  , class TInjector
  , class TInitialization
  , class... TCtor
> struct provider<TExpected, TGiven, TName, aux::pair<TInitialization, aux::type_list<TCtor...>>, TInjector> {
    using provider_t = decltype(TInjector::config::provider(aux::declval<TInjector>()));
    template<class TMemory, class... TArgs>
    struct is_creatable {
        static constexpr auto value =
            provider_t::template is_creatable<TInitialization, TMemory, TGiven, TArgs...>::value;
    };
    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        return get_impl(memory, static_cast<const injector__<TInjector>&>(injector_).create_impl(aux::type<TCtor>{})...);
    }
    template<class TMemory, class... TArgs, BOOST_DI_REQUIRES(is_creatable<TMemory, TArgs...>::value) = 0>
    auto get_impl(const TMemory& memory, TArgs&&... args) const {
        return TInjector::config::provider(injector_).template get<TExpected, TGiven>(
            TInitialization{}
          , memory
          , static_cast<TArgs&&>(args)...
        );
    }
    template<class TMemory, class... TArgs, BOOST_DI_REQUIRES(!is_creatable<TMemory, TArgs...>::value) = 0>
    auto get_impl(const TMemory&, TArgs&&...) const {
        return concepts::creatable_error<TInitialization, TName, TExpected*, TGiven*, TArgs...>();
    }
    const TInjector& injector_;
};
namespace successful {
template<class, class, class, class>
struct provider;
template<
    class TExpected
  , class TGiven
  , class TInjector
  , class TInitialization
  , class... TCtor
> struct provider<TExpected, TGiven, aux::pair<TInitialization, aux::type_list<TCtor...>>, TInjector> {
    template<class TMemory = type_traits::heap>
    auto get(const TMemory& memory = {}) const {
        return TInjector::config::provider(injector_).template get<TExpected, TGiven>(
            TInitialization{}
          , memory
          , static_cast<const injector__<TInjector>&>(injector_).create_successful_impl(aux::type<TCtor>{})...
        );
    }
    const TInjector& injector_;
};
}
}}}}
namespace boost { namespace di { inline namespace v1 { namespace core {
namespace successful {
template<class, class T, class TWrapper>
struct wrapper {
    using value_type = T;
    inline operator T() const noexcept {
        return BOOST_DI_TYPE_WKND(T)wrapper_;
    }
    inline operator T() noexcept {
        return BOOST_DI_TYPE_WKND(T)wrapper_;
    }
    T operator*() const {
        return BOOST_DI_TYPE_WKND(T)wrapper_;
    }
    TWrapper wrapper_;
};
}
template<class, class T, class TWrapper, class = int>
struct wrapper_impl {
    using value_type = T;
    inline operator T() const noexcept {
        return wrapper_;
    }
    inline operator T() noexcept {
        return wrapper_;
    }
    TWrapper wrapper_;
};
template<class T, class TWrapper>
struct wrapper_impl<aux::true_type, T, TWrapper, BOOST_DI_REQUIRES(!aux::is_convertible<TWrapper, T>::value)> {
    using value_type = T;
    inline operator T() const noexcept {
        return typename concepts::type<TWrapper>::template is_not_convertible_to<T>{};
    }
    inline operator T() noexcept {
        return typename concepts::type<TWrapper>::template is_not_convertible_to<T>{};
    }
    TWrapper wrapper_;
};
template<class TCast, class T, class TWrapper>
using wrapper = wrapper_impl<TCast, T, TWrapper>;
}}}}
namespace boost { namespace di { inline namespace v1 { namespace core {
template<class T, class... TArgs> decltype(::boost::di::aux::declval<T>().call( ::boost::di::aux::declval<TArgs>()...) , ::boost::di::aux::true_type()) has_call_impl(int); template<class, class...> ::boost::di::aux::false_type has_call_impl(...); template<class T, class... TArgs> struct has_call : decltype(has_call_impl<T, TArgs...>(0)) { };
struct from_injector { };
struct from_deps { };
struct init { };
struct with_error { };
template<class>
struct copyable;
template<class T>
struct copyable_impl : aux::conditional<
    aux::is_default_constructible<
        typename T::scope::template scope<
            typename T::expected, typename T::given>
    >::value
  , aux::type_list<>
  , aux::type_list<T>
> { };
template<class... TDeps>
struct copyable<aux::type_list<TDeps...>>
    : aux::join<typename copyable_impl<TDeps>::type...>
{ };
template<class TDeps>
using copyable_t = typename copyable<TDeps>::type;
template<class T, class>
struct referable {
    using type = T;
};
template<class T, class TDependency>
struct referable<T&, TDependency> {
    using type = aux::conditional_t<TDependency::template is_referable<T&>::value, T&, T>;
};
template<class T, class TDependency>
struct referable<const T&, TDependency> {
    using type = aux::conditional_t<TDependency::template is_referable<const T&>::value, const T&, T>;
};
#if defined(_MSC_VER)
    template<class T, class TDependency>
    struct referable<T&&, TDependency> {
        using type = aux::conditional_t<TDependency::template is_referable<T&&>::value, T&&, T>;
    };
#endif
template<class T, class TDependency>
using referable_t = typename referable<T, TDependency>::type;
#if defined(_MSC_VER)
    template<class T, class TInjector>
    inline auto build(const TInjector& injector) noexcept {
        return T{injector};
    }
#endif
template<class T>
inline decltype(auto) get_arg(const T& arg, const aux::false_type&) noexcept {
    return arg;
}
template<class T>
inline decltype(auto) get_arg(const T& arg, const aux::true_type&) noexcept {
    return arg.configure();
}
template<class TConfig , class TPolicies = pool<> , class... TDeps>
class injector : pool<bindings_t<TDeps...>> {
    friend class binder; template<class> friend struct pool;
    using pool_t = pool<bindings_t<TDeps...>>;
protected:
    template<class T, class TName = no_name, class TIsRoot = aux::false_type>
    struct is_creatable {
        using dependency_t = binder::resolve_t<injector, T, TName>;
        using given_t = type_traits::given_traits_t<T, typename dependency_t::given>;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using type = aux::conditional_t<aux::is_same<_, given_t>::value, void, type_traits::typename_traits_t<T, given_t>>;
        static constexpr auto value = aux::is_convertible<
            decltype(
                dependency__<dependency_t>::template try_create<type>(
                    try_provider<
                        given_t
                      , ctor_t
                      , injector
                      , decltype(TConfig::provider(aux::declval<injector>()))
                    >{}
                )
            ), type>::value && policy::template try_call< arg_wrapper<referable_t<type, dependency__<dependency_t>>, TName, TIsRoot, pool_t> , TPolicies , dependency_t , ctor_t >::value ;
    };
public:
    using deps = bindings_t<TDeps...>;
    using config = TConfig;
    template<class... TArgs>
    explicit injector(const init&, const TArgs&... args) noexcept
        : injector{from_deps{}, get_arg(args, has_configure<decltype(args)>{})...}
    { }
    template<class TConfig_, class TPolicies_, class... TDeps_>
    explicit injector(const injector<TConfig_, TPolicies_, TDeps_...>& other) noexcept
        : injector{from_injector{}, other, deps{}}
    { }
    template<class T, BOOST_DI_REQUIRES(is_creatable<T, no_name, aux::true_type>::value) = 0>
    T create() const {
        return BOOST_DI_TYPE_WKND(T)create_successful_impl<aux::true_type>(aux::type<T>{});
    }
    template<class T, BOOST_DI_REQUIRES(!is_creatable<T, no_name, aux::true_type>::value) = 0>
    BOOST_DI_DEPRECATED("creatable constraint not satisfied")
    T create() const {
        return BOOST_DI_TYPE_WKND(T)create_impl<aux::true_type>(aux::type<T>{});
    }
    template<class T, BOOST_DI_REQUIRES(!has_deps<T>::value) = 0>
    operator T() const {
        return create<T>();
    }
protected:
    template<class T, bool = false>
    struct try_create {
        using type = aux::conditional_t<is_creatable<T>::value, typename is_creatable<T>::type, void>;
    };
    template<class TParent, bool B>
    struct try_create<any_type_fwd<TParent>, B> {
        using type = any_type<TParent, injector, with_error>;
    };
    template<class TParent, bool B>
    struct try_create<any_type_ref_fwd<TParent>, B> {
        using type = any_type_ref<TParent, injector, with_error>;
    };
    template<class TParent, bool B>
    struct try_create<any_type_1st_fwd<TParent>, B> {
        using type = any_type_1st<TParent, injector, with_error>;
    };
    template<class TParent, bool B>
    struct try_create<any_type_1st_ref_fwd<TParent>, B> {
        using type = any_type_1st_ref<TParent, injector, with_error>;
    };
    template<class TName, class T, bool B>
    struct try_create<di::named<TName, T>, B> {
        using type = aux::conditional_t<is_creatable<T, TName>::value, typename is_creatable<T, TName>::type, void>;
    };
    template<bool B>
    struct try_create<self, B> {
        using type = injector;
    };
    template<class TIsRoot = aux::false_type, class T>
    auto create_impl(const aux::type<T>&) const {
        return create_impl__<TIsRoot, T>();
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_fwd<TParent>>&) const {
        return any_type<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_ref_fwd<TParent>>&) const {
        return any_type_ref<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_1st_fwd<TParent>>&) const {
        return any_type_1st<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_1st_ref_fwd<TParent>>&) const {
        return any_type_1st_ref<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class T, class TName>
    auto create_impl(const aux::type<di::named<TName, T>>&) const {
        return create_impl__<TIsRoot, T, TName>();
    }
    template<class TIsRoot = aux::false_type, class T>
    auto create_successful_impl(const aux::type<T>&) const {
        return create_successful_impl__<TIsRoot, T>();
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_fwd<TParent>>&) const {
        return successful::any_type<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_ref_fwd<TParent>>&) const {
        return successful::any_type_ref<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_1st_fwd<TParent>>&) const {
        return successful::any_type_1st<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_1st_ref_fwd<TParent>>&) const {
        return successful::any_type_1st_ref<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class T, class TName>
    auto create_successful_impl(const aux::type<di::named<TName, T>>&) const {
        return create_successful_impl__<TIsRoot, T, TName>();
    }
    template<class TIsRoot = aux::false_type>
    auto create_successful_impl(const aux::type<self>&) const {
        return *this;
    }
private:
    template<class... TArgs>
    explicit injector(const from_deps&, const TArgs&... args) noexcept
        : pool_t{copyable_t<deps>{}, core::pool_t<TArgs...>{args...}}
    { }
    template<class TInjector, class... TArgs>
    explicit injector(const from_injector&, const TInjector& injector, const aux::type_list<TArgs...>&) noexcept
    #if defined(_MSC_VER)
        : pool_t{copyable_t<deps>{}, pool_t{build<TArgs>(injector)...}}
    #else
        : pool_t{copyable_t<deps>{}, pool_t{TArgs{injector}...}}
    #endif
    { }
    template<class TIsRoot = aux::false_type, class T, class TName = no_name>
    auto create_impl__() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = aux::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = type_traits::given_traits_t<T, typename dependency_t::given>;
        using type_t = type_traits::typename_traits_t<T, given_t>;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using provider_t = core::provider<expected_t, given_t, TName, ctor_t, injector>;
        using wrapper_t = decltype(static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this}));
        using create_t = referable_t<type_t, dependency__<dependency_t>>;
        policy::template call<arg_wrapper<create_t, TName, TIsRoot, pool_t>>( TConfig::policies(*this), dependency, ctor_t{} );
        return wrapper<typename aux::is_same<type_t, T>::type, create_t, wrapper_t>{
            static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this})
        };
    }
    template<class TIsRoot = aux::false_type, class T, class TName = no_name>
    auto create_successful_impl__() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = aux::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = type_traits::given_traits_t<T, typename dependency_t::given>;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using type_t = type_traits::typename_traits_t<T, given_t>;
        using provider_t = successful::provider<expected_t, given_t, ctor_t, injector>;
        using wrapper_t = decltype(static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this}));
        using create_t = referable_t<type_t, dependency__<dependency_t>>;
        policy::template call<arg_wrapper<create_t, TName, TIsRoot, pool_t>>( TConfig::policies(*this), dependency, ctor_t{} );
        return successful::wrapper<typename aux::is_same<type_t, T>::type, create_t, wrapper_t>{
            static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this})
        };
    }
};
template<class TConfig , class... TDeps>
class injector <TConfig, pool<>, TDeps...> : pool<bindings_t<TDeps...>> {
    friend class binder; template<class> friend struct pool;
    using pool_t = pool<bindings_t<TDeps...>>;
protected:
    template<class T, class TName = no_name, class TIsRoot = aux::false_type>
    struct is_creatable {
        using dependency_t = binder::resolve_t<injector, T, TName>;
        using given_t = type_traits::given_traits_t<T, typename dependency_t::given>;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using type = aux::conditional_t<aux::is_same<_, given_t>::value, void, type_traits::typename_traits_t<T, given_t>>;
        static constexpr auto value = aux::is_convertible<
            decltype(
                dependency__<dependency_t>::template try_create<type>(
                    try_provider<
                        given_t
                      , ctor_t
                      , injector
                      , decltype(TConfig::provider(aux::declval<injector>()))
                    >{}
                )
            ), type>::value ;
    };
public:
    using deps = bindings_t<TDeps...>;
    using config = TConfig;
    template<class... TArgs>
    explicit injector(const init&, const TArgs&... args) noexcept
        : injector{from_deps{}, get_arg(args, has_configure<decltype(args)>{})...}
    { }
    template<class TConfig_, class TPolicies_, class... TDeps_>
    explicit injector(const injector<TConfig_, TPolicies_, TDeps_...>& other) noexcept
        : injector{from_injector{}, other, deps{}}
    { }
    template<class T, BOOST_DI_REQUIRES(is_creatable<T, no_name, aux::true_type>::value) = 0>
    T create() const {
        return BOOST_DI_TYPE_WKND(T)create_successful_impl<aux::true_type>(aux::type<T>{});
    }
    template<class T, BOOST_DI_REQUIRES(!is_creatable<T, no_name, aux::true_type>::value) = 0>
    BOOST_DI_DEPRECATED("creatable constraint not satisfied")
    T create() const {
        return BOOST_DI_TYPE_WKND(T)create_impl<aux::true_type>(aux::type<T>{});
    }
    template<class T, BOOST_DI_REQUIRES(!has_deps<T>::value) = 0>
    operator T() const {
        return create<T>();
    }
protected:
    template<class T, bool = false>
    struct try_create {
        using type = aux::conditional_t<is_creatable<T>::value, typename is_creatable<T>::type, void>;
    };
    template<class TParent, bool B>
    struct try_create<any_type_fwd<TParent>, B> {
        using type = any_type<TParent, injector, with_error>;
    };
    template<class TParent, bool B>
    struct try_create<any_type_ref_fwd<TParent>, B> {
        using type = any_type_ref<TParent, injector, with_error>;
    };
    template<class TParent, bool B>
    struct try_create<any_type_1st_fwd<TParent>, B> {
        using type = any_type_1st<TParent, injector, with_error>;
    };
    template<class TParent, bool B>
    struct try_create<any_type_1st_ref_fwd<TParent>, B> {
        using type = any_type_1st_ref<TParent, injector, with_error>;
    };
    template<class TName, class T, bool B>
    struct try_create<di::named<TName, T>, B> {
        using type = aux::conditional_t<is_creatable<T, TName>::value, typename is_creatable<T, TName>::type, void>;
    };
    template<bool B>
    struct try_create<self, B> {
        using type = injector;
    };
    template<class TIsRoot = aux::false_type, class T>
    auto create_impl(const aux::type<T>&) const {
        return create_impl__<TIsRoot, T>();
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_fwd<TParent>>&) const {
        return any_type<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_ref_fwd<TParent>>&) const {
        return any_type_ref<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_1st_fwd<TParent>>&) const {
        return any_type_1st<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_impl(const aux::type<any_type_1st_ref_fwd<TParent>>&) const {
        return any_type_1st_ref<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class T, class TName>
    auto create_impl(const aux::type<di::named<TName, T>>&) const {
        return create_impl__<TIsRoot, T, TName>();
    }
    template<class TIsRoot = aux::false_type, class T>
    auto create_successful_impl(const aux::type<T>&) const {
        return create_successful_impl__<TIsRoot, T>();
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_fwd<TParent>>&) const {
        return successful::any_type<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_ref_fwd<TParent>>&) const {
        return successful::any_type_ref<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_1st_fwd<TParent>>&) const {
        return successful::any_type_1st<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class TParent>
    auto create_successful_impl(const aux::type<any_type_1st_ref_fwd<TParent>>&) const {
        return successful::any_type_1st_ref<TParent, injector>{*this};
    }
    template<class TIsRoot = aux::false_type, class T, class TName>
    auto create_successful_impl(const aux::type<di::named<TName, T>>&) const {
        return create_successful_impl__<TIsRoot, T, TName>();
    }
    template<class TIsRoot = aux::false_type>
    auto create_successful_impl(const aux::type<self>&) const {
        return *this;
    }
private:
    template<class... TArgs>
    explicit injector(const from_deps&, const TArgs&... args) noexcept
        : pool_t{copyable_t<deps>{}, core::pool_t<TArgs...>{args...}}
    { }
    template<class TInjector, class... TArgs>
    explicit injector(const from_injector&, const TInjector& injector, const aux::type_list<TArgs...>&) noexcept
    #if defined(_MSC_VER)
        : pool_t{copyable_t<deps>{}, pool_t{build<TArgs>(injector)...}}
    #else
        : pool_t{copyable_t<deps>{}, pool_t{TArgs{injector}...}}
    #endif
    { }
    template<class TIsRoot = aux::false_type, class T, class TName = no_name>
    auto create_impl__() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = aux::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = type_traits::given_traits_t<T, typename dependency_t::given>;
        using type_t = type_traits::typename_traits_t<T, given_t>;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using provider_t = core::provider<expected_t, given_t, TName, ctor_t, injector>;
        using wrapper_t = decltype(static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this}));
        using create_t = referable_t<type_t, dependency__<dependency_t>>;
        return wrapper<typename aux::is_same<type_t, T>::type, create_t, wrapper_t>{
            static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this})
        };
    }
    template<class TIsRoot = aux::false_type, class T, class TName = no_name>
    auto create_successful_impl__() const {
        auto&& dependency = binder::resolve<T, TName>((injector*)this);
        using dependency_t = aux::remove_reference_t<decltype(dependency)>;
        using expected_t = typename dependency_t::expected;
        using given_t = type_traits::given_traits_t<T, typename dependency_t::given>;
        using ctor_t = typename type_traits::ctor_traits__<given_t>::type;
        using type_t = type_traits::typename_traits_t<T, given_t>;
        using provider_t = successful::provider<expected_t, given_t, ctor_t, injector>;
        using wrapper_t = decltype(static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this}));
        using create_t = referable_t<type_t, dependency__<dependency_t>>;
        return successful::wrapper<typename aux::is_same<type_t, T>::type, create_t, wrapper_t>{
            static_cast<dependency__<dependency_t>&&>(dependency).template create<type_t>(provider_t{*this})
        };
    }
};
}}
}}
namespace boost { namespace di { inline namespace v1 { namespace concepts {
struct get { };
struct is_creatable { };
template<class>
struct provider {
    template<class...>
    struct requires_ : aux::false_type { };
};
template<class T>
typename provider<T>::template requires_<get, is_creatable> providable_impl(...);
template<class T>
auto providable_impl(T&& t) -> aux::is_valid_expr<
    decltype(t.template get<_, _>(type_traits::direct{}, type_traits::heap{}))
  , decltype(t.template get<_, _>(type_traits::direct{}, type_traits::heap{}, int{}))
  , decltype(t.template get<_, _>(type_traits::uniform{}, type_traits::stack{}))
  , decltype(t.template get<_, _>(type_traits::uniform{}, type_traits::stack{}, int{}))
  , decltype(T::template is_creatable<type_traits::direct, type_traits::heap, _>::value)
  , decltype(T::template is_creatable<type_traits::uniform, type_traits::stack, _, int>::value)
>;
template<class T>
struct providable__ {
    using type = decltype(providable_impl<T>(aux::declval<T>()));
};
template<class T>
using providable = typename providable__<T>::type;
}}}}
namespace boost { namespace di { inline namespace v1 { namespace concepts {
template<class> struct policies { };
struct providable_type { };
struct callable_type { };
template<class>
struct config {
    template<class...>
    struct requires_ : aux::false_type { };
};
aux::false_type configurable_impl(...);
template<class T>
auto configurable_impl(T&& t) -> aux::is_valid_expr<
    decltype(T::provider(static_cast<const T&>(t)))
  , decltype(T::policies(static_cast<const T&>(t)))
>;
template<class T1, class T2>
struct get_configurable_error
    : aux::type_list<T1, T2>
{ };
template<class T>
struct get_configurable_error<aux::true_type, T> {
    using type = T;
};
template<class T>
struct get_configurable_error<T, aux::true_type> {
    using type = T;
};
template<>
struct get_configurable_error<aux::true_type, aux::true_type>
    : aux::true_type
{ };
template<class T>
auto is_configurable(const aux::true_type&) {
    return typename get_configurable_error<
        decltype(providable<decltype(T::provider(aux::declval<T>()))>())
      , decltype(callable<decltype(T::policies(aux::declval<T>()))>())
    >::type{};
}
template<class T>
auto is_configurable(const aux::false_type&) {
    return typename config<T>::template requires_<provider<providable_type(...)>, policies<callable_type(...)>>{};
}
template<class T>
struct configurable__ {
    using type = decltype(is_configurable<T>(decltype(configurable_impl(aux::declval<T>())){}));
};
template<class T>
using configurable = typename configurable__<T>::type;
}}}}
namespace boost { namespace di { inline namespace v1 { namespace detail {
template<class>
void create(const aux::true_type&) { }
template<class>
BOOST_DI_DEPRECATED("creatable constraint not satisfied")
void
    create
(const aux::false_type&) { }
template<class, class...>
struct injector;
template<class... T>
struct injector<int, T...> : core::injector<::BOOST_DI_CFG, core::pool<>, T...> {
    template<class TConfig, class TPolicies, class... TDeps>
    injector(const core::injector<TConfig, TPolicies, TDeps...>& injector) noexcept
        : core::injector<::BOOST_DI_CFG, core::pool<>, T...>(injector) {
            using injector_t = core::injector<TConfig, TPolicies, TDeps...>;
            int _[]{0, (
                detail::create<T>(
                    aux::integral_constant<bool,
                        core::injector__<injector_t>::template is_creatable<T>::value ||
                        core::injector__<injector_t>::template is_creatable<T*>::value
                    >{}
                )
            , 0)...}; (void)_;
    }
};
}
template<class... T>
using injector = detail::injector<BOOST_DI_REQUIRES_MSG(concepts::boundable<aux::type<T...>>), T...>;
}}}
namespace boost { namespace di { inline namespace v1 {
template<
     class TConfig = ::BOOST_DI_CFG
   , class... TDeps
   , BOOST_DI_REQUIRES_MSG(concepts::boundable<aux::type_list<TDeps...>>) = 0
   , BOOST_DI_REQUIRES_MSG(concepts::configurable<TConfig>) = 0
> inline auto make_injector(const TDeps&... args) noexcept {
    return core::injector<TConfig, decltype(((TConfig*)0)->policies(0)), TDeps...>{core::init{}, args...};
}
}}}
namespace boost { namespace di { inline namespace v1 { namespace policies { namespace detail {
struct type_op { };
template<class T, class = int>
struct apply_impl {
    template<class>
    struct apply : T { };
};
template<template<class...> class T, class... Ts>
struct apply_impl<T<Ts...>, BOOST_DI_REQUIRES(!aux::is_base_of<type_op, T<Ts...>>::value)> {
    template<class TOp, class>
    struct apply_placeholder_impl {
        using type = TOp;
    };
    template<class TOp>
    struct apply_placeholder_impl<_, TOp> {
        using type = TOp;
    };
    template<template<class...> class TExpr, class TOp, class... TArgs>
    struct apply_placeholder {
        using type = TExpr<typename apply_placeholder_impl<TArgs, TOp>::type...>;
    };
    template<class TArg>
    struct apply : apply_placeholder<T, typename TArg::type, Ts...>::type
    { };
};
template<class T>
struct apply_impl<T, BOOST_DI_REQUIRES(aux::is_base_of<type_op, T>::value)> {
    template<class TArg>
    struct apply : T::template apply<TArg>::type { };
};
template<class T>
struct not_ : detail::type_op {
    template<class TArg>
    struct apply : aux::integral_constant<bool,
        !detail::apply_impl<T>::template apply<TArg>::value
    > { };
};
template<class... Ts>
struct and_ : detail::type_op {
    template<class TArg>
    struct apply : aux::is_same<
        aux::bool_list<detail::apply_impl<Ts>::template apply<TArg>::value...>
      , aux::bool_list<aux::always<Ts>::value...>
    > { };
};
template<class... Ts>
struct or_ : detail::type_op {
    template<class TArg>
    struct apply : aux::integral_constant<bool,
        !aux::is_same<
            aux::bool_list<detail::apply_impl<Ts>::template apply<TArg>::value...>
          , aux::bool_list<aux::never<Ts>::value...>
        >::value
    > { };
};
}
template<class T>
struct type {
template<class TPolicy>
struct not_allowed_by {
    operator aux::false_type() const {
        using constraint_not_satisfied = not_allowed_by;
        return
            constraint_not_satisfied{}.error();
    }
    static inline aux::false_type
    error(_ = "type disabled by constructible policy, added by BOOST_DI_CFG or make_injector<CONFIG>!");
};};
template<class T>
struct is_bound : detail::type_op {
    struct not_resolved { };
    template<class TArg>
    struct apply : aux::integral_constant<bool,
        !aux::is_same<
            typename TArg::template resolve<
                aux::conditional_t<
                    aux::is_same<T, _>::value
                  , typename TArg::type
                  , T
                >
              , typename TArg::name
              , not_resolved
            >
          , not_resolved
         >::value>
    { };
};
template<class T>
struct is_injected : detail::type_op {
    template<class TArg
           , class U = aux::decay_t<aux::conditional_t<aux::is_same<T, _>::value, typename TArg::type, T>>
    > struct apply : aux::conditional_t<
        aux::is_class<U>::value
      , typename type_traits::is_injectable<U>::type
      , aux::true_type
    > { };
};
namespace operators {
template<class X, class Y>
inline auto operator||(const X&, const Y&) {
    return detail::or_<X, Y>{};
}
template<class X, class Y>
inline auto operator&&(const X&, const Y&) {
    return detail::and_<X, Y>{};
}
template<class T>
inline auto operator!(const T&) {
    return detail::not_<T>{};
}
}
template<class T>
struct constructible_impl {
    template<class TArg, BOOST_DI_REQUIRES(TArg::is_root::value || T::template apply<TArg>::value) = 0>
    aux::true_type operator()(const TArg&) const {
        return {};
    }
    template<class TArg, BOOST_DI_REQUIRES(!TArg::is_root::value && !T::template apply<TArg>::value) = 0>
    aux::false_type operator()(const TArg&) const {
        return typename type<typename TArg::type>::template not_allowed_by<T>{};
    }
};
template<class T = aux::never<_>, BOOST_DI_REQUIRES(aux::is_base_of<detail::type_op, T>::value) = 0>
inline auto constructible(const T& = {}) {
    return constructible_impl<T>{};
}
template<class T = aux::never<_>, BOOST_DI_REQUIRES(!aux::is_base_of<detail::type_op, T>::value) = 0>
inline auto constructible(const T& = {}) {
    return constructible_impl<detail::or_<T>>{};
}
}}}}
namespace boost { namespace di { inline namespace v1 { namespace providers {
class heap {
public:
    template<class TInitialization, class TMemory, class T, class... TArgs>
    struct is_creatable {
        static constexpr auto value =
            concepts::creatable<TInitialization, T, TArgs...>::value;
    };
    template<class, class T, class TMemory, class... TArgs>
    auto get(const type_traits::direct&
           , const TMemory&
           , TArgs&&... args) const {
        return new T(static_cast<type_traits::cast_t<TArgs>>(args)...);
    }
    template<class, class T, class TMemory, class... TArgs>
    auto get(const type_traits::uniform&
           , const TMemory&
           , TArgs&&... args) const {
        return new T{static_cast<type_traits::cast_t<TArgs>>(args)...};
    }
};
}}}}
#define BOOST_DI_IF(cond, t, f) BOOST_DI_IF_I(cond, t, f)
#define BOOST_DI_REPEAT(i, m, ...) BOOST_DI_REPEAT_N(i, m, __VA_ARGS__)
#define BOOST_DI_CAT(a, ...) BOOST_DI_PRIMITIVE_CAT(a, __VA_ARGS__)
#define BOOST_DI_EMPTY()
#define BOOST_DI_COMMA() ,
#define BOOST_DI_EAT(...)
#define BOOST_DI_EXPAND(...) __VA_ARGS__
#define BOOST_DI_SIZE(...) BOOST_DI_CAT(BOOST_DI_VARIADIC_SIZE_I(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,),)
#define BOOST_DI_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define BOOST_DI_ELEM(n, ...) BOOST_DI_ELEM_I(n,__VA_ARGS__)
#define BOOST_DI_IS_EMPTY(...) BOOST_DI_DETAIL_IS_EMPTY_IIF(BOOST_DI_IBP(__VA_ARGS__))(BOOST_DI_DETAIL_IS_EMPTY_GEN_ZERO, BOOST_DI_DETAIL_IS_EMPTY_PROCESS)(__VA_ARGS__)
#define BOOST_DI_DETAIL_IS_EMPTY_PRIMITIVE_CAT(a, b) a ## b
#define BOOST_DI_DETAIL_IS_EMPTY_IIF(bit) BOOST_DI_DETAIL_IS_EMPTY_PRIMITIVE_CAT(BOOST_DI_DETAIL_IS_EMPTY_IIF_,bit)
#define BOOST_DI_DETAIL_IS_EMPTY_NON_FUNCTION_C(...) ()
#define BOOST_DI_DETAIL_IS_EMPTY_GEN_ZERO(...) 0
#define BOOST_DI_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, size, ...) size
#define BOOST_DI_IF_I(cond, t, f) BOOST_DI_IIF(cond, t, f)
#define BOOST_DI_IIF_0(t, f) f
#define BOOST_DI_IIF_1(t, f) t
#define BOOST_DI_IIF_2(t, f) t
#define BOOST_DI_IIF_3(t, f) t
#define BOOST_DI_IIF_4(t, f) t
#define BOOST_DI_IIF_5(t, f) t
#define BOOST_DI_IIF_6(t, f) t
#define BOOST_DI_IIF_7(t, f) t
#define BOOST_DI_IIF_8(t, f) t
#define BOOST_DI_IIF_9(t, f) t
#define BOOST_DI_ELEM_I(n, ...) BOOST_DI_CAT(BOOST_DI_CAT(BOOST_DI_ELEM, n)(__VA_ARGS__,),)
#define BOOST_DI_ELEM0(p1, ...) p1
#define BOOST_DI_ELEM1(p1, p2, ...) p2
#define BOOST_DI_ELEM2(p1, p2, p3, ...) p3
#define BOOST_DI_ELEM3(p1, p2, p3, p4, ...) p4
#define BOOST_DI_ELEM4(p1, p2, p3, p4, p5, ...) p5
#define BOOST_DI_ELEM5(p1, p2, p3, p4, p5, p6, ...) p6
#define BOOST_DI_ELEM6(p1, p2, p3, p4, p5, p6, p7, ...) p7
#define BOOST_DI_ELEM7(p1, p2, p3, p4, p5, p6, p7, p8, ...) p8
#define BOOST_DI_ELEM8(p1, p2, p3, p4, p5, p6, p7, p8, p9, ...) p9
#define BOOST_DI_ELEM9(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, ...) p10
#define BOOST_DI_REPEAT_N(i, m, ...) BOOST_DI_REPEAT_##i(m, __VA_ARGS__)
#define BOOST_DI_REPEAT_1(m, ...) m(0, __VA_ARGS__)
#define BOOST_DI_REPEAT_2(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__)
#define BOOST_DI_REPEAT_3(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__)
#define BOOST_DI_REPEAT_4(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__)
#define BOOST_DI_REPEAT_5(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__)
#define BOOST_DI_REPEAT_6(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__)
#define BOOST_DI_REPEAT_7(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) m(6, __VA_ARGS__)
#define BOOST_DI_REPEAT_8(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) m(6, __VA_ARGS__) m(7, __VA_ARGS__)
#define BOOST_DI_REPEAT_9(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) m(6, __VA_ARGS__) m(7, __VA_ARGS__) m(8, __VA_ARGS__)
#define BOOST_DI_REPEAT_10(m, ...) m(0, __VA_ARGS__) m(1, __VA_ARGS__) m(2, __VA_ARGS__) m(3, __VA_ARGS__) m(4, __VA_ARGS__) m(5, __VA_ARGS__) m(6, __VA_ARGS__) m(7, __VA_ARGS__) m(8, __VA_ARGS__) m(9, __VA_ARGS__)
#if defined(_MSC_VER)
    #define BOOST_DI_EAT_IF_NONE BOOST_DI_EAT(
    #define BOOST_DI_VD_IBP_CAT(a, b) BOOST_DI_VD_IBP_CAT_I(a, b)
    #define BOOST_DI_VD_IBP_CAT_I(a, b) BOOST_DI_VD_IBP_CAT_II(a ## b)
    #define BOOST_DI_VD_IBP_CAT_II(res) res
    #define BOOST_DI_IBP_SPLIT(i, ...) BOOST_DI_VD_IBP_CAT(BOOST_DI_IBP_PRIMITIVE_CAT(BOOST_DI_IBP_SPLIT_,i)(__VA_ARGS__),BOOST_DI_EMPTY())
    #define BOOST_DI_IBP_IS_VARIADIC_C(...) 1 1
    #define BOOST_DI_IBP_SPLIT_0(a, ...) a
    #define BOOST_DI_IBP_SPLIT_1(a, ...) __VA_ARGS__
    #define BOOST_DI_IBP_CAT(a, ...) BOOST_DI_IBP_PRIMITIVE_CAT(a,__VA_ARGS__)
    #define BOOST_DI_IBP_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
    #define BOOST_DI_IBP_IS_VARIADIC_R_1 1,
    #define BOOST_DI_IBP_IS_VARIADIC_R_BOOST_DI_IBP_IS_VARIADIC_C 0,
    #define BOOST_DI_IBP(...) BOOST_DI_IBP_SPLIT(0, BOOST_DI_IBP_CAT(BOOST_DI_IBP_IS_VARIADIC_R_, BOOST_DI_IBP_IS_VARIADIC_C __VA_ARGS__))
    #define BOOST_DI_IIF(bit, t, f) BOOST_DI_IIF_OO((bit, t, f))
    #define BOOST_DI_IIF_OO(par) BOOST_DI_IIF_I ## par
    #define BOOST_DI_IIF_I(bit, t, f) BOOST_DI_IIF_ ## bit(t, f)
    #define BOOST_DI_DETAIL_IS_EMPTY_IIF_0(t, b) b
    #define BOOST_DI_DETAIL_IS_EMPTY_IIF_1(t, b) t
    #define BOOST_DI_DETAIL_IS_EMPTY_PROCESS(...) BOOST_DI_IBP(BOOST_DI_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__ ())
#else
    #define BOOST_DI_EAT_IF_NONE
    #define BOOST_DI_IBP_SPLIT(i, ...) BOOST_DI_PRIMITIVE_CAT(BOOST_DI_IBP_SPLIT_, i)(__VA_ARGS__)
    #define BOOST_DI_IBP_SPLIT_0(a, ...) a
    #define BOOST_DI_IBP_SPLIT_1(a, ...) __VA_ARGS__
    #define BOOST_DI_IBP_IS_VARIADIC_C(...) 1
    #define BOOST_DI_IBP_IS_VARIADIC_R_1 1,
    #define BOOST_DI_IBP_IS_VARIADIC_R_BOOST_DI_IBP_IS_VARIADIC_C 0,
    #define BOOST_DI_IBP(...) BOOST_DI_IBP_SPLIT(0, BOOST_DI_CAT( BOOST_DI_IBP_IS_VARIADIC_R_, BOOST_DI_IBP_IS_VARIADIC_C __VA_ARGS__))
    #define BOOST_DI_IIF(bit, t, f) BOOST_DI_IIF_I(bit, t, f)
    #define BOOST_DI_IIF_I(bit, t, f) BOOST_DI_IIF_II(BOOST_DI_IIF_ ## bit(t, f))
    #define BOOST_DI_IIF_II(id) id
    #define BOOST_DI_DETAIL_IS_EMPTY_IIF_0(t, ...) __VA_ARGS__
    #define BOOST_DI_DETAIL_IS_EMPTY_IIF_1(t, ...) t
    #define BOOST_DI_DETAIL_IS_EMPTY_PROCESS(...) BOOST_DI_IBP(BOOST_DI_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__ ())
#endif
namespace boost { namespace di { inline namespace v1 {
template<class, class> struct named;
namespace detail {
struct named_impl { template<class T> T operator=(const T&) const; };
static constexpr BOOST_DI_UNUSED named_impl named{};
template<class T, class TName>
struct combine_impl {
    using type = di::named<TName, T>;
};
template<class T>
struct combine_impl<T, aux::none_type> {
    using type = T;
};
template<class, class>
struct combine;
template<class... T1, class... T2>
struct combine<aux::type_list<T1...>, aux::type_list<T2...>> {
    using type = aux::type_list<typename combine_impl<T1, T2>::type...>;
};
template<class T1, class T2>
using combine_t = typename combine<T1, T2>::type;
template<class T>
auto ctor_impl4__(int) -> aux::function_traits_t<BOOST_DI_DECLTYPE_WKND(&T::template ctor<_, _, _, _>)>;
template<class T>
aux::type_list<> ctor_impl4__(...);
template<class T>
auto ctor_impl3__(int) -> aux::function_traits_t<BOOST_DI_DECLTYPE_WKND(&T::template ctor<_, _, _>)>;
template<class T>
decltype(ctor_impl4__<T>(0)) ctor_impl3__(...);
template<class T>
auto ctor_impl2__(int) -> aux::function_traits_t<BOOST_DI_DECLTYPE_WKND(&T::template ctor<_, _>)>;
template<class T>
decltype(ctor_impl3__<T>(0)) ctor_impl2__(...);
template<class T>
auto ctor_impl1__(int) -> aux::function_traits_t<BOOST_DI_DECLTYPE_WKND(&T::template ctor<_>)>;
template<class T>
decltype(ctor_impl2__<T>(0)) ctor_impl1__(...);
template<class T>
auto ctor__(int) -> aux::function_traits_t<BOOST_DI_DECLTYPE_WKND(&T::ctor)>;
template<class T>
decltype(ctor_impl1__<T>(0)) ctor__(...);
}
template<class... Ts>
using inject = aux::type_list<Ts...>;
}}}
#define BOOST_DI_GEN_CTOR_IMPL(p, i) \
    BOOST_DI_IF(i, BOOST_DI_COMMA, BOOST_DI_EAT)() \
    BOOST_DI_IF(BOOST_DI_IBP(p), BOOST_DI_EAT p, p)
#define BOOST_DI_GEN_CTOR(i, ...) BOOST_DI_GEN_CTOR_IMPL(BOOST_DI_ELEM(i, __VA_ARGS__,), i)
#define BOOST_DI_GEN_ARG_NAME(p) BOOST_DI_GEN_ARG_NAME_IMPL p )
#define BOOST_DI_GEN_NONE_TYPE(p) ::boost::di::aux::none_type
#define BOOST_DI_GEN_ARG_NAME_IMPL(p) decltype(::boost::di::detail::p) BOOST_DI_EAT(
#define BOOST_DI_GEN_NAME_IMPL(p, i) \
    BOOST_DI_IF(i, BOOST_DI_COMMA, BOOST_DI_EAT)() \
    BOOST_DI_IF(BOOST_DI_IBP(p), BOOST_DI_GEN_ARG_NAME, BOOST_DI_GEN_NONE_TYPE)(p)
#define BOOST_DI_GEN_NAME(i, ...) BOOST_DI_GEN_NAME_IMPL(BOOST_DI_ELEM(i, __VA_ARGS__,), i)
#define BOOST_DI_T_INJECT_IMPL(...) __VA_ARGS__
#define BOOST_DI_T_INJECT_IMPL__(type) type
#define BOOST_DI_T_INJECT(type) BOOST_DI_T_INJECT__ type BOOST_DI_EAT_IF_NONE )
#define BOOST_DI_T_INJECT__(...) __VA_ARGS__ BOOST_DI_T_INJECT_IMPL(
#define BOOST_DI_T_GET(...) __VA_ARGS__ BOOST_DI_EAT(
#define BOOST_DI_INJECT_TRAITS_T(T, ...) BOOST_DI_INJECT_TRAITS__(T, __VA_ARGS__)
#define BOOST_DI_INJECT_TRAITS_T__(T, ...) BOOST_DI_INJECT_TRAITS__((), __VA_ARGS__)
#define BOOST_DI_INJECT_TRAITS_EMPTY_IMPL(...) \
    using boost_di_inject__ BOOST_DI_UNUSED = ::boost::di::aux::type_list<>
#define BOOST_DI_INJECT_TRAITS__(T, ...) \
    BOOST_DI_IF( \
        BOOST_DI_IS_EMPTY(__VA_ARGS__) \
      , BOOST_DI_INJECT_TRAITS_EMPTY_IMPL \
      , BOOST_DI_INJECT_TRAITS_IMPL \
    )(T, __VA_ARGS__)
#define BOOST_DI_INJECT_TRAITS_IMPL(T, ...) \
    struct boost_di_inject__ {\
        BOOST_DI_T_GET T)\
        static void ctor( \
            BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_CTOR, __VA_ARGS__) \
        ); \
        static void name( \
            BOOST_DI_REPEAT(BOOST_DI_SIZE(__VA_ARGS__), BOOST_DI_GEN_NAME, __VA_ARGS__) \
        ); \
        using type BOOST_DI_UNUSED = ::boost::di::detail::combine_t< \
            decltype(::boost::di::detail::ctor__<boost_di_inject__>(0)) \
          , ::boost::di::aux::function_traits_t<decltype(name)> \
        >; \
        static_assert( \
            BOOST_DI_SIZE(__VA_ARGS__) <= BOOST_DI_CFG_CTOR_LIMIT_SIZE \
          , "Number of constructor arguments is out of range - see BOOST_DI_CFG_CTOR_LIMIT_SIZE" \
        );\
    }
#define BOOST_DI_INJECT_TRAITS(...) BOOST_DI_INJECT_TRAITS__((), __VA_ARGS__)
#define BOOST_DI_INJECT(T, ...) \
    BOOST_DI_IF(BOOST_DI_IBP(T), BOOST_DI_INJECT_TRAITS_T, BOOST_DI_INJECT_TRAITS_T__)(T, __VA_ARGS__); \
    BOOST_DI_IF(BOOST_DI_IBP(T), BOOST_DI_T_INJECT, BOOST_DI_T_INJECT_IMPL__)(T) \
    (BOOST_DI_REPEAT( \
        BOOST_DI_SIZE(__VA_ARGS__) \
      , BOOST_DI_GEN_CTOR \
      , __VA_ARGS__) \
    )
#endif
#endif

