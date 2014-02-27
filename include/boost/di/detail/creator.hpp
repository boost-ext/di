//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_CREATOR_HPP
    #define BOOST_DI_DETAIL_CREATOR_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"
    #include "boost/di/type_traits/create_traits.hpp"
    #include "boost/di/type_traits/make_plain.hpp"
    #include "boost/di/type_traits/is_same_base_of.hpp"

    #include "boost/di/concepts/type_traits/name.hpp"
    #include "boost/di/concepts.hpp"
    #include "boost/di/extensions/static/builder.hpp"
    #include "boost/di/extensions/static/binder.hpp"
    #include "boost/di/extensions/dynamic/builder.hpp"

    #include <typeinfo>
    #include <map>
    #include <vector>
    #include <typeinfo>
    #include <boost/config.hpp>
    #include <boost/bind.hpp>
    #include <boost/function.hpp>
    #include <boost/any.hpp>
    #include <boost/type.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/type_traits/is_polymorphic.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/has_xxx.hpp>

    namespace boost {
    namespace di {
    namespace detail {

    template<typename TDependecies>
    struct builder
    {
        template<typename T>
        struct get
            : mpl::if_<mpl::empty<TDependecies>, dynamic_builder<T>, static_builder<T> >::type
        { };
    };

    template<
        typename TDependecies
      , template<typename> class TBuilder = builder<TDependecies>::template get
      , template<typename> class TBinder = binder
    >
    class creator
        : public TBuilder<TDependecies>
    {
        template<typename TDependency>
        struct ctor
            : type_traits::ctor_traits<typename TDependency::given>::type
        { };

        template<typename T, typename TCallStack>
        struct binder
            : TBinder<TDependecies>::template resolve<T, TCallStack>::type
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

        template<
            typename T
          , typename TCallStack
          , typename TPolicies
          , typename TDeps
          , typename TRefs
          , typename TVisitor
        >
        class eager_creator
        {
            typedef typename type_traits::make_plain<T>::type plain_t;

        public:
            eager_creator(creator& c, TDeps& deps, TRefs& refs, const TVisitor& visitor)
                : c_(c), deps_(deps), refs_(refs), visitor_(visitor)
            { }

            template<
                typename U
            #if !defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
            #endif
            >
            operator U() {
                #if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
                    typedef typename type_traits::make_plain<U>::type PU;
                #endif

                return c_.create_impl<
                    U
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<U, TCallStack>
                >(deps_, refs_, visitor_);
            }

            template<
                typename U
            #if !defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
            #endif
            >
            operator const U&() const {
                #if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
                    typedef typename type_traits::make_plain<U>::type PU;
                #endif

                return c_.create_impl<
                    const U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<const U&, TCallStack>
                >(deps_, refs_, visitor_);
            }

            template<
                typename U
            #if !defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
              , typename PU = typename type_traits::make_plain<U>::type
              , typename = typename disable_if<type_traits::is_same_base_of<PU, plain_t> >::type
            #endif
            >
            operator U&() const {
                #if defined(BOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS)
                    typedef typename type_traits::make_plain<U>::type PU;
                #endif

                return c_.create_impl<
                    U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<U&, TCallStack>
                >(deps_, refs_, visitor_);
            }

        private:
            creator& c_;
            TDeps& deps_;
            TRefs& refs_;
            const TVisitor& visitor_;
        };

    public:
        template<
            typename T
          , typename TParent // to ignore copy/move ctor
          , typename TCallStack
          , typename TPolicies
          , typename TDeps
          , typename TRefs
          , typename TVisitor
        >
        eager_creator<TParent, TCallStack, TPolicies, TDeps, TRefs, TVisitor>
        create_(TDeps& deps, TRefs& refs, const TVisitor& visitor
              , typename enable_if<is_same<T, any_type> >::type* = 0) {
            return eager_creator<TParent, TCallStack, TPolicies, TDeps, TRefs, TVisitor>(
                *this, deps, refs, visitor
            );
        }

        template<
            typename T
          , typename // TParent - not needed
          , typename TCallStack
          , typename TPolicies
          , typename TDeps
          , typename TRefs
          , typename TVisitor
        >
        const convertible<T>& create_(
            TDeps& deps, TRefs& refs, const TVisitor& visitor, typename disable_if<is_same<T, any_type> >::type* = 0) {
            return create_impl<
                T
              , typename mpl::push_back<
                    TCallStack
                  , typename binder<T, TCallStack>::given
                >::type
              , TPolicies
              , binder<T, TCallStack>
            >(deps, refs, visitor);
        }

    private:
        #define BOOST_PP_FILENAME_1 "boost/di/detail/creator.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
        #include BOOST_PP_ITERATE()

        template<typename TSeq, typename TDeps, typename T>
        typename enable_if<mpl::empty<TSeq> >::type assert_policies() { }

        template<typename TSeq, typename TDeps, typename T>
        typename disable_if<mpl::empty<TSeq> >::type assert_policies() {
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
      , typename TPolicies
      , typename TDependency
      , typename TDeps
      , typename TRefs
      , typename TVisitor
    >
    typename enable_if_c<
        mpl::size<typename ctor<TDependency>::type>::value == BOOST_PP_ITERATION()
      , const convertible<T>&
    >::type create_impl(TDeps& deps, TRefs& refs, const TVisitor& visitor) {
        typedef dependency<T, TCallStack, TDependency> dependency_type;
        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        return this->template build<
            T
          , typename ctor<TDependency>::type
          , TCallStack
          , TPolicies
          , TDependency
        >(*this, deps, refs, visitor);
    }

#endif

