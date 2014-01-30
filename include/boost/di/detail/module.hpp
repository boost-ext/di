//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_MODULE_HPP
    #define BOOST_DI_DETAIL_MODULE_HPP

    #include "boost/di/aux_/meta.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/detail/pool.hpp"
    #include "boost/di/detail/binder.hpp"
    #include "boost/di/detail/creator.hpp"
    #include "boost/di/detail/visitor.hpp"

	#include <vector>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/type.hpp>
    #include <boost/non_type.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/typeof/typeof.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/is_sequence.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/transform.hpp>
    #include <boost/mpl/set.hpp>
    #include <boost/mpl/remove_if.hpp>
    #include <boost/mpl/pop_front.hpp>
    #include <boost/mpl/front.hpp>
    #include <boost/mpl/push_back.hpp>
    #include <boost/mpl/insert.hpp>
    #include <boost/mpl/contains.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>
    #include <boost/mpl/has_xxx.hpp>
    #include <boost/mpl/assert.hpp>

    namespace boost {
    namespace di {
    namespace detail {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(is_policy)

    template<typename TSeq, typename TCond>
    struct get_types
        : mpl::remove_if<TSeq, mpl::not_<TCond> >::type
    { };

    template<typename TSeq>
    struct get_deps
        : get_types<
              TSeq
            , mpl::not_<has_is_policy<mpl::_> >
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

        template<typename T>
        class has_call_impl
        {
            struct base_impl { void call() { } };
            struct base : T, base_impl { base() { } };

            template<typename U>
            static mpl::aux::no_tag test(
                U*
              , non_type<void (base_impl::*)(), &U::call>* = 0
            );

            template<typename>
            static mpl::aux::yes_tag test(...);

        public:
            BOOST_STATIC_CONSTANT(
                bool
              , value = sizeof(test<base>(0)) == sizeof(mpl::aux::yes_tag)
            );
        };

        template<typename T, typename TAction>
        class has_call
        {
            template<typename>
            struct void_ { };

            template<typename S, typename U>
            friend U& operator,(const U&, void_<S>);

            struct base : T
            {
                base() { }
                using T::call;
                mpl::aux::no_tag call(...) const;
            };

            template<typename, typename = void>
            struct base_call
                : mpl::false_
            { };

            template<typename TDummy>
            struct base_call<mpl::true_, TDummy>
                : is_same<
                      BOOST_TYPEOF_TPL(
                         ((((base*)0)->call(*(TAction*)0)), void_<T>())
                      )
                    , void_<T>
                  >
            { };

        public :
            BOOST_STATIC_CONSTANT(
                bool
              , value = base_call<mpl::bool_<has_call_impl<T>::value> >::value
            );
        };

        template<typename TSeq>
        struct unique
            : mpl::fold<
                  typename mpl::fold<
                      TSeq
                    , mpl::set0<>
                    , mpl::insert<mpl::_1, mpl::_2>
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
            typename TSeq
          , typename T
          , typename TPolicy
        >
        struct verify_policy
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
                    , verify_policy<TSeq, T, mpl::_1>
                  >::type
              >
        { };

    public:
        typedef get_deps<TDeps> deps;
        typedef get_types<TDeps, has_is_policy<mpl::_> > policies;

        module() { }

		~module() {
			for (std::size_t i = 0; i < cleanup.size(); ++i) {
				cleanup[i]();
			}
		}

        #define BOOST_PP_FILENAME_1 "boost/di/detail/module.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_LIMITS_BEGIN(1)
        #include BOOST_PP_ITERATE()

        template<typename T>
        T create() {
            BOOST_MPL_ASSERT((typename verify_policies<policies, deps, T>::type));

            return TCreator<TBinder<deps> >::template
                execute<T, mpl::true_, mpl::vector0<> >(static_cast<TPool<deps>&>(*this), cleanup);
        }

        template<typename T, typename Visitor>
        void visit(const Visitor& visitor) {
            BOOST_MPL_ASSERT((typename verify_policies<policies, deps, T>::type));

            TVisitor<TBinder<deps> >::template
                execute<T, mpl::vector0<> >(visitor);
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
          , typename enable_if<has_call<typename mpl::front<TSeq>::type, TAction> >::type* = 0) {
            static_cast<typename mpl::front<TSeq>::type&>(deps).call(action);
            call_impl<typename mpl::pop_front<TSeq>::type>(deps, action);
        }

        template<typename TSeq, typename T, typename TAction>
        typename disable_if<mpl::empty<TSeq> >::type call_impl(
            T& deps
          , const TAction& action
          , typename disable_if<has_call<typename mpl::front<TSeq>::type, TAction> >::type* = 0) {
            call_impl<typename mpl::pop_front<TSeq>::type>(deps, action);
        }

        std::vector<void(*)()> cleanup;
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

