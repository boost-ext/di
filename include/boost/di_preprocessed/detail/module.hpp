#ifndef BOOST_DI_DETAIL_MODULE_HPP
#define BOOST_DI_DETAIL_MODULE_HPP

    #include "boost/di_preprocessed/aux_/common.hpp"
    #include "boost/di_preprocessed/aux_/memory.hpp"
    #include "boost/di_preprocessed/detail/pool.hpp"
    #include "boost/di_preprocessed/detail/binder.hpp"
    #include "boost/di_preprocessed/detail/creator.hpp"
    #include <typeinfo>
    #include <vector>
    #include <map>
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

    template<
        typename TDeps = mpl::vector0<>
      , template<typename> class TBinder = binder
      , template<
            typename
          , typename = ::boost::di::detail::never< ::boost::mpl::_1 >
          , typename = void
        > class TPool = pool
      , template<typename> class TCreator = creator
    >
    class module : public TPool<TDeps>
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(scope)

        template<
            typename
          , template<typename> class
          , template<typename, typename, typename> class
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
                      BOOST_TYPEOF_TPL(
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

        template<typename T>
        class has_assert_policy
        {
            struct base_impl { static void assert_policy() { }; };

            struct base
                : base_impl
                , mpl::if_<is_class<T>, T, mpl::void_>::type
            { base() { } };

            template<typename U>
            static mpl::aux::no_tag test(
                U*
              , non_type<void(*)(), &U::assert_policy>* = 0
            );

            static mpl::aux::yes_tag test(...);

        public:
            typedef has_assert_policy type;

            BOOST_STATIC_CONSTANT(
                bool
              , value = !has_scope<T>::value &&
                        sizeof(test((base*)(0))) == sizeof(mpl::aux::yes_tag)
            );
        };

        template<
            typename T
          , typename TCallStack
          , typename TBind
        >
        struct binder
            : TBind::template resolve<T, TCallStack>::type
        { };

        class empty_visitor
        {
        public:
            template<typename T>
            void operator()(const T&) const { }
        };

        class type_comparator
        {
        public:
            bool operator ()(const std::type_info* lhs, const std::type_info* rhs) const {
                return lhs->before(*rhs);
            }
        };

    public:
        typedef TDeps deps;

        module() { }




    template< typename Args0>
    explicit module( Args0 args0)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0)
            , init()
          )
    { }

    template<typename T, typename Args0>
    T create( Args0 ) {
        typedef mpl::vector< Args0> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1>
    explicit module( Args0 args0 , Args1 args1)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1>
    T create( Args0 , Args1 ) {
        typedef mpl::vector< Args0 , Args1> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2>
    T create( Args0 , Args1 , Args2 ) {
        typedef mpl::vector< Args0 , Args1 , Args2> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3>
    T create( Args0 , Args1 , Args2 , Args3 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    explicit module( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18 , Args19 args19)
        : TPool<deps>(
              TPool<
                  mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19>
                , mpl::not_<
                      mpl::or_<
                          mpl::contains<deps, mpl::_>
                        , has_types<mpl::_>
                      >
                  >
              >( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18 , args19)
            , init()
          )
    { }

    template<typename T, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    T create( Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19 ) {
        typedef mpl::vector< Args0 , Args1 , Args2 , Args3 , Args4 , Args5 , Args6 , Args7 , Args8 , Args9 , Args10 , Args11 , Args12 , Args13 , Args14 , Args15 , Args16 , Args17 , Args18 , Args19> policies;
        std::vector<aux::shared_ptr<void> > refs_;

        return TCreator<TBinder<deps> >::template
            execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
    }

        template<typename T>
        T create() {
            typedef mpl::vector0<> policies;
            std::vector<aux::shared_ptr<void> > refs_;

            return TCreator<TBinder<deps> >::template
                execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, empty_visitor())(boost::type<T>());
        }

        template<typename T, typename Visitor>
        T visit(const Visitor& visitor) {
            typedef mpl::vector0<> policies;
            std::vector<aux::shared_ptr<void> > refs_;

            return TCreator<TBinder<deps> >::template
                execute<T, T, mpl::vector0<>, policies>(static_cast<TPool<deps>&>(*this), scopes_, refs_, visitor)(boost::type<T>());
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

        std::map<const std::type_info*, aux::shared_ptr<void>, type_comparator> scopes_;
    };

    }
    }
    }

#endif

