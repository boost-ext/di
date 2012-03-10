#include <boost/make_shared.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/limits/vector.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/reverse_fold.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <cxxabi.h>
#include <typeinfo>
#include <iostream>
#include <boost/mpl/int.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/type.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace QDeps
{
template<typename TScope, typename T0, typename T1, typename TContext> struct Impl { };

template
<
    typename TScope,
    typename TAttr,
    typename TValue = boost::mpl::_1,
    typename TContext = boost::mpl::vector0<>
>
struct Inst
{
    typedef TScope Scope;
    typedef TAttr Attr;
    typedef TValue Value;
    typedef TContext Context;
};

class PerRequest{};
class Singleton{};
} //

class I1{};
class D1{};
class D2{};
class T5{};
class T6{};
class T7{};
class T8{};

class Path_{};
//TODO T1, T2, ...
template<typename T0, typename T1, typename T2 = boost::mpl::void_> class Path : Path_
{
    typedef T0 t0;
    typedef boost::mpl::vector<T1, T2> leafs;
};
class Internal {};

template<typename T0, typename T1 = void>
class Singleton : Internal {
public:
    typedef T0 t0;
    typedef T1 t1;
};

template<typename T0, typename T1 = void>
class PerRequest : Internal {
public:
    typedef T0 t0;
    typedef T1 t1;
};


template<typename T, typename TContext> class CreateDependency;

template<typename T, typename TContext>
class CreateDependency<PerRequest<T>, TContext>
{
public:
    typedef QDeps::Inst<QDeps::PerRequest, T, boost::mpl::_1, TContext> type;
};

template<typename T1, typename T2, typename TContext>
class CreateDependency<PerRequest<T1, T2>, TContext>
{
public:
    typedef QDeps::Impl<QDeps::PerRequest, T1, T2, TContext> type;
};

template<typename T, typename TContext>
class CreateDependency<Singleton<T>, TContext>
{
public:
    typedef QDeps::Inst<QDeps::Singleton, T, boost::mpl::_1, TContext> type;
};

template<typename T1, typename T2, typename TContext>
class CreateDependency<Singleton<T1, T2>, TContext>
{
public:
    typedef QDeps::Impl<QDeps::Singleton, T1, T2, TContext> type;
};

template<typename T> class GetDependency;

template<typename T>
class GetDependency<PerRequest<T> >
{
public:
    typedef T type;
};

template<typename T1, typename T2>
class GetDependency<PerRequest<T1, T2> >
{
public:
    typedef T2 type;
};

template<typename T>
class GetDependency<Singleton<T> >
{
public:
    typedef T type;
};

template<typename T1, typename T2>
class GetDependency<Singleton<T1, T2> >
{
public:
    typedef T2 type;
};

template<typename T>
struct GetLeafs
{
    typedef typename T::leafs type;
};

template<typename T>
struct GetT0
{
    typedef typename T::t0 type;
};

template<typename T>
struct GetT1
{
    typedef typename T::t1 type;
};


typedef boost::mpl::vector
<
    Path
    <
        Singleton<I1, D1>,
        //T6,
        //T7
        Path<
            Singleton<T5>, T6
        >
    >
    //,PerRequest<I1, D2>
>
types;

template<typename TBinding, typename TSeq = boost::mpl::vector0<>, typename TContext = boost::mpl::vector0<>, typename T = boost::mpl::vector0<> >
struct PathImpl :
    boost::mpl::if_
    <
        boost::mpl::is_sequence<TBinding>,
        typename boost::mpl::fold
        <
            TBinding,
            TSeq,
            boost::mpl::if_
            <
                boost::is_base_of<boost::mpl::void_, boost::mpl::_2>,
                boost::mpl::_1,

                boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
/*                boost::mpl::if_*/
                //<
                    //boost::mpl::is_sequence<TContext>,
                    //boost::mpl::if_
                    //<
                        //boost::is_base_of<Path_, boost::mpl::_2>,
                        //PathImpl<GetLeafs<boost::mpl::_2>, boost::mpl::_1, boost::mpl::push_back<TContext, GetT0<boost::mpl::_2> > >,
                        ////boost::mpl::if_
                        ////<
                            ////boost::is_base_of<Internal, boost::mpl::_2>,
                            ////boost::mpl::push_back<boost::mpl::_1, CreateDependency<boost::mpl::_2, TContext> >,
                            ////boost::mpl::push_back<boost::mpl::_1, CreateDependency<TContext, boost::mpl::_2> >
                            //PathImpl<TContext, boost::mpl::_1, boost::mpl::_2, boost::mpl::vector1<boost::mpl::_2> >
                ////boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>
                        ////>
                    //>,
                    ////boost::mpl::push_back<boost::mpl::_1, CreateDependency<boost::mpl::_2, T> >
                    //PathImpl<TContext, boost::mpl::_1, boost::mpl::_2, boost::mpl::push_back<T, boost::mpl::_2> >
                /*>*/
            >
        >::type,
        boost::mpl::push_back<boost::mpl::_1, CreateDependency<T, TContext> >
    >
{ };

using namespace boost::mpl;

template<typename T, typename TContext, typename TResult>
class PathImpl2_
:
    fold
    <
        T,
        TResult,
        boost::mpl::push_back<boost::mpl::_1, CreateDependency< boost::mpl::_2, TContext> >
    >
{ };

template<typename T>
struct GG : fold
    <
        T,
        vector0<>,
        push_back<_1, _2>
    >
{};

template<typename T, typename TResult = boost::mpl::vector0<> >
struct PathImpl_ :
    fold
    <
        T,
        TResult,
        if_
        <
            boost::is_base_of<boost::mpl::void_, boost::mpl::_2>,
            boost::mpl::_1,

            if_
            <
                boost::is_base_of<Path_, boost::mpl::_2>,
                PathImpl_<GetLeafs<boost::mpl::_2>, boost::mpl::push_back<_1, GetT0<boost::mpl::_2> > >,
                _1
            >
        >
    >
{ };

template<typename T, typename TResult = boost::mpl::vector0<> >
struct PathImpl__ :
    fold
    <
        T,
        TResult,
        if_
        <
            boost::is_base_of<boost::mpl::void_, boost::mpl::_2>,
            boost::mpl::_1,

            if_
            <
                boost::is_base_of<Path_, boost::mpl::_2>,
                PathImpl__<GetLeafs<boost::mpl::_2>, push_back<_1, GetDependency< GetT0<_2> > > >,
                push_back<_1, _2>
            >
        >
    >
{ };

template<typename TSeq, typename S>
struct Do : fold
    <
        TSeq,
        vector0<>,
        if_
        <
            greater< size<_1>, S>,
            push_back<_1, _2>,
            _1
        >
    >
{ };

template<typename Seq, typename T1 = typename PathImpl_<Seq>::type, typename T2 = typename PathImpl__<Seq>::type>
struct Merge : fold
    <
        T1,
        vector0<>,
        boost::mpl::push_back<_1, CreateDependency<_2, typename Do<T2, size<_1> >::type > >
    >

{ };

class Print
{
public:
    template<typename T> void operator()(T)
    {
        std::cout << abi::__cxa_demangle(typeid(T).name(), 0, 0, 0) << std::endl;
    }
};

int main()
{
    boost::mpl::for_each<Merge<types>::type>(Print());

    return 0;
}

