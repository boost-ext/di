#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_BACK_SCOPE_PERREQUEST_HPP
    #define QDEPS_BACK_SCOPE_PERREQUEST_HPP

    #include <boost/shared_ptr.hpp>
    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/enum_params.hpp>
    #include <boost/preprocessor/repetition/enum_binary_params.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include "QDeps/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, QDEPS_FUNCTION_ARITY_LIMIT_SIZE, "QDeps/Back/Scope/PerRequest.hpp"))

    namespace QDeps
    {
    namespace Back
    {
    namespace Scope
    {

    class PerRequest
    {
    public:
        template<typename T>
        struct ResultType
        {
            typedef boost::shared_ptr<T> type;
        };

        #include BOOST_PP_ITERATE()
    };

    } // namespace Scope
    } // namespace Back
    } // namespace QDeps

    #endif

#else

    template<typename T BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), typename Arg)>
    boost::shared_ptr<T> create(BOOST_PP_ENUM_BINARY_PARAMS(BOOST_PP_ITERATION(), const Arg, &p_arg))
    {
        boost::shared_ptr<T> l_instance(new T(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), p_arg)));
        return l_instance;
    }

#endif

