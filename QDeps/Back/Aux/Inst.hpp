//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_BACK_AUX_INST_HPP
    #define QDEPS_BACK_AUX_INST_HPP

    #include <boost/make_shared.hpp>
    #include <boost/shared_ptr.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include <boost/mpl/int.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/void.hpp>
    #include <boost/mpl/string.hpp>
    #include <boost/mpl/contains.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include "QDeps/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, QDEPS_FUNC_ARITY, "QDeps/Back/Aux/Inst.hpp"))

    namespace QDeps
    {
    namespace Back
    {
    namespace Aux
    {

    template<typename T>
    struct Value : boost::mpl::true_
    {
        template<typename TAttr, typename TPool, typename TScope>
        static boost::shared_ptr<TAttr> get(TPool& p_pool, TScope&, typename boost::enable_if< boost::mpl::contains<typename TPool::Seq, TAttr> >::type* = 0)
        {
            return p_pool.template get<TAttr>();
        }

        template<typename TAttr, typename TPool, typename TScope>
        static boost::shared_ptr<TAttr> get(TPool&, TScope& p_scope, typename boost::disable_if< boost::mpl::contains<typename TPool::Seq, TAttr> >::type* = 0)
        {
            return p_scope.template create<TAttr>();
        }
    };

    template<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, int C)>
    struct Value< boost::mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> > : boost::mpl::false_
    {
        template<typename TAttr, typename TPool, typename TScope> static boost::shared_ptr<TAttr> get(TPool&, TScope&)
        {
            return boost::make_shared<TAttr>(
                boost::mpl::c_str< boost::mpl::string<BOOST_PP_ENUM_PARAMS(BOOST_MPL_STRING_MAX_PARAMS, C)> >::value
            );
        }
    };

    template<int N>
    struct Value<boost::mpl::int_<N> > : boost::mpl::false_
    {
        template<typename TAttr, typename TPool, typename TScope> static boost::shared_ptr<TAttr> get(TPool&, TScope&)
        {
            return boost::make_shared<TAttr>(N);
        }
    };

    template
    <
        typename TScope,
        typename TAttr,
        typename TValue = boost::mpl::void_,
        typename TContext = boost::mpl::vector0<>,
        typename TDependency = boost::is_same<boost::mpl::_1, TAttr>
    >
    class Inst
    {
    public:
        typedef TContext Context;
        typedef TDependency Dependency;
        typedef typename Value<TValue>::type HasExternalValue;

        template<typename TPool> boost::shared_ptr<TAttr> create(TPool& p_pool)
        {
            return Value<TValue>::template get<TAttr>(p_pool, m_scope);
        }

        #include BOOST_PP_ITERATE()

    private:
        TScope m_scope;
    };

    } // namespace Aux
    } // namespace Back
    } // namespace QDeps

    #endif

#else

    template<typename TPool, BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    boost::shared_ptr<TAttr> create(TPool&, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg))
    {
        return m_scope.template create<TAttr>(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg));
    }

#endif

