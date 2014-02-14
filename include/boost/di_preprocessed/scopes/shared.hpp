#ifndef BOOST_DI_SCOPES_SHARED_HPP
#define BOOST_DI_SCOPES_SHARED_HPP

    #include "boost/di_preprocessed/aux_/common.hpp"
    #include "boost/di_preprocessed/convertibles/shared.hpp"
    #include "boost/di_preprocessed/type_traits/create_traits.hpp"
    #include <boost/mpl/int.hpp>










    namespace boost {
    namespace di {
    namespace scopes {

    template<template<typename> class TConvertible = convertibles::shared>
    class shared
    {
    public:
        typedef mpl::int_<0> priority;

        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            result_type create() {
                if (!object_) {
                    object_.reset(type_traits::create_traits<TExpected, TGiven>());
                }
                return object_;
            }




    template< typename Args0>
    result_type create( Args0 args0) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1>
    result_type create( Args0 args0 , Args1 args1) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18)
            );
        }
        return object_;
    }




    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18 , Args19 args19) {
        if (!object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18 , args19)
            );
        }
        return object_;
    }

        private:
            result_type object_;
        };
    };

    }
    }
    }

#endif

