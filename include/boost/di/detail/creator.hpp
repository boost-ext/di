//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_CREATOR_HPP
    #define BOOST_DI_DETAIL_CREATOR_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"
    #include "boost/di/type_traits/make_plain.hpp"
    #include "boost/di/type_traits/is_same_base_of.hpp"

    #include <typeinfo>
    #include <boost/config.hpp>
    #include <boost/type.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/has_xxx.hpp>

    namespace boost {
    namespace di {
    namespace detail {

    template<
        typename TBinder
      , typename TPolicies
    >
    class creator
    {
        template<typename TDependency>
        struct ctor
            : type_traits::ctor_traits<typename TDependency::given>::type
        { };

        template<typename T, typename TCallStack>
        struct binder
            : TBinder::template resolve<T, TCallStack>::type
        { };

        template<
            typename T
          , typename TCallStack
          , typename TDependency
        >
        struct dependency
        {
            typedef T type;
            typedef TCallStack call_stack;
            typedef typename TDependency::given given;
            typedef typename TDependency::expected expected;
            typedef typename TDependency::scope scope;
            typedef typename TDependency::name name;
            typedef typename TDependency::context context;
        };

        BOOST_MPL_HAS_XXX_TRAIT_DEF(named_type)

        template<
            typename T
          , typename TCallStack
          , typename TDeps
          , typename TScopes
          , typename TRefs
          , typename TVisitor
        >
        class eager_creator
        {
            typedef typename type_traits::make_plain<T>::type plain_t;

            template<typename U, typename = void>
            struct named_type
            {
                typedef U type;
            };

            template<typename U>
            struct named_type<U, typename enable_if<
                has_named_type<typename type_traits::remove_accessors<U>::type> >::type
            >
            {
                typedef typename type_traits::remove_accessors<typename U::named_type>::type type;
            };

        public:
            eager_creator(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor)
                : deps_(deps), scopes_(scopes), refs_(refs), visitor_(visitor)
            { }

            template<
                typename U
            #if !defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
              , typename NU = typename named_type<U>::type
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
            #endif
            >
            operator U() {
                #if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
                    typedef typename named_type<U>::type NU;
                    typedef typename type_traits::make_plain<U>::type PU;
                #endif

                return creator::execute_impl<
                    NU
                  , typename mpl::push_back<TCallStack, PU>::type
                  , binder<U, TCallStack>
                >(deps_, scopes_, refs_, visitor_)(boost::type<NU>());
            }

            template<
                typename U

            #if !defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
              , typename NU = typename named_type<U>::type
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
            #endif
            >
            operator const U&() const {
                #if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
                    typedef typename named_type<U>::type NU;
                    typedef typename type_traits::make_plain<U>::type PU;
                #endif

                return creator::execute_impl<
                    const U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , binder<const U&, TCallStack>
                >(deps_, scopes_, refs_, visitor_)(boost::type<const NU&>());
            }

            template<
                typename U
            #if !defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
              , typename NU = typename named_type<U>::type
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
            #endif
            >
            operator U&() const {
                #if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
                    typedef typename named_type<U>::type NU;
                    typedef typename type_traits::make_plain<U>::type PU;
                #endif

                return creator::execute_impl<
                    U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , binder<U&, TCallStack>
                >(deps_, scopes_, refs_, visitor_)(boost::type<NU&>());
            }

        private:
            TDeps& deps_;
            TScopes& scopes_;
            TRefs& refs_;
            const TVisitor& visitor_;
        };

    public:
        template<
            typename T
          , typename TParent
          , typename TCallStack
          , typename TDeps
          , typename TScopes
          , typename TRefs
          , typename TVisitor
        >
        static eager_creator<TParent, TCallStack, TDeps, TScopes, TRefs, TVisitor>
        execute(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor, typename enable_if<is_same<T, any_type> >::type* = 0) {
            return eager_creator<TParent, TCallStack, TDeps, TScopes, TRefs, TVisitor>(deps, scopes, refs, visitor);
        }

        template<
            typename T
          , typename // parent, to delete copy/move ctor, not needed
          , typename TCallStack
          , typename TDeps
          , typename TScopes
          , typename TRefs
          , typename TVisitor
        >
        static const typename binder<T, TCallStack>::result_type&
        execute(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor, typename disable_if<is_same<T, any_type> >::type* = 0) {
            return execute_impl<
                T
              , typename mpl::push_back<
                    TCallStack
                  , typename binder<T, TCallStack>::given
                >::type
              , binder<T, TCallStack>
            >(deps, scopes, refs, visitor);
        }

    private:
        #define BOOST_PP_FILENAME_1 "boost/di/detail/creator.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
        #include BOOST_PP_ITERATE()

        template<
            typename TDependency
          , typename TDeps
          , typename TScopes
        >
        static typename enable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
        acquire(TDeps& deps, TScopes&) {
            return static_cast<TDependency&>(deps);
        }

        template<
            typename TDependency
          , typename TDeps
          , typename TScopes
        >
        static typename disable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
        acquire(TDeps&, TScopes& scopes) {
            typename TScopes::const_iterator it = scopes.find(&typeid(TDependency));
            if (it != scopes.end()) {
                return *static_cast<TDependency*>(it->second.get());
            }

            TDependency* dependency = new TDependency();
            scopes[&typeid(TDependency)] = aux::shared_ptr<void>(dependency);
            return *dependency;
        }

        template<typename TSeq, typename TDeps, typename T>
        static typename enable_if<mpl::empty<TSeq> >::type assert_policies() { }

        template<typename TSeq, typename TDeps, typename T>
        static typename disable_if<mpl::empty<TSeq> >::type assert_policies() {
            typedef typename mpl::front<TSeq>::type policy;
            policy::template assert_policy<TDeps, T>();
            assert_policies<typename mpl::pop_front<TSeq>::type, TDeps, T>();
        }
    };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename TCallStack
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == BOOST_PP_ITERATION()
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;

        #define BOOST_DI_CREATOR_EXECUTE(z, n, _)       \
            BOOST_PP_COMMA_IF(n)                        \
            execute<                                    \
               typename mpl::at_c<                      \
                   typename ctor<TDependency>::type     \
                 , n                                    \
               >::type                                  \
              , T                                       \
              , TCallStack                              \
            >(deps, scopes, refs, visitor)

        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    BOOST_PP_REPEAT(
                        BOOST_PP_ITERATION()
                      , BOOST_DI_CREATOR_EXECUTE
                      , ~
                    )
                )
            );

        #undef BOOST_DI_CREATOR_EXECUTE

        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }

#endif

