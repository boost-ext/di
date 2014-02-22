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
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/has_xxx.hpp>

#include <boost/units/detail/utility.hpp>
    namespace boost {
    namespace di {
    namespace detail {

    template<typename TBinder>
    class creator
    {
        template<typename>
        friend class creator;

        class type_comparator
        {
        public:
            bool operator ()(const std::type_info* lhs, const std::type_info* rhs) const {
                return lhs->before(*rhs);
            }
        };

        typedef std::map<const std::type_info*, aux::shared_ptr<void>, type_comparator> scopes_type;
        typedef std::map<const std::type_info*, function<any()>, type_comparator> f_type;
        typedef std::vector<std::pair<function<int(const std::type_info*, const std::type_info*, const std::vector<const std::type_info*>&, int)>, f_type> > creators_type;

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
            eager_creator(creator& c, TDeps& deps, TRefs& refs, const TVisitor& visitor)
                : c_(c), deps_(deps), refs_(refs), visitor_(visitor)
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

                return c_.execute_impl<
                    NU
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<U, TCallStack>
                >(deps_, refs_, visitor_)(boost::type<NU>());
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

                return c_.execute_impl<
                    const U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<const U&, TCallStack>
                >(deps_, refs_, visitor_)(boost::type<const NU&>());
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

                return c_.execute_impl<
                    U&
                  , typename mpl::push_back<TCallStack, PU>::type
                  , TPolicies
                  , binder<U&, TCallStack>
                >(deps_, refs_, visitor_)(boost::type<NU&>());
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
        execute(TDeps& deps, TRefs& refs, const TVisitor& visitor
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
        const typename binder<T, TCallStack>::result_type&
        execute(TDeps& deps, TRefs& refs, const TVisitor& visitor
              , typename disable_if<is_same<T, any_type> >::type* = 0) {
            return execute_impl<
                T
              , typename mpl::push_back<
                    TCallStack
                  , typename binder<T, TCallStack>::given
                >::type
              , TPolicies
              , binder<T, TCallStack>
            >(deps, refs, visitor);
        }

        template<
            typename TDependency
          , typename TPolicies
          , typename TCallStack
          , typename TCreator
          , typename TDeps
          , typename TRefs
          , typename TVisitor
        >
        void bind_create(const TCreator& creator, TDeps& deps, TRefs& refs, const TVisitor& visitor) {
            f_type m;

            m[&typeid(convertibles::shared<typename TDependency::expected>)] =
               boost::bind(
                   &TCreator::template execute_any<
                       aux::shared_ptr<typename TDependency::expected>
                     , aux::shared_ptr<typename TDependency::expected>
                     , TCallStack
                     , TPolicies
                     , TDeps
                     , TRefs
                     , TVisitor
                   >
                 , creator
                 , deps
                 , refs
                 , visitor
               );

            m[&typeid(convertibles::copy<typename TDependency::expected>)] =
               boost::bind(
                   &TCreator::template execute_any<
                       typename TDependency::expected
                     , typename TDependency::expected
                     , TCallStack
                     , TPolicies
                     , TDeps
                     , TRefs
                     , TVisitor
                   >
                 , creator
                 , deps
                 , refs
                 , visitor
               );

            m[&typeid(convertibles::reference<typename TDependency::expected>)] =
               boost::bind(
                   &TCreator::template execute_any<
                       const typename TDependency::expected&
                     , const typename TDependency::expected&
                     , TCallStack
                     , TPolicies
                     , TDeps
                     , TRefs
                     , TVisitor
                   >
                 , creator
                 , deps
                 , refs
                 , visitor
               );

            //m[&typeid(convertibles::value<typename TDependency::expected>)] =
               //boost::bind(
                   //&TCreator::template execute_any<
                       //typename TDependency::expected
                     //, typename TDependency::expected
                     //, TCallStack
                     //, TPolicies
                     //, TDeps
                     //, TRefs
                     //, TVisitor
                   //>
                 //, creator
                 //, deps
                 //, refs
                 //, visitor
               //);

            creators_.push_back(
                std::make_pair(
                    boost::bind(&TDependency::when, _1, _2, _3, _4)
                  , m
                )
            );
        }

    private:
        template<
            typename T
          , typename TParent // to ignore copy/move ctor
          , typename TCallStack
          , typename TPolicies
          , typename TDeps
          , typename TRefs
          , typename TVisitor
        >
        any execute_any(TDeps& deps, TRefs& refs, const TVisitor& visitor) {
            skip_ = &typeid(T);
            return any(execute<T, TParent, TCallStack, TPolicies>(deps, refs, visitor));
        }

        #define BOOST_PP_FILENAME_1 "boost/di/detail/creator.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
        #include BOOST_PP_ITERATE()

        template<
            typename TDependency
          , typename TDeps
        >
        typename enable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
        acquire(TDeps& deps) {
            return static_cast<TDependency&>(deps);
        }

        template<
            typename TDependency
          , typename TDeps
        >
        typename disable_if<is_base_of<TDependency, TDeps>, TDependency&>::type
        acquire(TDeps&) {
            typename scopes_type::const_iterator it = scopes_.find(&typeid(TDependency));
            if (it != scopes_.end()) {
                return *static_cast<TDependency*>(it->second.get());
            }

            TDependency* dependency = new TDependency();
            scopes_[&typeid(TDependency)] = aux::shared_ptr<void>(dependency);
            return *dependency;
        }

        template<typename TSeq, typename TDeps, typename T>
        typename enable_if<mpl::empty<TSeq> >::type assert_policies() { }

        template<typename TSeq, typename TDeps, typename T>
        typename disable_if<mpl::empty<TSeq> >::type assert_policies() {
            typedef typename mpl::front<TSeq>::type policy;
            policy::template assert_policy<TDeps, T>();
            assert_policies<typename mpl::pop_front<TSeq>::type, TDeps, T>();
        }

    private:
        template<typename TSeq, typename V>
        static typename enable_if<mpl::empty<TSeq> >::type fill_call_stack(V&) { }

        template<typename TSeq, typename V>
        static typename disable_if<mpl::empty<TSeq> >::type fill_call_stack(V& v) {
			v.push_back(&typeid(typename mpl::front<TSeq>::type));
            fill_call_stack<typename mpl::pop_front<TSeq>::type>(v);
		}

        scopes_type scopes_;
        creators_type creators_;
        const std::type_info* skip_;
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
      , const typename TDependency::result_type&
    >::type execute_impl(TDeps& deps, TRefs& refs, const TVisitor& visitor) {

        typedef dependency<T, TCallStack, TDependency> dependency_type;
        typedef typename TDependency::result_type convertible_type;

        assert_policies<TPolicies, typename TDeps::types, dependency_type>();
        (visitor)(dependency_type());

        //should be binder
        int best = 0;
        std::size_t r = 0;

        if (skip_ != &typeid(T)) {
            skip_ = &typeid(void);

            for (std::size_t q = 0; q < creators_.size(); ++q) {

                std::vector<const std::type_info*> call_stack;
                fill_call_stack<TCallStack>(call_stack);

                int current = creators_[q].first(
                    &typeid(typename type_traits::make_plain<T>::type)
                  , &typeid(typename concepts::type_traits::get_name<T>::type)
                  , call_stack
                  , TDependency::scope::priority::value
                );

                if (current > best) {
                    best = current;
                    r = q;
                }
            }

            if (best > 0) {
                convertible_type* convertible = 0;

                for (const auto& i : creators_[r].second) {
                    if (&typeid(typename TDependency::result_type) == i.first) {
                         convertible = new convertible_type(
                            any_cast<typename TDependency::result_type>(i.second())
                        );
                    }
                }

                refs.push_back(aux::shared_ptr<void>(convertible));
                return *convertible;
            }
        }

        #define BOOST_DI_CREATOR_EXECUTE(z, n, _)       \
            BOOST_PP_COMMA_IF(n)                        \
            execute<                                    \
               typename mpl::at_c<                      \
                   typename ctor<TDependency>::type     \
                 , n                                    \
               >::type                                  \
             , T                                        \
             , TCallStack                               \
             , TPolicies                                \
            >(deps, refs, visitor)

        convertible_type* convertible =
            new convertible_type(
                acquire<typename TDependency::type>(deps).create(
                    type_traits::policy<
                        mpl::empty<typename TDeps::types>::value
                    >()
                    BOOST_PP_COMMA_IF(BOOST_PP_ITERATION())
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

