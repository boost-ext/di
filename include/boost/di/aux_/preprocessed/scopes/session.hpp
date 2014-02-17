//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_SCOPES_SESSION_HPP
#define BOOST_DI_SCOPES_SESSION_HPP

#include "boost/di/aux_/preprocessed/scopes/session.hpp"
#include "boost/di/aux_/config.hpp"
#include "boost/di/aux_/memory.hpp"
#include "boost/di/aux_/preprocessed/type_traits/create_traits.hpp"
#include "boost/di/aux_/preprocessed/convertibles/shared.hpp"
#include <cassert>
#include <boost/mpl/int.hpp>

    namespace boost {
    namespace di {
    namespace scopes {

    class session_entry { };
    class session_exit { };

    template<template<typename> class TConvertible = convertibles::shared>
    class session
    {
    public:
        typedef mpl::int_<0> priority;

        template<typename TExpected, typename TGiven = TExpected>
        class scope
        {
        public:
            typedef TConvertible<TExpected> result_type;

            scope()
                : in_scope_(false)
            { }

            void call(const session_entry&) {
                in_scope_ = true;
            }

            void call(const session_exit&) {
                in_scope_ = false;
                object_.reset();
            }

            result_type create() {
                if (in_scope_ && !object_) {
                    object_.reset(type_traits::create_traits<TExpected, TGiven>());
                }
                return object_;
            }

    template< typename Args0>
    result_type create( Args0 args0) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1>
    result_type create( Args0 args0 , Args1 args1) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18)
            );
        }
        return object_;
    }

    template< typename Args0 , typename Args1 , typename Args2 , typename Args3 , typename Args4 , typename Args5 , typename Args6 , typename Args7 , typename Args8 , typename Args9 , typename Args10 , typename Args11 , typename Args12 , typename Args13 , typename Args14 , typename Args15 , typename Args16 , typename Args17 , typename Args18 , typename Args19>
    result_type create( Args0 args0 , Args1 args1 , Args2 args2 , Args3 args3 , Args4 args4 , Args5 args5 , Args6 args6 , Args7 args7 , Args8 args8 , Args9 args9 , Args10 args10 , Args11 args11 , Args12 args12 , Args13 args13 , Args14 args14 , Args15 args15 , Args16 args16 , Args17 args17 , Args18 args18 , Args19 args19) {
        if (in_scope_ && !object_) {
            object_.reset(
                type_traits::create_traits<TExpected, TGiven>( args0 , args1 , args2 , args3 , args4 , args5 , args6 , args7 , args8 , args9 , args10 , args11 , args12 , args13 , args14 , args15 , args16 , args17 , args18 , args19)
            );
        }
        return object_;
    }

        private:
            result_type object_;
            bool in_scope_;
        };
    };

    }
    }
    }

#endif

