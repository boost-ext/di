//
// Copyright (c) 2012-2013 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_MODULE_HPP
    #define BOOST_DI_DETAIL_MODULE_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/typeof/typeof.hpp>
    #include <boost/non_type.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/assert.hpp>
    #include <boost/mpl/is_sequence.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/transform.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/not.hpp>
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
    #include <boost/mpl/filter_view.hpp>
    #include <boost/mpl/contains.hpp>
    #include <boost/mpl/pair.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>
    #include <boost/mpl/has_xxx.hpp>

    #include "boost/di/detail/pool.hpp"
    #include "boost/di/detail/binder.hpp"
    #include "boost/di/detail/creator.hpp"
    #include "boost/di/detail/visitor.hpp"
    #include "boost/di/policy.hpp"
    #include "boost/di/config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 ( \
        BOOST_DI_ITERATION_PARAMS(1, "boost/di/detail/module.hpp"))

    namespace boost {
    namespace di {
    namespace detail {

    template<typename TSeq, typename TCond>
    struct get_types
        : mpl::remove_if<TSeq, mpl::not_<TCond> >::type
    { };

    template<typename TSeq>
    struct get_deps
        : get_types<
              TSeq
            , mpl::not_<is_base_of<detail::policy_impl, mpl::_> >
          >::type
    { };

    template<
        typename TDeps = mpl::vector0<>
      , template<typename> class TBinder = binder
      , template<
            typename
          , typename = ::boost::di::detail::never< ::boost::mpl::_1 >
          , typename = void
        > class TPool = pool
      , template<typename> class TCreator = creator
      , template<typename> class TVisitor = visitor
    >
    class module : public TPool<get_deps<TDeps> >
    {
        template<
            typename
          , template<typename> class
          , template<typename, typename, typename> class
          , template<typename> class
          , template<typename> class
        > friend class module;

        template<typename T, typename TAction>
        class has_call
        {
            struct base_impl { void call(const TAction&) { } };
            struct base : T, base_impl { base() { } };

            template<typename U>
            static mpl::aux::no_tag test(
                U*
              , non_type<void (base_impl::*)(const TAction&), &U::call>* = 0
            );

            template<typename>
            static mpl::aux::yes_tag test(...);

        public:
            BOOST_STATIC_CONSTANT(
                bool
              , value = sizeof(test<base>(0)) == sizeof(mpl::aux::yes_tag)
            );
        };

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

        template<typename T>
        struct ctor
            : type_traits::ctor_traits<
                  typename type_traits::make_plain<T>::type
              >::type
        { };

        template<
            typename T
          , typename TCallStack
          , typename TBind
        >
        struct binder
            : TBind::template get_dependency<T, TCallStack>::type
        { };

        template<
            typename T
          , typename TBind
          , typename TCallStack =
                mpl::vector1<typename type_traits::make_plain<T>::type>
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

        typedef is_base_of<detail::policy_impl, mpl::_> is_policy;

    public:
        typedef get_types<TDeps, is_policy> policies;
        typedef get_deps<TDeps> deps;

        module() { }

        #include BOOST_PP_ITERATE()

        template<typename T>
        T create() {
            BOOST_MPL_ASSERT((typename verify_policies<policies, deps, T>::type));

            typedef typename binder<T, mpl::vector0<>, TBinder<deps> >::type binder_t;

            typedef typename unique<
                mpl::joint_view<
                    deps
                  , mpl::joint_view<
                        mpl::vector1<binder_t>
                      , typename deps_impl<T, TBinder<deps> >::type
                    >
                >
            >::type deps_t;

            TPool<deps_t> deps_(static_cast<TPool<deps>&>(*this), init());

            return TCreator<TBinder<deps> >::template
                execute<T, mpl::vector0<> >(deps_);
        }

        template<typename T, typename Visitor>
        void visit(const Visitor& visitor) {
            BOOST_MPL_ASSERT((typename verify_policies<policies, deps, T>::type));

            TVisitor<TBinder<deps> >::template
                execute<T, mpl::vector0<> >(visitor);
        }

        template<typename TAction>
        void call(const TAction& action) {
            call_impl<deps>(action);
        }

    private:
        template<typename TSeq, typename TAction>
        typename enable_if<mpl::empty<TSeq> >::type call_impl(const TAction&) { }

        template<typename TSeq, typename TAction>
        typename disable_if<mpl::empty<TSeq> >::type call_impl(
            const TAction& action
          , typename enable_if<has_call<typename mpl::front<TSeq>::type, TAction> >::type* = 0) {
            typedef typename mpl::front<TSeq>::type type;
            static_cast<type&>(static_cast<TPool<deps>&>(*this)).call(action);
            call_impl<typename mpl::pop_front<TSeq>::type>(action);
        }

        template<typename TSeq, typename TAction>
        typename disable_if<mpl::empty<TSeq> >::type call_impl(
            const TAction& action
          , typename disable_if<has_call<typename mpl::front<TSeq>::type, TAction> >::type* = 0) {
            call_impl<typename mpl::pop_front<TSeq>::type>(action);
        }
    };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    //bind<...>, etc.   -> ignore
    //policy<...>       -> ignore
    //module<....>      -> get all dependencies from the module
    //dependency<....>  -> pass

    template<BOOST_DI_TYPES(Args)>
    explicit module(BOOST_DI_ARGS(Args, args))
        : TPool<deps>(
              TPool<
                  mpl::vector<BOOST_DI_TYPES_PASS(Args)>
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

#endif

