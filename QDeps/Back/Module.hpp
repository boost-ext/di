//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_MODULE_HPP
#define QDEPS_BACK_MODULE_HPP

#include <boost/make_shared.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/has_xxx.hpp>
#include "QDeps/Back/Aux/Shared/Convert.hpp"
#include "QDeps/Back/Aux/Shared/Make.hpp"
#include "QDeps/Back/Aux/Scopes/Singleton.hpp"
#include "QDeps/Back/Aux/Scopes/PerRequest.hpp"
#include "QDeps/Back/Aux/Bind.hpp"
#include "QDeps/Back/Aux/Impl.hpp"
#include "QDeps/Back/Aux/Inst.hpp"
#include "QDeps/Back/Detail/Creator.hpp"

namespace QDeps
{
namespace Back
{

template
<
    typename TDeps,
    //typename TPolicy //TODO allow only shared ptr
    template<typename> class TConvert = Aux::Shared::Convert,
    template<typename, typename = TDeps> class TCreator = Detail::Creator
>
class Module
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(ctor__)

    template<typename T> struct PlainType
    {
        typedef typename boost::remove_cv<typename boost::remove_reference<T>::type>::type type;
    };

public:
    template<typename T> T create()
    {
        typedef typename Aux::Shared::Make<typename PlainType<T>::type>::type Type;
        return TConvert<T>::execute(createImpl<Type>(), m_deps);
    }

    //TODO get dependency in case Module owns it
    //template<typename T> T get()

private:
    template<typename T>
    T createImpl(typename boost::enable_if< has_ctor__<typename T::element_type> >::type* = 0)
    {
        typedef typename T::element_type Type;
        typedef typename boost::function_types::parameter_types<typename Type::ctor__::type>::type Ctor;
        return createImpl<T, Ctor>();
    }

    template<typename T>
    T createImpl(typename boost::disable_if< has_ctor__<typename T::element_type> >::type* = 0,
                 typename boost::disable_if< boost::mpl::empty<typename TCreator<typename T::element_type>::type> >::type* = 0)
    {
        //on top is the best match
        typedef typename TCreator<typename T::element_type>::type Creator;
        typedef typename boost::mpl::deref<typename boost::mpl::begin<Creator>::type>::type BestMatch;
        typedef typename Aux::Shared::Make<typename BestMatch::Implementation>::type Type;
        return createImpl<Type>();
    }

    template<typename T>
    T createImpl(typename boost::disable_if< has_ctor__<typename T::element_type> >::type* = 0,
                 typename boost::enable_if< boost::mpl::empty<typename TCreator<typename T::element_type>::type> >::type* = 0)
    {
        typedef typename T::element_type Type;
        return boost::make_shared<Type>(); //default Ctor
    }

#   define QDEPS_TYPEDEF_ARG_IMPL(z, n, Ctor)                                                           \
        typedef typename boost::mpl::at_c<Ctor, n>::type Arg##n;

#   define QDEPS_ARG_IMPL(z, n, p_deps) BOOST_PP_COMMA_IF(n)                                            \
        TConvert<Arg##n>::execute(                                                                      \
            createImpl<typename Aux::Shared::Make<typename PlainType<Arg##n>::type>::type>(),           \
            p_deps                                                                                      \
        )

#   define QDEPS_CREATE_IMPL(z, n, p_deps)                                                              \
        template<typename T, typename Ctor>                                                             \
        T createImpl(typename boost::enable_if_c<boost::mpl::size<Ctor>::value == n>::type* = 0)        \
        {                                                                                               \
            BOOST_PP_REPEAT(n, QDEPS_TYPEDEF_ARG_IMPL, Ctor)                                            \
                                                                                                        \
            return boost::make_shared<typename T::element_type>(                                        \
                BOOST_PP_REPEAT(n, QDEPS_ARG_IMPL, p_deps)                                              \
            );                                                                                          \
        }                                                                                               \

    BOOST_PP_REPEAT_FROM_TO(1, BOOST_MPL_LIMIT_VECTOR_SIZE, QDEPS_CREATE_IMPL, m_deps)

#   undef QDEPS_CREATE_IMPL
#   undef QDEPS_ARG_IMPL
#   undef QDEPS_TYPEDEF_ARG_IMPL

private:
    std::vector< boost::shared_ptr<void> > m_deps; //to keep shared ownership
};

} // namespace Back
} // namespace QDeps

#endif

