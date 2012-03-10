//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_BACK_AUX_IMPL_HPP
    #define QDEPS_BACK_AUX_IMPL_HPP

    #include <boost/make_shared.hpp>
    #include <boost/shared_ptr.hpp>
    #include <boost/type_traits/is_same.hpp>
    #include <boost/mpl/vector.hpp>
    #include <boost/mpl/placeholders.hpp>
    #include <boost/mpl/int.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/string.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include "QDeps/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, QDEPS_FUNC_ARITY, "QDeps/Back/Aux/Impl.hpp"))

    namespace QDeps
    {
    namespace Back
    {
    namespace Aux
    {

    template
    <
        typename TScope,
        typename TIf,
        typename TImpl,
        typename TContext = boost::mpl::vector0<>,
        typename TDependency = boost::is_same<boost::mpl::_1, TIf>
    >
    class Impl
    {
    public:
        typedef TContext Context;
        typedef TDependency Dependency;
        typedef boost::mpl::false_ HasExternalValue;

        template<typename TPool> boost::shared_ptr<TIf> create(TPool&)
        {
            return m_scope.template create<TImpl>();
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
    boost::shared_ptr<TImpl> create(TPool&, BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), Arg, p_arg))
    {
        return m_scope.template create<TImpl>(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg));
    }

#endif

