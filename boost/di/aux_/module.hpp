//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_AUX_MODULE_HPP
    #define BOOST_DI_AUX_MODULE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/filter_view.hpp>
    #include <boost/mpl/joint_view.hpp>
    #include <boost/mpl/inherit_linearly.hpp>
    #include <boost/mpl/inherit.hpp>
    #include <boost/mpl/remove_if.hpp>
    #include <boost/mpl/deref.hpp>
    #include <boost/mpl/begin_end.hpp>
    #include <boost/mpl/pop_front.hpp>
    #include <boost/mpl/front.hpp>
    #include <boost/mpl/and.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/equal_to.hpp>
    #include "boost/di/aux/pool.hpp"
    #include "boost/di/aux/has_traits.hpp"
    #include "boost/di/detail/creator.hpp"
    #include "boost/di/detail/visitor.hpp"
    #include "boost/di/policy.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_MPL_LIMIT_VECTOR_SIZE     \
          , "boost/di/aux_/module.hpp"      \
        )                                   \
    )

    namespace boost {
    namespace di {
    namespace aux_ {

    template<
        typename TDeps = mpl::vector0<>
      , typename TExternals = TDeps
      , template<
            typename
          , typename = void
        > class TPool = pool
      , template<
            typename
        > class TCreator = creator
      , template<
            typename
        > class TVisitor = visitor
    >
    class module
    {
        template<
            typename
          , typename
          , template<typename, typename> class
          , template<typename> class
          , template<typename> class
        > friend class module;

        template<typename TSeq>
        struct is_module
            : mpl::equal_to<
                  mpl::size<TSeq>
                , typename mpl::fold<
                      TSeq
                    , mpl::int_<0>
                    , mpl::if_<
                          has_deps<mpl::_2>
                        , mpl::next<mpl::_1>
                        , mpl::_1
                      >
                  >::type
              >
        { };

        template<typename T, typename = void>
        struct deps_impl
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct deps_impl<T, typename enable_if<has_deps<T> >::type>
        {
            typedef typename T::deps type;
        };

        template<typename T, typename = void>
        struct pool_impl
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_impl<T, typename enable_if<has_pool<T> >::type>
        {
            typedef mpl::vector<typename T::pool> type;
        };

    public:
        struct deps
            : mpl::remove_if<
                  typename mpl::fold<
                      TDeps
                    , mpl::vector0<>
                    , mpl::copy<
                          deps_impl<mpl::_2>
                        , mpl::back_inserter<boost::mpl::_1>
                      >
                  >::type
                , mpl::or_<
                      is_base_of<policy, mpl::_1>
                    , has_element_type<mpl::_1>
                  >
              >::type
        { };

        struct policies
            : mpl::deref<
                  typename mpl::begin<
                       mpl::joint_view<
                          mpl::filter_view<TDeps, is_base_of<policy, mpl::_1> >
                        , mpl::vector1<typename defaults<policy, specialized>::type>
                      >
                  >::type
              >::type
        { };

        typedef TPool<
            typename mpl::fold<
                TExternals
              , mpl::vector0<>
              , mpl::copy<
                    pool_impl<mpl::_2>
                  , mpl::back_inserter<boost::mpl::_1>
                >
            >::type
        > pool;

    private:
        struct entries
            : mpl::inherit_linearly<deps, mpl::inherit<mpl::_1, mpl::_2> >::type
        { };

    public:
        module() { }

        #include BOOST_PP_ITERATE()

        template<typename T>
        T create() {
            typedef typename policies::template verify<deps, T>::type policies_t;
            return TCreator<deps>::template execute<T, mpl::vector0<> >(entries_, pool_);
        }

        template<typename T, typename Visitor>
        void visit(const Visitor& visitor) {
            typedef typename policies::template verify<deps, T>::type policies_t;
            TVisitor<deps>::template execute<T, mpl::vector0<> >(visitor);
        }

        template<typename Scope, typename Action>
        void call(const Action& action) {
            call_impl<Scope, deps>(action);
        }

    private:
        template<
            typename Scope
          , typename Deps
          , typename Action
        >
        typename enable_if<mpl::empty<Deps> >::type
        call_impl(const Action&) { }

        template<
            typename Scope
          , typename Deps
          , typename Action
        >
        typename enable_if<
            mpl::and_<
                mpl::not_<mpl::empty<Deps> >
              , is_same<typename mpl::front<Deps>::type::scope, Scope>
            >
        >::type
        call_impl(const Action& action) {
            typedef typename mpl::front<Deps>::type type;
            static_cast<type&>(entries_).call(action);
            call_impl<Scope, typename mpl::pop_front<Deps>::type>(action);
        }

        pool pool_;
        entries entries_;
    };

    } // namespace aux_
    } // namespace di
    } // namespace boost

    #endif

#else

    template<BOOST_DI_TYPES(Args)>
    explicit module(
        BOOST_DI_ARGS(Args, args)
      , typename enable_if<is_module<mpl::vector<BOOST_DI_TYPES_PASS(Args)> > >::type* = 0)
        : pool_(BOOST_PP_ENUM_BINARY_PARAMS(
              BOOST_PP_ITERATION()
            , args
            , .pool_ BOOST_PP_INTERCEPT
          ))
    { }

    template<BOOST_DI_TYPES(Args)>
    explicit module(
        BOOST_DI_ARGS(Args, args)
      , typename disable_if<is_module<mpl::vector<BOOST_DI_TYPES_PASS(Args)> > >::type* = 0)
        : pool_(BOOST_DI_ARGS_FORWARD(args))
    { }

#endif

