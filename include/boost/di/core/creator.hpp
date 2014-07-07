//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_CORE_CREATOR_HPP
    #define BOOST_DI_CORE_CREATOR_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/core/binder.hpp"
    #include "boost/di/core/any_type.hpp"
    #include "boost/di/type_traits/ctor_traits.hpp"
    #include "boost/di/type_traits/function_traits.hpp"
    #include "boost/di/wrappers/universal.hpp"
    #include "boost/di/creators/new_creator.hpp"

    #include <typeinfo>
    #include <map>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/ref.hpp>
    #include <boost/none_t.hpp>
    #include <boost/bind.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/empty.hpp>
    #include <boost/mpl/push_back.hpp>

    namespace boost {
    namespace di {
    namespace core {

    template<
        typename TDependecies = mpl::vector0<>
      , template<typename> class TBinder = binder
      , template<
            typename = ::boost::none_t
          , typename = ::boost::mpl::vector0<>
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
            : type_traits::function_traits<BOOST_DI_FEATURE_DECLTYPE(&TDependency::create)>::type
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

    public:
        explicit creator(const TBinder<TDependecies>& binder = TBinder<TDependecies>())
            : binder_(binder)
        { }

        template<
            typename T
          , typename TParent // ignore copy/move ctor
          , typename TCallStack
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TArgs
        >
        typename enable_if<is_same<T, TAnyType<> >, TAnyType<TParent, TCallStack, creator, TDeps, TRefs, TVisitor, TArgs> >::type
        create(TDeps& deps, TRefs& refs, const TVisitor& visitor, const TArgs& args) {
            return TAnyType<TParent, TCallStack, creator, TDeps, TRefs, TVisitor, TArgs>(*this, deps, refs, visitor, args);
        }

        template<
            typename T
          , typename // TParent - not used
          , typename TCallStack
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TArgs
        >
        typename disable_if<is_same<T, TAnyType<> >, wrappers::universal<T> >::type
        create(TDeps& deps, TRefs& refs, const TVisitor& visitor, const TArgs& args) {
            typedef typename TBinder<TDependecies>::template
                resolve<T, TCallStack>::type dependency_type;

            BOOST_DI_FEATURE_EXAMINE_CALL_STACK(
                typedef typename mpl::push_back<TCallStack, T>::type call_stack_type;
            )

            BOOST_DI_FEATURE_NO_EXAMINE_CALL_STACK(
                typedef TCallStack call_stack_type;
            )

            typedef data<T, call_stack_type, dependency_type> data_type;
            assert_policies<typename TArgs::types, data_type>(args);
            (visitor)(data_type());

            return create_impl<T, dependency_type, call_stack_type>(deps, refs, visitor, args);
        }

    private:
        #define BOOST_PP_FILENAME_1 "boost/di/core/creator.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_CTOR_LIMIT_FROM(0)
        #include BOOST_PP_ITERATE()

        template<
            typename T
          , typename TDependency
          , typename TCallStack
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TArgs
        >
        typename enable_if_c<!mpl::size<scope_create<TDependency> >::value, wrappers::universal<T> >::type
        create_impl(TDeps& deps, TRefs& refs, const TVisitor&, const TArgs&) {
            return wrappers::universal<T>(refs, acquire<TDependency>(deps).create());
        }

        template<
            typename T
          , typename TDependency
          , typename TCallStack
          , typename TDeps
          , typename TRefs
          , typename TVisitor
          , typename TArgs
        >
        typename disable_if_c<!mpl::size<scope_create<TDependency> >::value, wrappers::universal<T> >::type
        create_impl(TDeps& deps, TRefs& refs, const TVisitor& visitor, const TArgs& args) {
            typedef typename type_traits::ctor_traits<typename TDependency::given>::type ctor_type;

            return wrappers::universal<T>(
                refs
              , acquire<TDependency>(deps).create(
                    boost::bind(
                        &creator::create_impl<
                            T
                          , TDependency
                          , ctor_type
                          , TCallStack
                          , TDeps
                          , TRefs
                          , TVisitor
                          , TArgs
                        >
                      , this
                      , boost::ref(deps)
                      , boost::ref(refs)
                      , boost::cref(visitor)
                      , boost::cref(args)
                    )
                )
            );
        }

        template<typename TSeq, typename, typename TArgs>
        static typename enable_if<mpl::empty<TSeq> >::type assert_policies(const TArgs&) { }

        template<typename TSeq, typename T, typename TArgs>
        static typename disable_if<mpl::empty<TSeq> >::type assert_policies(const TArgs& args) {
            typedef typename mpl::front<TSeq>::type policy;
            static_cast<const policy&>(args).template assert_policy<T>();
            assert_policies<typename mpl::pop_front<TSeq>::type, T>(args);
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
        creators::new_creator new_creator_; // todo remove
    };

    } // namespace core
    } // namespace di
    } // namespace boost

    #endif

#else

    template<
        typename T
      , typename TDependency
      , typename TCtor
      , typename TCallStack
      , typename TDeps
      , typename TRefs
      , typename TVisitor
      , typename TArgs
    >
    typename enable_if_c<mpl::size<TCtor>::value == BOOST_PP_ITERATION(), typename TDependency::expected*>::type
    create_impl(TDeps& deps, TRefs& refs, const TVisitor& visitor, const TArgs& args) {
        (void)deps;
        (void)refs;
        (void)visitor;
        (void)args;

        #define BOOST_DI_CREATOR_CREATE(_, n, create)   \
            BOOST_PP_COMMA_IF(n)                        \
            create<                                     \
                typename mpl::at_c<TCtor, n>::type      \
              , T                                       \
              , TCallStack                              \
            >(deps, refs, visitor, args)

        return new_creator_.create<typename TDependency::expected, typename TDependency::given>(
            BOOST_PP_REPEAT(
                BOOST_PP_ITERATION()
              , BOOST_DI_CREATOR_CREATE
              , create
            )
        );

        #undef BOOST_DI_CREATOR_CREATE
      }

#endif

