#ifndef BOOST_DI_DETAIL_POOL_HPP
#define BOOST_DI_DETAIL_POOL_HPP

    #include "boost/di_preprocessed/aux_/common.hpp"
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/apply.hpp>
    #include <boost/mpl/fold.hpp>
    #include <boost/mpl/copy.hpp>
    #include <boost/mpl/back_inserter.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/has_xxx.hpp>
    #include <boost/config.hpp>

    namespace boost {
    namespace di {
    namespace detail {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(types)

    class init { };

    template<typename T>
    struct never
        : mpl::false_
    { };

    template<
        typename TSeq = mpl::vector0<>
      , typename TIgnore = never<mpl::_1>
      , typename = void
    >
    class pool
    {
    public:
        typedef pool type;
        typedef TSeq types;

        pool() { }

        template<typename T>
        explicit pool(const pool<T>&, const init&)
        { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 1>::type>
        : public mpl::at_c<TSeq, 0>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 2>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 3>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 4>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 5>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 6>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 7>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 8>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 9>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 10>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 11>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type , public mpl::at_c<TSeq, 10>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 11 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 12>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type , public mpl::at_c<TSeq, 10>::type , public mpl::at_c<TSeq, 11>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 11 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 12 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 13>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type , public mpl::at_c<TSeq, 10>::type , public mpl::at_c<TSeq, 11>::type , public mpl::at_c<TSeq, 12>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 11 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 12 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 13 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 14>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type , public mpl::at_c<TSeq, 10>::type , public mpl::at_c<TSeq, 11>::type , public mpl::at_c<TSeq, 12>::type , public mpl::at_c<TSeq, 13>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 11 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 12 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 13 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 14 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 15>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type , public mpl::at_c<TSeq, 10>::type , public mpl::at_c<TSeq, 11>::type , public mpl::at_c<TSeq, 12>::type , public mpl::at_c<TSeq, 13>::type , public mpl::at_c<TSeq, 14>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 11 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 12 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 13 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 14 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 15 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 16>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type , public mpl::at_c<TSeq, 10>::type , public mpl::at_c<TSeq, 11>::type , public mpl::at_c<TSeq, 12>::type , public mpl::at_c<TSeq, 13>::type , public mpl::at_c<TSeq, 14>::type , public mpl::at_c<TSeq, 15>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 11 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 12 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 13 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 14 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 15 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 16 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 17>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type , public mpl::at_c<TSeq, 10>::type , public mpl::at_c<TSeq, 11>::type , public mpl::at_c<TSeq, 12>::type , public mpl::at_c<TSeq, 13>::type , public mpl::at_c<TSeq, 14>::type , public mpl::at_c<TSeq, 15>::type , public mpl::at_c<TSeq, 16>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15 , const Args16 &args16) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) , Args16(args16) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 11 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 12 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 13 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 14 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 15 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 16 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 17 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 16>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 16 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 18>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type , public mpl::at_c<TSeq, 10>::type , public mpl::at_c<TSeq, 11>::type , public mpl::at_c<TSeq, 12>::type , public mpl::at_c<TSeq, 13>::type , public mpl::at_c<TSeq, 14>::type , public mpl::at_c<TSeq, 15>::type , public mpl::at_c<TSeq, 16>::type , public mpl::at_c<TSeq, 17>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15 , const Args16 &args16) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) , Args16(args16) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15 , const Args16 &args16 , const Args17 &args17) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) , Args16(args16) , Args17(args17) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 11 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 12 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 13 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 14 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 15 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 16 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 17 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 16>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 16 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 18 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 16>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 16 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 17>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 17 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 19>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type , public mpl::at_c<TSeq, 10>::type , public mpl::at_c<TSeq, 11>::type , public mpl::at_c<TSeq, 12>::type , public mpl::at_c<TSeq, 13>::type , public mpl::at_c<TSeq, 14>::type , public mpl::at_c<TSeq, 15>::type , public mpl::at_c<TSeq, 16>::type , public mpl::at_c<TSeq, 17>::type , public mpl::at_c<TSeq, 18>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15 , const Args16 &args16) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) , Args16(args16) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15 , const Args16 &args16 , const Args17 &args17) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) , Args16(args16) , Args17(args17) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15 , const Args16 &args16 , const Args17 &args17 , const Args18 &args18) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) , Args16(args16) , Args17(args17) , Args18(args18) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 11 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 12 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 13 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 14 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 15 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 16 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 17 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 16>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 16 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 18 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 16>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 16 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 17>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 17 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 19 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 16>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 16 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 17>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 17 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 18>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 18 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    template<typename TSeq, typename TIgnore>
    class pool<TSeq, TIgnore, typename enable_if_c<mpl::size<TSeq>::value == 20>::type>
        : public mpl::at_c<TSeq, 0>::type , public mpl::at_c<TSeq, 1>::type , public mpl::at_c<TSeq, 2>::type , public mpl::at_c<TSeq, 3>::type , public mpl::at_c<TSeq, 4>::type , public mpl::at_c<TSeq, 5>::type , public mpl::at_c<TSeq, 6>::type , public mpl::at_c<TSeq, 7>::type , public mpl::at_c<TSeq, 8>::type , public mpl::at_c<TSeq, 9>::type , public mpl::at_c<TSeq, 10>::type , public mpl::at_c<TSeq, 11>::type , public mpl::at_c<TSeq, 12>::type , public mpl::at_c<TSeq, 13>::type , public mpl::at_c<TSeq, 14>::type , public mpl::at_c<TSeq, 15>::type , public mpl::at_c<TSeq, 16>::type , public mpl::at_c<TSeq, 17>::type , public mpl::at_c<TSeq, 18>::type , public mpl::at_c<TSeq, 19>::type

    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type;
        };

    public:
        typedef pool type;

        struct types
            : mpl::fold<
                  TSeq
                , mpl::vector0<>
                , mpl::copy<
                      pool_type<mpl::_2>
                    , mpl::back_inserter<mpl::_1>
                  >
              >::type
        { };

        pool() { }
        template< typename Args0> explicit pool( const Args0 &args0) : Args0(args0) { }

        template< typename Args0 , typename Args1> explicit pool( const Args0 &args0 , const Args1 &args1) : Args0(args0) , Args1(args1) { }

        template< typename Args0 , typename Args1 , typename Args2> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2) : Args0(args0) , Args1(args1) , Args2(args2) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15 , const Args16 &args16) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) , Args16(args16) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15 , const Args16 &args16 , const Args17 &args17) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) , Args16(args16) , Args17(args17) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15 , const Args16 &args16 , const Args17 &args17 , const Args18 &args18) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) , Args16(args16) , Args17(args17) , Args18(args18) { }

        template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19> explicit pool( const Args0 &args0 , const Args1 &args1 , const Args2 &args2 , const Args3 &args3 , const Args4 &args4 , const Args5 &args5 , const Args6 &args6 , const Args7 &args7 , const Args8 &args8 , const Args9 &args9 , const Args10 &args10 , const Args11 &args11 , const Args12 &args12 , const Args13 &args13 , const Args14 &args14 , const Args15 &args15 , const Args16 &args16 , const Args17 &args17 , const Args18 &args18 , const Args19 &args19) : Args0(args0) , Args1(args1) , Args2(args2) , Args3(args3) , Args4(args4) , Args5(args5) , Args6(args6) , Args7(args7) , Args8(args8) , Args9(args9) , Args10(args10) , Args11(args11) , Args12(args12) , Args13(args13) , Args14(args14) , Args15(args15) , Args16(args16) , Args17(args17) , Args18(args18) , Args19(args19) { }
        template<typename T, typename I>
        explicit pool(
            const pool<T, I>&
          , const init&
          , typename enable_if_c<
                mpl::size<typename pool<T, I>::types>::value == 0
            >::type* = 0)
        { }
        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 1 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 2 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 3 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 4 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 5 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 6 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 7 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 8 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 9 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 10 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 11 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 12 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 13 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 14 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 15 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 16 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 17 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 16>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 16 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 18 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 16>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 16 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 17>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 17 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 19 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 16>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 16 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 17>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 17 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 18>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 18 >::type >() ) { }

        template<typename T, typename I> explicit pool( const pool<T, I>& p , const init& , typename enable_if_c< mpl::size<typename pool<T, I>::types>::value == 20 >::type* = 0) : mpl::at_c<typename pool<T, I>::types, 0>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 0 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 1>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 1 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 2>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 2 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 3>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 3 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 4>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 4 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 5>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 5 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 6>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 6 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 7>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 7 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 8>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 8 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 9>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 9 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 10>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 10 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 11>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 11 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 12>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 12 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 13>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 13 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 14>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 14 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 15>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 15 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 16>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 16 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 17>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 17 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 18>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 18 >::type >() ) , mpl::at_c<typename pool<T, I>::types, 19>::type( p.template get< typename mpl::at_c< typename pool<T, I>::types , 19 >::type >() ) { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }
    };

    }
    }
    }

#endif

