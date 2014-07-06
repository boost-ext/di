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
    #include "boost/di/type_traits/has_call.hpp"

    #include <vector>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/pop_front.hpp>
    #include <boost/mpl/front.hpp>
    #include <boost/mpl/contains.hpp>

    namespace boost {
    namespace di {
    namespace core {

    template<
        typename TDependecies = mpl::vector0<>
      , template<
            typename
          , template<typename, typename> class = ::boost::di::core::binder
          , template<
                typename
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
            typename = ::boost::mpl::vector0<>
          , typename = ::boost::di::core::never< ::boost::mpl::_1 >
          , typename = void
        > class TPool = pool
    >
    class module
        : public TPool<TDependecies>
    {
        template<
            typename
          , template<
                typename
              , template<typename, typename> class
              , template<
                    typename
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
        > friend class module;

        class empty_visitor
        {
        public:
            template<typename T>
            void operator()(const T&) const { }
        };

    public:
        typedef TDependecies deps;

        explicit module(const TCreator<TDependecies>& creator = TCreator<TDependecies>())
            : creator_(creator)
        { }

        #define BOOST_PP_FILENAME_1 "boost/di/core/module.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_TYPES_MPL_LIMIT_FROM(1)
        #include BOOST_PP_ITERATE()

        template<typename T>
        T create() {
            typedef mpl::vector0<> call_stack;
            std::vector<aux::shared_ptr<void> > refs_;

            return creator_.template create<T, T, call_stack>(
                static_cast<TPool<deps>&>(*this), refs_, empty_visitor(), TPool<>());
        }

        template<typename T, typename Visitor>
        T visit(const Visitor& visitor) {
            typedef mpl::vector0<> call_stack;
            std::vector<aux::shared_ptr<void> > refs_;

            return creator_.template create<T, T, call_stack>(
                static_cast<TPool<deps>&>(*this), refs_, visitor, TPool<>());
        }

        template<typename TAction>
        void call(const TAction& action) {
            call_impl<deps>(static_cast<TPool<deps>&>(*this), action);
        }

    private:
        template<typename TSeq, typename T, typename TAction>
        typename enable_if<mpl::empty<TSeq> >::type call_impl(T&, const TAction&) { }

        template<typename TSeq, typename T, typename TAction>
        typename disable_if<mpl::empty<TSeq> >::type call_impl(
            T& deps
          , const TAction& action
          , typename enable_if<type_traits::has_call<typename mpl::front<TSeq>::type, TAction> >::type* = 0) {
            static_cast<typename mpl::front<TSeq>::type&>(deps).call(action);
            call_impl<typename mpl::pop_front<TSeq>::type>(deps, action);
        }

        template<typename TSeq, typename T, typename TAction>
        typename disable_if<mpl::empty<TSeq> >::type call_impl(
            T& deps
          , const TAction& action
          , typename disable_if<type_traits::has_call<typename mpl::front<TSeq>::type, TAction> >::type* = 0) {
            call_impl<typename mpl::pop_front<TSeq>::type>(deps, action);
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
                  BOOST_DI_MPL_VECTOR_TYPES_PASS(TArgs)
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >(BOOST_DI_ARGS_PASS(args))
            , init()
          )
    { }

    template<typename T, BOOST_DI_TYPES(TArgs)>
    T create(BOOST_DI_ARGS(TArgs, args)) {
        typedef mpl::vector0<> call_stack;
        TPool<BOOST_DI_MPL_VECTOR_TYPES_PASS(TArgs)> args_(BOOST_DI_ARGS_PASS(args));
        std::vector<aux::shared_ptr<void> > refs_;

        return creator_.template create<T, T, call_stack>(
            static_cast<TPool<deps>&>(*this), refs_, empty_visitor(), args_
        );
    }

#endif

