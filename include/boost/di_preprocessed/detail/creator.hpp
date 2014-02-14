#ifndef BOOST_DI_DETAIL_CREATOR_HPP
#define BOOST_DI_DETAIL_CREATOR_HPP

    #include "boost/di_preprocessed/aux_/common.hpp"
    #include "boost/di_preprocessed/aux_/memory.hpp"
    #include "boost/di_preprocessed/type_traits/ctor_traits.hpp"
    #include "boost/di_preprocessed/type_traits/make_plain.hpp"
    #include "boost/di_preprocessed/type_traits/is_same_base_of.hpp"
    #include <typeinfo>
    #include <boost/config.hpp>
    #include <boost/type.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/has_xxx.hpp>











    namespace boost {
    namespace di {
    namespace detail {

    template<typename TBinder>
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
        };

        BOOST_MPL_HAS_XXX_TRAIT_DEF(named_type)

        template<
            typename T
          , typename TCallStack
          , typename TPolicies
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

              , typename NU = typename named_type<U>::type
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type

            >
            operator U() {





                return creator::execute_impl<
                    NU
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<U, TCallStack>
                >(deps_, scopes_, refs_, visitor_)(boost::type<NU>());
            }

            template<
                typename U


              , typename NU = typename named_type<U>::type
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type

            >
            operator const U&() const {





                return creator::execute_impl<
                    const U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<const U&, TCallStack>
                >(deps_, scopes_, refs_, visitor_)(boost::type<const NU&>());
            }

            template<
                typename U

              , typename NU = typename named_type<U>::type
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type

            >
            operator U&() const {





                return creator::execute_impl<
                    U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
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
          , typename TPolicies
          , typename TDeps
          , typename TScopes
          , typename TRefs
          , typename TVisitor
        >
        static eager_creator<TParent, TCallStack, TPolicies, TDeps, TScopes, TRefs, TVisitor>
        execute(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor
              , typename enable_if<is_same<T, any_type> >::type* = 0) {
            return eager_creator<TParent, TCallStack, TPolicies, TDeps, TScopes, TRefs, TVisitor>(
                deps, scopes, refs, visitor
            );
        }

        template<
            typename T
          , typename
          , typename TCallStack
          , typename TPolicies
          , typename TDeps
          , typename TScopes
          , typename TRefs
          , typename TVisitor
        >
        static const typename binder<T, TCallStack>::result_type&
        execute(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor
              , typename disable_if<is_same<T, any_type> >::type* = 0) {
            return execute_impl<
                T
              , typename mpl::push_back<
                    TCallStack
                  , typename binder<T, TCallStack>::given
                >::type
              , TPolicies
              , binder<T, TCallStack>
            >(deps, scopes, refs, visitor);
        }

    private:



    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 0
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                   




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 1
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 2
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 3
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 4
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 5
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 6
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 7
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 8
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 9
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 10
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 11
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 10 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 12
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 10 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 11 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 13
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 10 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 11 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 12 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 14
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 10 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 11 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 12 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 13 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 15
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 10 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 11 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 12 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 13 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 14 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 16
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 10 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 11 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 12 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 13 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 14 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 15 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 17
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 10 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 11 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 12 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 13 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 14 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 15 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 16 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 18
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 10 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 11 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 12 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 13 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 14 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 15 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 16 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 17 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 19
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 10 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 11 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 12 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 13 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 14 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 15 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 16 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 17 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 18 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }




    template<
        typename T
      , typename TCallStack
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TScopes
      , typename TRefs
      , typename TVisitor
    >
    static typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == 20
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TScopes& scopes, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        typedef typename TDependency::result_type convertible_type;
        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps, scopes).create(
                    execute< typename mpl::at_c< typename ctor<TDependency>::type , 0 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 1 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 2 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 3 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 4 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 5 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 6 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 7 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 8 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 9 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 10 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 11 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 12 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 13 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 14 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 15 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 16 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 17 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 18 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor) , execute< typename mpl::at_c< typename ctor<TDependency>::type , 19 >::type , T , TCallStack , TPolicies >(deps, scopes, refs, visitor)




                )
            );



        refs.push_back(aux::shared_ptr<void>(convertible));

        return *convertible;
    }

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

    }
    }
    }

#endif

