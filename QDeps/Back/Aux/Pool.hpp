//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_BACK_AUX_POOL_HPP
#define QDEPS_BACK_AUX_POOL_HPP

#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/non_type.hpp>

#   ifdef __GNUC__
#       pragma GCC system_header
#   endif

#   define QDEPS_SEQ_VARIADIC_ELEM(seq) QDEPS_SEQ_ELEM_0 seq
#   define QDEPS_SEQ_ELEM_0(...) QDEPS_SEQ_ELEM_1
#   define QDEPS_SEQ_ELEM_1(...) QDEPS_SEQ_ELEM_2
#   define QDEPS_SEQ_ELEM_2(...) __VA_ARGS__
#   define QDEPS_PARAMS_IMPL(z, n, param) BOOST_PP_COMMA_IF(n) const param##n& p_##n
#   define QDEPS_BASE_PARAMS_IMPL(z, n, param) BOOST_PP_COMMA_IF(n) p_##n
#   define QDEPS_CTOR_ARGS(r, n, elem)                                                                              \
        BOOST_PP_IF(BOOST_PP_EQUAL(r, 2), BOOST_PP_EMPTY, BOOST_PP_COMMA)()                                         \
        BOOST_PP_IF(BOOST_PP_EQUAL(r, 2), elem(BOOST_PP_REPEAT(n, QDEPS_BASE_PARAMS_IMPL, TCtor)), elem)

#   define QDEPS_CTOR_DEF_IMPL(z, n, ctor)                                                                              \
        template<BOOST_PP_ENUM_PARAMS(n, typename TCtor)> BOOST_PP_SEQ_ELEM(0, ctor)                                \
        (BOOST_PP_REPEAT(n, QDEPS_PARAMS_IMPL, TCtor))                                                              \
            : BOOST_PP_SEQ_FOR_EACH(QDEPS_CTOR_ARGS, n, BOOST_PP_SEQ_ELEM(1, ctor))                                 \
        QDEPS_SEQ_VARIADIC_ELEM(ctor)

#   define QDEPS_CTOR(Class, Seq, ...)                                                                              \
        Class() : BOOST_PP_SEQ_FOR_EACH(QDEPS_CTOR_ARGS, 0, Seq) __VA_ARGS__                                                                                         \
        BOOST_PP_REPEAT_FROM_TO(1, BOOST_MPL_LIMIT_VECTOR_SIZE, QDEPS_CTOR_DEF_IMPL, (Class)(Seq)(__VA_ARGS__))

namespace QDeps
{
namespace Back
{
namespace Aux
{

namespace Detail
{
template<typename TSeq, int Value, typename Enable = void>
struct Parameter
{
    typedef typename boost::mpl::at_c<TSeq, Value>::type type;
};

template<typename TSeq, int Value>
struct Parameter<TSeq, Value, typename boost::enable_if_c<(Value >= boost::mpl::size<TSeq>::value)>::type>
{
    typedef boost::non_type<int, Value> type;
};
} // namespace Detail

#define QDEPS_MEMBER_IMPL(_, n, seq) BOOST_PP_COMMA_IF(n) Detail::Parameter<seq, n>::type

template<typename TSeq = boost::mpl::vector0<> >
struct Pool : BOOST_PP_REPEAT(BOOST_MPL_LIMIT_VECTOR_SIZE, QDEPS_MEMBER_IMPL, TSeq)
{
public:
    typedef TSeq Seq;

    Pool() { }

#   define QDEPS_CTOR_INITLIST_IMPL(_, n, na)                                                                       \
        BOOST_PP_COMMA_IF(n) T##n(p##n)

#define QDEPS_CTOR_IMPL(_, n, na)\
    template<BOOST_PP_ENUM_PARAMS(n, typename T)> Pool(BOOST_PP_ENUM_BINARY_PARAMS(n, const T, &p))             \
        : BOOST_PP_REPEAT(n, QDEPS_CTOR_INITLIST_IMPL, ~)\
    { }

    BOOST_PP_REPEAT_FROM_TO(1, BOOST_MPL_LIMIT_VECTOR_SIZE, QDEPS_CTOR_IMPL, ~)
};

} // namespace Aux
} // namespace Back
} // namespace QDeps

#endif


