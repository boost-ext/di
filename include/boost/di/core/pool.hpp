//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef BOOST_DI_CORE_POOL_HPP
    #define BOOST_DI_CORE_POOL_HPP

    #include "boost/di/aux_/config.hpp"

    #include <boost/preprocessor/iteration/local.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/config.hpp>

    #include "boost/di/type_traits/ctor_traits.hpp"

    #if defined(BOOST_GCC) || defined(BOOST_CLANG)
        #pragma GCC diagnostic ignored "-Wreorder"
    #endif

    namespace boost {
    namespace di {
    namespace core {

    BOOST_MPL_HAS_XXX_TRAIT_DEF(types)

    class init { };

    template<typename>
    struct never
        : aux::mpl::false_
    { };

    template<
        typename TSeq = aux::mpl::vector<>
      , typename TIgnore = never<aux::mpl::arg<0> >
      , typename = void
    >
    class pool;

    #define BOOST_PP_FILENAME_1 "boost/di/core/pool.hpp"
    #define BOOST_PP_ITERATION_LIMITS BOOST_DI_TYPES_MPL_LIMIT_FROM(1)
    #include BOOST_PP_ITERATE()

    } // namespace core
    } // namespace di
    } // namespace boost

    #endif

#else
    template<typename TIgnore, typename... TArgs>
    class pool<aux::mpl::vector<TArgs...>, TIgnore> : public TArgs...
    {
        template<typename T, typename = void>
        struct pool_type
        {
            typedef aux::mpl::vector<T> type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<has_types<T> >::type>
        {
            typedef typename T::types type;
        };

        template<typename T>
        struct pool_type<T, typename enable_if<
            typename aux::mpl::apply<TIgnore, T>::type>::type>
        {
            typedef void type; // ignore type
        };

    public:
        typedef pool type;

        struct types
            : aux::mpl::fold<
                  aux::mpl::vector<TArgs...>
                , aux::mpl::vector<>
                , aux::mpl::copy<
                      pool_type<aux::mpl::arg<1> >
                    , aux::mpl::back_inserter<aux::mpl::arg<0>>
                  >
              >::type
        { };

        template<typename... TQ>
        explicit pool(const TQ&... args)
            : TQ(args)...
        { }

        template<typename I, typename T>
        pool(const pool<T, I>& p, const init&)
            : pool(p, typename normalize_vector<typename pool<T, I>::types>::type(), init())
        { }

        template<typename T>
        const T& get() const {
            return static_cast<const T&>(*this);
        }

    private:
        template<typename I, typename T, typename... TQ>
        pool(const pool<T, I>& p, const aux::mpl::vector<TQ...>&, const init&)
            : TQ(p.template get<TQ>())...
        { }
    };

#endif

