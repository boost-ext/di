//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_DETAIL_MODULE_HPP
    #define BOOST_DI_DETAIL_MODULE_HPP

    #include "boost/di/aux_/config.hpp"
    #include "boost/di/aux_/memory.hpp"
    #include "boost/di/detail/pool.hpp"
    #include "boost/di/detail/creator.hpp"

    #include <vector>
    #include <boost/non_type.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/pop_front.hpp>
    #include <boost/mpl/front.hpp>
    #include <boost/mpl/contains.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>
    #include <boost/mpl/has_xxx.hpp>

    namespace boost {
    namespace di {
    namespace detail {

    template<
        typename TDependecies = mpl::vector0<>
      , template<
            typename
          , template<typename, typename> class = ::boost::di::detail::binder
          , typename = ::boost::di::detail::default_create_policy
        > class TCreator = creator
      , template<
            typename
          , typename = ::boost::di::detail::never< ::boost::mpl::_1 >
          , typename = void
        > class TPool = pool
    >
    class module
        : public TPool<TDependecies>
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(scope)

        template<
            typename
          , template<typename, template<typename, typename> class, typename> class
          , template<typename, typename, typename> class
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
            typedef has_call_impl type;

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
                      BOOST_DI_FEATURE_DECLTYPE(
                         ((((base*)0)->call(*(TAction*)0)), void_<T>())
                      )
                    , void_<T>
                  >
            { };

        public :
            typedef has_call type;

            BOOST_STATIC_CONSTANT(
                bool
              , value = base_call<mpl::bool_<has_call_impl<T>::value> >::value
            );
        };

        class empty_visitor
        {
        public:
            template<typename T>
            void operator()(const T&) const { }
        };

    public:
        typedef TDependecies deps;

        explicit module(TCreator<TDependecies> creator = TCreator<TDependecies>())
            : creator_(creator)
        { }

        #define BOOST_PP_FILENAME_1 "boost/di/detail/module.hpp"
        #define BOOST_PP_ITERATION_LIMITS BOOST_DI_TYPES_MPL_LIMIT_FROM(1)
        #include BOOST_PP_ITERATE()

        template<typename T>
        T create() {
            typedef mpl::vector0<> policies;
            typedef mpl::vector0<> call_stack;
            std::vector<boost::any> refs_;

            return creator_.template create<T, T, call_stack, policies>(
                static_cast<TPool<deps>&>(*this), refs_, empty_visitor());
        }

        template<typename T, typename Visitor>
        T visit(const Visitor& visitor) {
            typedef mpl::vector0<> policies;
            typedef mpl::vector0<> call_stack;
            std::vector<boost::any> refs_;

            return creator_.template create<T, T, call_stack, policies>(
                static_cast<TPool<deps>&>(*this), refs_, visitor);
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

        TCreator<TDependecies> creator_;
    };

    } // namespace detail
    } // namespace di
    } // namespace boost

    #endif

#else

    // bind<...>, etc.   -> ignore
    // module<....>      -> get all dependencies from the module
    // dependency<....>  -> pass

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

    template<typename T, BOOST_DI_TYPES(Args)>
    T create(BOOST_DI_ARGS_NOT_USED(Args)) {
        typedef mpl::vector<BOOST_DI_TYPES_PASS(Args)> policies;
        typedef mpl::vector0<> call_stack;
        std::vector<boost::any> refs_;

        return creator_.template create<T, T, call_stack, policies>(
            static_cast<TPool<deps>&>(*this), refs_, empty_visitor()
        );
    }

#endif

