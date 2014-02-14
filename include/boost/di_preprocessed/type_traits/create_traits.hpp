#ifndef BOOST_DI_TYPE_TRAITS_CREATE_TRAITS_HPP
#define BOOST_DI_TYPE_TRAITS_CREATE_TRAITS_HPP

    #include "boost/di_preprocessed/aux_/common.hpp"
    #include "boost/di_preprocessed/type_traits/ctor_traits.hpp"
    #include <string>
    #include <boost/type.hpp>
    #include <boost/non_type.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/type_traits/is_class.hpp>
    #include <boost/mpl/string.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/if.hpp>
    #include <boost/mpl/bool.hpp>
    #include <boost/mpl/or.hpp>
    #include <boost/mpl/void.hpp>
    #include <boost/mpl/aux_/yes_no.hpp>












    namespace boost {
    namespace di {
    namespace type_traits {

    template<typename T>
    class has_value
    {
        struct base_impl { int value; };
        struct base
            : base_impl
            , mpl::if_<is_class<T>, T, mpl::void_>::type
        { base() { } };

        template<typename U>
        static mpl::aux::no_tag test(
            U*
          , non_type<int base_impl::*, &U::value>* = 0
        );

        static mpl::aux::yes_tag test(...);

    public:
        typedef has_value type;
        BOOST_STATIC_CONSTANT(
            bool
          , value = sizeof(test((base*)(0))) == sizeof(mpl::aux::yes_tag)
        );
    };

    template<typename>
    struct is_mpl_string
        : mpl::false_
    { };

    template< int C0 , int C1 , int C2 , int C3 , int C4 , int C5 , int C6 , int C7>
    struct is_mpl_string<mpl::string< C0 , C1 , C2 , C3 , C4 , C5 , C6 , C7> >
        : mpl::true_
    { };

    template<typename T>
    struct is_explicit
    {
        BOOST_STATIC_CONSTANT(
            bool
          , value = has_value<T>::value || is_mpl_string<T>::value
        );
    };

    template<typename TExpected, typename TGiven>
    typename disable_if<is_explicit<TGiven>, TExpected*>::type
    create_traits() {
        return new TGiven();
    }

    template<typename TExpected, typename TGiven>
    typename enable_if<has_value<TGiven>, TExpected*>::type
    create_traits() {
        return new TExpected(TGiven::value);
    }

    template<typename TExpected, typename TGiven>
    typename enable_if<is_mpl_string<TGiven>, TExpected*>::type
    create_traits() {
        return new TExpected(mpl::c_str<TGiven>::value);
    }

    template<typename T, typename TObject>
    inline TObject create_impl(TObject object, typename enable_if<is_same<T, detail::any_type> >::type* = 0) {
        return object;
    }

    template<typename T, typename TObject>
    inline T create_impl(TObject object, typename disable_if<is_same<T, detail::any_type> >::type* = 0) {
        return object(type<T>());
    }




    template<typename TExpected, typename TGiven, typename Args0>
    TExpected* create_traits( Args0 args0) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1>
    TExpected* create_traits( Args0 args0 , Args1 args1) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 10>::type>(args10)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 10>::type>(args10) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 11>::type>(args11)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 10>::type>(args10) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 11>::type>(args11) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 12>::type>(args12)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 10>::type>(args10) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 11>::type>(args11) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 12>::type>(args12) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 13>::type>(args13)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 10>::type>(args10) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 11>::type>(args11) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 12>::type>(args12) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 13>::type>(args13) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 14>::type>(args14)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 10>::type>(args10) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 11>::type>(args11) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 12>::type>(args12) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 13>::type>(args13) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 14>::type>(args14) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 15>::type>(args15)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 10>::type>(args10) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 11>::type>(args11) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 12>::type>(args12) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 13>::type>(args13) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 14>::type>(args14) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 15>::type>(args15) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 16>::type>(args16)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 10>::type>(args10) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 11>::type>(args11) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 12>::type>(args12) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 13>::type>(args13) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 14>::type>(args14) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 15>::type>(args15) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 16>::type>(args16) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 17>::type>(args17)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 10>::type>(args10) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 11>::type>(args11) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 12>::type>(args12) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 13>::type>(args13) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 14>::type>(args14) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 15>::type>(args15) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 16>::type>(args16) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 17>::type>(args17) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 18>::type>(args18)




        );
    }




    template<typename TExpected, typename TGiven, typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    TExpected* create_traits( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18 , Args19 args19) {
        return new TGiven(
            create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 0>::type>(args0) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 1>::type>(args1) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 2>::type>(args2) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 3>::type>(args3) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 4>::type>(args4) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 5>::type>(args5) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 6>::type>(args6) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 7>::type>(args7) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 8>::type>(args8) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 9>::type>(args9) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 10>::type>(args10) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 11>::type>(args11) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 12>::type>(args12) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 13>::type>(args13) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 14>::type>(args14) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 15>::type>(args15) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 16>::type>(args16) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 17>::type>(args17) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 18>::type>(args18) , create_impl<typename mpl::at_c<typename ctor_traits<TGiven>::type, 19>::type>(args19)




        );
    }

    }
    }
    }

#endif

