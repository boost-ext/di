//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_MODULE_HPP
    #define BOOST_DI_DETAIL_MODULE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/facilities/intercept.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/typeof/typeof.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/assert.hpp>
    #include <boost/mpl/is_sequence.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/transform.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/set.hpp>
    #include <boost/mpl/remove_if.hpp>
    #include <boost/mpl/count_if.hpp>
    #include <boost/mpl/pop_front.hpp>
    #include <boost/mpl/front.hpp>
    #include <boost/mpl/not.hpp>
    #include <boost/mpl/equal_to.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/insert.hpp>
    #include <boost/mpl/joint_view.hpp>
    #include <boost/mpl/pair.hpp>
    #include <boost/mpl/has_xxx.hpp>

    #include "boost/di/detail/pool.hpp"
    #include "boost/di/detail/binder.hpp"
    #include "boost/di/detail/creator.hpp"
    #include "boost/di/detail/visitor.hpp"
    #include "boost/di/policy.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (   \
        BOOST_DI_ITERATION_PARAMS(          \
            1                               \
          , BOOST_DI_LIMIT_SIZE             \
          , "boost/di/detail/module.hpp"    \
        )                                   \
    )

    namespace boost {
    namespace di {
    namespace detail {

    template<
        typename TDeps = mpl::vector0<>
      , template<typename> class TBinder = binder
      , template<typename, typename = void> class TPool = pool
      , template<typename> class TCreator = creator
      , template<typename> class TVisitor = visitor
    >
    class module
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(deps)

        template<
            typename
          , template<typename> class
          , template<typename, typename> class
          , template<typename> class
          , template<typename> class
        > friend class module;

        template<typename TSeq>
        struct unique
            : mpl::fold<
                  typename mpl::fold<
                      TSeq
                    , mpl::set0<>
                    , mpl::insert<mpl::_1, mpl::_2 >
                  >::type
                , mpl::vector0<>
                , mpl::push_back<mpl::_1, mpl::_2>
              >
        { };

        template<
            typename TSeq
          , typename TLambda
        >
        struct get_types
            : mpl::remove_if<TSeq, TLambda>::type
        { };

        template<typename T>
        struct deps_type
        {
            typedef typename T::deps type;
        };

        template<typename TSeq>
        struct is_module
            : mpl::equal_to<
                  mpl::size<TSeq>
                , mpl::count_if<TSeq, has_deps<mpl::_> >
              >::type
        { };

        template<typename TSeq>
        struct get_deps
            : unique<
                  typename mpl::fold<
                      get_types<TSeq, is_base_of<detail::policy_impl, mpl::_> >
                    , mpl::vector0<>
                    , mpl::copy<
                          mpl::if_<
                              has_deps<mpl::_2>
                            , deps_type<mpl::_2>
                            , mpl::vector1<mpl::_2>
                          >
                        , mpl::back_inserter<mpl::_1>
                      >
                  >::type
              >::type
        { };

        template<typename T>
        struct ctor
            : type_traits::ctor_traits<typename type_traits::make_plain<T>::type>::type
        { };

        template<
            typename T
          , typename TCallStack
          , typename TBind
        >
        struct binder
            : TBind::template impl<T, TCallStack>::type
        { };

        template<
            typename T
          , typename TBind
          , typename TCallStack = mpl::vector1<typename type_traits::make_plain<T>::type>
        >
        struct deps_impl
            : unique<
                  typename mpl::fold<
                      ctor<typename binder<T, TCallStack, TBind>::given>
                    , mpl::vector0<>
                    , mpl::copy<
                          mpl::joint_view<
                              mpl::vector1<binder<mpl::_2, TCallStack, TBind> >
                            , deps_impl<
                                  mpl::_2
                                , TBind
                                , mpl::push_back<
                                      TCallStack
                                    , type_traits::make_plain<mpl::_2>
                                  >
                              >
                          >
                        , mpl::back_inserter<mpl::_1>
                      >
                  >::type
              >::type
        { };

        template<
            typename TSeq
          , typename T
          , typename TPolicy
        >
        struct verify_policies_impl
            : TPolicy::template verify<TSeq, T>::type
        { };

        template<
            typename TPolicies
          , typename TSeq
          , typename T
        >
        struct verify_policies
            : mpl::is_sequence<
                  typename mpl::transform<
                      TPolicies
                    , verify_policies_impl<TSeq, T, mpl::_1>
                  >::type
              >
        { };

    public:
        typedef get_types<TDeps, mpl::not_<is_base_of<detail::policy_impl, mpl::_> > > policies;
        typedef get_deps<TDeps> deps;

        module() { }

        #include BOOST_PP_ITERATE()

        template<typename T>
        T create() {
            BOOST_MPL_ASSERT((typename verify_policies<policies, deps, T>::type));

            TPool<
                typename unique<
                    mpl::joint_view<
                        deps
                      , mpl::joint_view<
                            mpl::vector1<typename binder<T, mpl::vector0<>, TBinder<deps> >::type>
                          , typename deps_impl<T, TBinder<deps> >::type
                        >
                    >
                >::type
            > deps_(this->deps_);

            return TCreator<TBinder<deps> >::template
                execute<T, mpl::vector0<> >(deps_);
        }

        template<typename T, typename Visitor>
        void visit(const Visitor& visitor) {
            BOOST_MPL_ASSERT((typename verify_policies<policies, deps, T>::type));

            TVisitor<TBinder<deps> >::template
                execute<T, mpl::vector0<> >(visitor);
        }

        template<typename Scope, typename Action>
        void call(const Action& action) {
            TPool<deps> deps_;
            call_impl<Scope, deps>(action, deps_);
        }

    private:
        template<
            typename Scope
          , typename Deps
          , typename Action
          , typename T
        >
        typename enable_if<mpl::empty<Deps> >::type
        call_impl(const Action&, T&) { }

        template<
            typename Scope
          , typename Deps
          , typename Action
          , typename T
        >
        typename enable_if<
            mpl::and_<
                mpl::not_<mpl::empty<Deps> >
              , is_same<typename mpl::front<Deps>::type::scope, Scope>
            >
        >::type
        call_impl(const Action& action, T& deps) {
            typedef typename mpl::front<Deps>::type type;
            static_cast<type&>(deps).call(action);
            call_impl<Scope, typename mpl::pop_front<Deps>::type>(action, deps);
        }

        TPool<deps> deps_;
    };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    #define BOOST_DI_GET_DEPS(_, n, args) \
        BOOST_PP_COMMA_IF(n) BOOST_TYPEOF(args##n.deps_)

    template<BOOST_DI_TYPES(Args)>
    explicit module(
        BOOST_DI_ARGS(Args, args)
      , typename enable_if<is_module<mpl::vector<BOOST_DI_TYPES_PASS(Args)> > >::type* = 0)
        : deps_(TPool<mpl::vector<BOOST_PP_REPEAT(BOOST_PP_ITERATION(), BOOST_DI_GET_DEPS, args)> >(
              BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), args, .deps_ BOOST_PP_INTERCEPT))
          )
    { }

    #undef BOOST_DI_GET_DEPS

    template<BOOST_DI_TYPES(Args)>
    explicit module(
        BOOST_DI_ARGS(Args, args)
      , typename disable_if<is_module<mpl::vector<BOOST_DI_TYPES_PASS(Args)> > >::type* = 0)
        : deps_(BOOST_DI_ARGS_FORWARD(args))
    { }

#endif

