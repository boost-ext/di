//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_CORE_MODULE_HPP
    #define BOOST_DI_CORE_MODULE_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/core/pool.hpp"
    #include "boost/di/core/creator.hpp"
    #include "boost/di/core/allocator.hpp"
    #include "boost/di/type_traits/has_call.hpp"

    #include <vector>
    #include <boost/utility/enable_if.hpp>

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

        #define BOOST_PP_FILENAME_1 "boost/di/core/module.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_TYPES_MPL_LIMIT_FROM(1)
        #include BOOST_PP_ITERATE()

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

    #endif

#else

    // bind<...>, etc.   -> ignore
    // module<....>      -> get all dependencies from the module
    // dependency<....>  -> pass

    template<BOOST_DI_TYPES(TArgs)>
    explicit module(BOOST_DI_ARGS(TArgs, args))
        : TPool<deps>(
              TPool<
                  typename aux::mpl::normalize_vector<BOOST_DI_MPL_VECTOR_TYPES_PASS(TArgs)>::type
                , aux::mpl::not_<
                      aux::mpl::or_<
                          aux::mpl::contains<deps, aux::mpl::_>
                        , has_types<aux::mpl::_>
                      >
                  >
              >(BOOST_DI_ARGS_PASS(args))
            , init()
          )
    { }

    template<typename T, BOOST_DI_TYPES(TPolicies)>
    T create(BOOST_DI_ARGS(TPolicies, policies)) {
        typedef aux::mpl::vector<> call_stack;
        TPool<BOOST_DI_MPL_VECTOR_TYPES_PASS(TPolicies)> policies_(BOOST_DI_ARGS_PASS(policies));
        std::vector<aux::shared_ptr<void> > refs_;

        return creator_.template create<T, T, call_stack>(
            TDefaultAllocator()
          , static_cast<TPool<deps>&>(*this)
          , refs_
          , empty_visitor()
          , policies_
        );
    }

    template<typename T, typename TAllocator, BOOST_DI_TYPES(TPolicies)>
    T allocate(const TAllocator& allocator, BOOST_DI_ARGS(TPolicies, policies)) {
        typedef aux::mpl::vector<> call_stack;
        TPool<BOOST_DI_MPL_VECTOR_TYPES_PASS(TPolicies)> policies_(BOOST_DI_ARGS_PASS(policies));
        std::vector<aux::shared_ptr<void> > refs_;

        return creator_.template create<T, T, call_stack>(
            allocator
          , static_cast<TPool<deps>&>(*this)
          , refs_
          , empty_visitor()
          , policies_
        );
    }

#endif

